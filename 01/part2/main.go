package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

func main() {
	wd, err := os.Getwd()
	if err != nil {
		panic(err)
	}
	parent := filepath.Dir(wd)
	content, err := ioutil.ReadFile(filepath.Join(parent, "in"))
	if err != nil {
		panic(err)
	}

	measurementsStr := strings.Split(strings.TrimSpace(string(content)), "\n")
	measurements := make([]int, len(measurementsStr))
	for i, m := range measurementsStr {
		measurements[i], err = strconv.Atoi(m)
		if err != nil {
			panic(err)
		}
	}

	windowSize := 3
	increasedCount := 0
	for i := 1; i < len(measurements)-windowSize+1; i++ {
		previousWindow := 0
		for j := 0; j < windowSize; j++ {
			previousWindow += measurements[i+j-1]
		}
		currentWindow := 0
		for j := 0; j < windowSize; j++ {
			currentWindow += measurements[i+j]
		}
		if previousWindow < currentWindow {
			increasedCount++
		}
	}
	fmt.Println(increasedCount)
}
