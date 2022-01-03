package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

const STEPS = 100

func main() {
	if len(os.Args) < 2 {
		panic("Please provide the input file as argument")
	}

	fileContentBytes, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		panic(err)
	}
	fileContent := strings.TrimSpace(string(fileContentBytes))

	grid := NewOctopusesGridFromString(fileContent)

	totalFlashes := 0
	for i := 0; i < STEPS; i++ {
		for _, octopus := range grid.AllOctopuses() {
			octopus.IncreaseEnergy()
		}
		stillFlashing := true
		for stillFlashing {
			foundOctopusFlashing := false
			for _, octopus := range grid.AllOctopuses() {
				if octopus.IsFlashing() {
					totalFlashes += 1
					octopus.TurnOffFlashing()
					foundOctopusFlashing = true
					for _, neighbor := range grid.Neighbors(octopus) {
						neighbor.IncreaseEnergy()
					}
				}
			}
			stillFlashing = foundOctopusFlashing
		}
		for _, octopus := range grid.AllOctopuses() {
			octopus.NextStep()
		}
	}

	fmt.Println(totalFlashes)
}
