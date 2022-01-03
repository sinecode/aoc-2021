package main

import (
	"fmt"
	"strconv"
	"strings"
)

type OctopusesGrid struct {
	octopuses [][]*Octopus
}

func NewOctopusesGridFromString(in string) *OctopusesGrid {
	lines := strings.Split(in, "\n")
	octopuses := make([][]*Octopus, 0)
	for x, line := range lines {
		octopusesLine := make([]*Octopus, 0)
		for y, energy := range line {
			energyValue, err := strconv.Atoi(string(energy))
			if err != nil {
				panic(err)
			}
			octopusesLine = append(octopusesLine, NewOctopus(x, y, energyValue))
		}
		octopuses = append(octopuses, octopusesLine)
	}
	return &OctopusesGrid{
		octopuses: octopuses,
	}
}

func (o *OctopusesGrid) AllOctopuses() []*Octopus {
	rst := make([]*Octopus, 0)
	for _, octopusesLine := range o.octopuses {
		for _, octopus := range octopusesLine {
			rst = append(rst, octopus)
		}
	}
	return rst
}

func (o *OctopusesGrid) Neighbors(oct *Octopus) []*Octopus {
	rst := make([]*Octopus, 0)
	for _, octopusesLine := range o.octopuses {
		for _, octopus := range octopusesLine {
			if oct.IsNeighbor(octopus) {
				rst = append(rst, octopus)
			}
		}
	}
	return rst
}
func (o *OctopusesGrid) PrintGrid() {
	for _, octopusesLine := range o.octopuses {
		for _, octopus := range octopusesLine {
			fmt.Printf("%d", octopus.Energy())
		}
		fmt.Printf("\n")
	}
}
