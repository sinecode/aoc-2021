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
	measurements := strings.Split(strings.TrimSpace(string(content)), "\n")
	if len(measurements) == 0 {
		fmt.Println(0)
		return
	}
	increasedCount := 0
	previous, err := strconv.Atoi(measurements[0])
	if err != nil {
		panic(err)
	}
	for i := 1; i < len(measurements); i++ {
		n, err := strconv.Atoi(measurements[i])
		if err != nil {
			panic(err)
		}
		if previous < n {
			increasedCount++
		}
		previous = n
	}
	fmt.Println(increasedCount)
}
