package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

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

	step := 1
	for ; ; step++ {
		for _, octopus := range grid.AllOctopuses() {
			octopus.IncreaseEnergy()
		}
		stillFlashing := true
		for stillFlashing {
			foundOctopusFlashing := false
			for _, octopus := range grid.AllOctopuses() {
				if octopus.IsFlashing() {
					octopus.TurnOffFlashing()
					foundOctopusFlashing = true
					for _, neighbor := range grid.Neighbors(octopus) {
						neighbor.IncreaseEnergy()
					}
				}
			}
			stillFlashing = foundOctopusFlashing
		}
		allFlashing := true
		for _, octopus := range grid.AllOctopuses() {
			allFlashing = allFlashing && octopus.Energy() == 0
			octopus.NextStep()
		}
		if allFlashing {
			break
		}
	}

	fmt.Println(step)
}
