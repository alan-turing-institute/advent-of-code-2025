package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

// constants
const (
	startLoc = 50
)

func readInput(filePath string) string {
	b, err := os.ReadFile(filePath) // just pass the file name
	if err != nil {
		log.Fatal(err)
	}
	return string(b)
}

// each line [LR][0-9]+
// L or R indicates direction to turn
// number indicates how far to turn
func parseLine(line string) (byte, int) {
	dir := line[0]
	turn, err := strconv.Atoi(line[1:])
	if err != nil {
		fmt.Printf("couldn't convert turn amount to int: %s", line[1:])
		panic(err)
	}
	return dir, turn
}

// turn dial (0-99) left or right by turn amount
func turnDial(loc int, turn int, direction byte) int {
	// remove redundant full rotations
	turn = turn % 100
	if direction == 'L' {
		turn = 100 - turn // left turn is equivalent to turning right (100 - x)
	}
	loc = (loc + turn) % 100
	return loc
}

func Part1(input string) int {
	// split string by lines
	lines := strings.Split(input, "\n")
	counter := 0
	loc := startLoc
	for _, line := range lines {
		dir, turn := parseLine(line)
		loc = turnDial(loc, turn, dir)
		// we only care about ending up on 0
		if loc == 0 {
			counter++
		}
	}
	return counter

}

func Part2(input string) int {
	// split string by lines
	lines := strings.Split(input, "\n")
	counter := 0
	loc := startLoc
	for _, line := range lines {
		dir, turn := parseLine(line)
		newLoc := turnDial(loc, turn, dir)
		// we only care about landing on or crossing 0
		if (newLoc == 0) || (newLoc < loc && dir == 'R') || (loc != 0 && newLoc > loc && dir == 'L') {
			counter++
		}
		// account for turns past 100
		counter += turn / 100
		loc = newLoc
	}

	return counter
}

func main() {
	input := readInput("input.txt")
	part1Ans := Part1(input)
	fmt.Printf("Part 1: %d\n", part1Ans)

	part2Ans := Part2(input)
	fmt.Printf("Part 2: %d\n", part2Ans)
}
