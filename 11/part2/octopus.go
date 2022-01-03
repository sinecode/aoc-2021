package main

import (
	"math"
)

const OCTUPUS_MAX_ENERGY = 10

type Octopus struct {
	x           int
	y           int
	energy      int
	isFlashing  bool
	canIncrease bool
}

func NewOctopus(x, y, startEnergy int) *Octopus {
	return &Octopus{
		x:           x,
		y:           y,
		energy:      startEnergy,
		isFlashing:  startEnergy == 0,
		canIncrease: startEnergy != 0,
	}
}

func (o *Octopus) Energy() int {
	return o.energy
}

func (o *Octopus) IncreaseEnergy() {
	if o.canIncrease {
		o.energy = (o.energy + 1) % OCTUPUS_MAX_ENERGY
		o.isFlashing = o.energy == 0
		o.canIncrease = o.energy != 0
	}
}

func (o *Octopus) IsFlashing() bool {
	return o.isFlashing
}

func (o *Octopus) TurnOffFlashing() {
	o.isFlashing = false
}

func (o *Octopus) NextStep() {
	o.canIncrease = true
}

func (o *Octopus) IsNeighbor(other *Octopus) bool {
	return !(o.x == other.x && o.y == other.y) &&
		(math.Abs(float64(o.x-other.x)) <= 1 && math.Abs(float64(o.y-other.y)) <= 1)
}
