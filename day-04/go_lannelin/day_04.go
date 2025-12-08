package main

import (
	"fmt"
	"log"
	"os"
	"strings"
)

func readInput(filePath string) string {
	b, err := os.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}
	// return split by line
	return string(b)
}

func parseInput(input string) [][]bool {
	lines := strings.Split(input, "\n")
	// assume all lines are the same length
	// make grid with surrounding false buffer
	//assume non empty input
	gridHeight := len(lines) + 2
	gridWidth := len(lines[0]) + 2

	grid := make([][]bool, gridHeight)

	// init buffer rows
	grid[0] = make([]bool, gridWidth)
	grid[gridHeight-1] = make([]bool, gridWidth)

	for i, line := range lines {
		grid[i+1] = make([]bool, len(line)+2) // default false
		for j, char := range line {
			if char == '@' {
				// +1s for buffer
				grid[i+1][j+1] = true
			}
		}
	}
	return grid
}

func countPaperNeighbours(grid [][]bool, selfI int, selfJ int) int {
	// start negative to allow for self
	count := -1
	for i := selfI - 1; i <= selfI+1; i++ {
		for j := selfJ - 1; j <= selfJ+1; j++ {
			if grid[i][j] {
				count++
			}
		}
	}
	return count
}

func Part1(input string) int {
	grid := parseInput(input)
	count := 0
	// iterate through grid, ignoring buffer
	for i := 1; i < len(grid)-1; i++ {
		for j := 1; j < len(grid[i])-1; j++ {
			if grid[i][j] && countPaperNeighbours(grid, i, j) < 4 {
				count++
			}

		}
	}
	return count
}

func Part2(input string) int {
	// create list of coords to visit
	// if we move paper, we need to revisit prev neighbours
	grid := parseInput(input)
	toVisit := make([][2]int, 0)
	for i := 1; i < len(grid)-1; i++ {
		for j := 1; j < len(grid[i])-1; j++ {
			if grid[i][j] {
				toVisit = append(toVisit, [2]int{i, j})
			}
		}
	}

	count := 0
	for len(toVisit) > 0 {
		// pop from stack
		coord := toVisit[len(toVisit)-1]
		toVisit = toVisit[:len(toVisit)-1]
		i, j := coord[0], coord[1]
		if grid[i][j] && countPaperNeighbours(grid, i, j) < 4 {
			count++
			// move paper
			grid[i][j] = false
			// need to revisit neighbours
			// TODO optimise to only add paper that isn't already in toVisit
			for ii := i - 1; ii <= i+1; ii++ {
				for jj := j - 1; jj <= j+1; jj++ {
					if grid[ii][jj] {
						toVisit = append(toVisit, [2]int{ii, jj})
					}
				}
			}
		}

	}
	return count

}

func main() {
	input := readInput("input.txt")
	part1Ans := Part1(input)
	fmt.Printf("Part 1: %d\n", part1Ans)

	part2Ans := Part2(input)
	fmt.Printf("Part 2: %d\n", part2Ans)
}
