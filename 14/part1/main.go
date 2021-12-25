package main

import (
	"bufio"
	"fmt"
	"io"
	"math"
	"os"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		panic("Please provide the input file as argument")
	}

	file, err := os.Open(os.Args[1])
	if err != nil {
		panic(err)
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	template, err := readStartTemplate(reader)
	if err != nil {
		panic(err)
	}

	// Skip line
	_, err = reader.ReadString('\n')
	if err != nil {
		panic(err)
	}

	rules, err := readRules(reader)
	if err != nil {
		panic(err)
	}

	steps := 10

	for step := 0; step < steps; step++ {
		var newTemplateBuilder strings.Builder
		for i := 0; i < len(template)-1; i++ {
			pair := fmt.Sprintf("%c%c", template[i], template[i+1])
			newElem, ok := rules[pair]
			if !ok {
				fmt.Fprintf(&newTemplateBuilder, "%c", pair[0])
			} else {
				fmt.Fprintf(&newTemplateBuilder, "%c%s", pair[0], newElem)
			}
		}
		fmt.Fprintf(&newTemplateBuilder, "%c", template[len(template)-1])

		template = newTemplateBuilder.String()
	}

	counter := make(map[rune]int)
	for _, chr := range template {
		if _, ok := counter[chr]; !ok {
			counter[chr] = 1
		} else {
			counter[chr] += 1
		}
	}

	mostCommonCount := math.MinInt
	leastCommonCount := math.MaxInt
	for _, v := range counter {
		if v > mostCommonCount {
			mostCommonCount = v
		}
		if v < leastCommonCount {
			leastCommonCount = v
		}
	}
	fmt.Println(mostCommonCount - leastCommonCount)
}

func readStartTemplate(reader *bufio.Reader) (string, error) {
	template, err := reader.ReadString('\n')
	if err != nil {
		return "", err
	}
	return strings.TrimSpace(template), nil
}

func readRules(reader *bufio.Reader) (map[string]string, error) {
	rules := make(map[string]string)
	for {
		ruleLine, err := reader.ReadString('\n')
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, err
		}
		ruleLine = strings.TrimSpace(ruleLine)
		ruleLine = strings.ReplaceAll(ruleLine, " ->", "")
		tokens := strings.Split(ruleLine, " ")
		rules[tokens[0]] = tokens[1]
	}
	return rules, nil
}
