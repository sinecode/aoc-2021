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
	startTemplate, err := readStartTemplate(reader)
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

	pairs := make(map[string]int)
	for i := 0; i < len(startTemplate)-1; i++ {
		pair := fmt.Sprintf("%c%c", startTemplate[i], startTemplate[i+1])
		if _, ok := pairs[pair]; !ok {
			pairs[pair] = 1
		} else {
			pairs[pair] += 1
		}
	}

	steps := 40

	for step := 0; step < steps; step++ {
		newPairs := make(map[string]int)
		for pair, pairCount := range pairs {
			newElem, ok := rules[pair]
			if !ok {
				newPairs[pair] = pairCount
				continue
			}
			newPairLeft := fmt.Sprintf("%c%s", pair[0], newElem)
			if _, ok := newPairs[newPairLeft]; !ok {
				newPairs[newPairLeft] = pairCount
			} else {
				newPairs[newPairLeft] += pairCount
			}
			newPairRight := fmt.Sprintf("%s%c", newElem, pair[1])
			if _, ok := newPairs[newPairRight]; !ok {
				newPairs[newPairRight] = pairCount
			} else {
				newPairs[newPairRight] += pairCount
			}
		}
		pairs = newPairs
	}

	counter := make(map[rune]int)
	for pair, pairCount := range pairs {
		// Always count the first element of the pair
		chr := rune(pair[0])
		if _, ok := counter[chr]; !ok {
			counter[chr] = pairCount
		} else {
			counter[chr] += pairCount
		}
	}

	// Add the last element because it was not counted
	lastChr := startTemplate[len(startTemplate)-1]
	counter[rune(lastChr)] += 1

	mostCommonCount := math.MinInt
	leastCommonCount := math.MaxInt
	for _, n := range counter {
		if n > mostCommonCount {
			mostCommonCount = n
		}
		if n < leastCommonCount {
			leastCommonCount = n
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
