package day1

import (
	"advent-of-code-2025/utils"
	"fmt"
	"math"
	"strconv"
	"strings"
)

// Part1 solves part 1 of day 1
func Part1(inputPath string) (interface{}, error) {
	/*
		Day one part 1 solution:

		Provided with left or right turns on a dial and number of 'clicks' to turn. The dial has 99
		positions, and we start at 0. The problem is to find how many times we land on position 0 again.

		To solve this, parse whether the turn is left or right, and do addition or subtraction modulo 99,
		upping a counter if 0 is hit (presumably we start at 0 so we count that initial position also).
		Finally return the count of times we hit 0.
	*/

	// read input file
	input, err := utils.ReadInputFile(inputPath)
	if err != nil {
		return nil, err
	}
	fmt.Printf("Successfully read input\n")

	// Count the number of times we land on 0
	count := 0
	position := 50 // problem states that we start on 50
	for _, line := range input {
		instruction, err := parseClickInstruction(line)
		if err != nil {
			return nil, err
		}
		position = (position + instruction) % 100
		if position == 0 {
			count++
		}
	}

	return count, nil
}

func parseClickInstruction(line string) (int, error) {
	/*
		Input of the form RX or LX where X is an integer. Take this input, strip the R or the L
		and return the number as a positive interger for R and negative integer for L.
	*/

	var instruction int
	if strings.Contains(line, "R") {
		// right turn
		num := strings.TrimPrefix(line, "R")
		n, err := strconv.Atoi(num)
		if err != nil {
			return 0, err
		}
		instruction = n
	} else if strings.Contains(line, "L") {
		// left turn
		num := strings.TrimPrefix(line, "L")
		n, err := strconv.Atoi(num)
		if err != nil {
			return 0, err
		}
		instruction = -n
	}
	return instruction, nil
}

// Part2 solves part 2 of day 1
func Part2(inputPath string) (interface{}, error) {
	/*
		Day one part 2 solution:

		Now we need to work out any time a click passes 0, not just when it lands on it.

		To do this we can see if the position crosses 0 when we add or subtract the instruction, and increment
		the count then as well as if it lands on it.
	*/

	// Read input
	input, err := utils.ReadInputFile(inputPath)
	if err != nil {
		return "", err
	}
	fmt.Printf("Successfully read input\n")

	// Count number of times we pass or land on 0
	count := 0
	position := 50
	for _, line := range input {
		instruction, err := parseClickInstruction(line)
		if err != nil {
			return "", err
		}
		zeroCrosses := 0
		move := position + instruction
		if move < 0 {
			move = -move
			zeroCrosses++
		}
		zeroCrosses = int(math.Floor(float64(move) / 100))
		count += int(zeroCrosses)

		position = (position + instruction) % 100
		if position == 0 {
			count++
		}
	}

	return count, nil
}
