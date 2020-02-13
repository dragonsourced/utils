package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

type entry struct {
	time        time.Time
	timestamp   string
	title       string
	link        string
	content     string
	contentType string
	id          string
	author      string
	enclosure   string
}

func parseLine(line string) entry {
	words := strings.Split(line, "\t")

	var e entry
	attrs := []*string{
		&e.timestamp,
		&e.title,
		&e.link,
		&e.content,
		&e.contentType,
		&e.id,
		&e.author,
		&e.enclosure,
	}

	lng := len(attrs)
	for i, w := range words {
		if i < lng {
			*attrs[i] = w
		}
	}

	i, err := strconv.ParseInt(e.timestamp, 10, 64)
	if err != nil {
		panic(err)
	}
	e.time = time.Unix(i, 0)

	// If the author field is still empty, use the link instead.
	if e.author == "" {
		urlbits := strings.Split(e.link, "/")
		if len(urlbits) < 1 {
			return e
		}
		if strings.Contains(e.link, "://") {
			urlbits = urlbits[2:]
		}
		e.author = urlbits[0]
	}

	return e
}

func (e *entry) print(args []string) {
	corresponds := map[string]*string{
		"timestamp":    &e.timestamp,
		"title":        &e.title,
		"link":         &e.link,
		"content":      &e.content,
		"content_type": &e.contentType,
		"id":           &e.id,
		"author":       &e.author,
		"enclosure":    &e.enclosure,
	}

	var toprint []string

	for _, arg := range args {
		if corresponds[arg] != nil {
			toprint = append(toprint, *corresponds[arg])
		}
	}

        fmt.Println(strings.Join(toprint, "\t"))
}

// New == < 24 hours old
func (e entry) isNew() bool {
	return time.Since(e.time).Hours() < 24
}

func main() {
	var (
		text string
		err  error
	)

	reader := bufio.NewReader(os.Stdin)
	entries := []entry{}
	args := os.Args[1:]

        if len(args) < 1 {
            println("Usage: sfeed_fmt [new] <attr>...")
            return
        }

	text, err = reader.ReadString('\n')
	for err == nil {
		line := strings.TrimRight(text, "\n")
		entries = append(entries, parseLine(line))
		text, err = reader.ReadString('\n')
	}

	old := true
	if len(args) > 0 && args[0] == "new" {
		old = false
	}
	for _, entry := range entries {
		if old || entry.isNew() {
			entry.print(args)
		}
	}
}
