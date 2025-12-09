package main

import (
	"fmt"
	"log"
	"os"
	"strings"
)

var (
	traceCache map[string]int
)

func init() {
	traceCache = make(map[string]int)
}

func readInput(filePath string) string {
	b, err := os.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}
	// return split by line
	return string(b)
}

func parseInput(input string) ([]string, int) {
	lines := strings.Split(input, "\n")
	start := strings.Index(lines[0], "S")
	if start == -1 {
		panic("No starting point 'S' found")
	}
	return lines, start
}

func Part1(input string) int {
	lines, start := parseInput(input)
	count := 0
	// keep track of beam locations in a map (uniqueness)
	// for current row only, will delete old keys
	beamLocations := map[int]bool{start: true}
	for rowIdx := 1; rowIdx < len(lines); rowIdx++ {
		for beamLoc := range beamLocations {
			if lines[rowIdx][beamLoc] == '^' {
				count++
				// beam splits
				delete(beamLocations, beamLoc)
				if beamLoc > 0 { // start bound
					beamLocations[beamLoc-1] = true
				}
				if beamLoc < len(lines[rowIdx])-1 { // end bound
					beamLocations[beamLoc+1] = true
				}
			}
		}

	}
	return count
}

func linesAndLocToKey(lines []string, beamLoc int) string {
	// create unique key for current state
	// lines length, beam location
	return fmt.Sprintf("%d|%d", len(lines), beamLoc)
}

// uses module level cache
func traceBeamPossibilities(lines []string, beamLoc int) int {
	// check cache first
	key := linesAndLocToKey(lines, beamLoc)
	if ret, ok := traceCache[key]; ok {
		return ret
	}
	timelines := 0

	if len(lines) <= 1 {
		return timelines
	}

	//consider first line only and recurse for next
	if lines[0][beamLoc] == '^' {
		//split
		timelines += 1
		if beamLoc > 0 { // start bound
			timelines += traceBeamPossibilities(lines[1:], beamLoc-1)
		}
		if beamLoc < len(lines[0])-1 { //end bound
			timelines += traceBeamPossibilities(lines[1:], beamLoc+1)
		}
	} else {
		//current trajectory
		timelines += traceBeamPossibilities(lines[1:], beamLoc)
	}
	traceCache[linesAndLocToKey(lines, beamLoc)] = timelines
	return timelines
}

func Part2(input string) int {
	lines, start := parseInput(input)

	return traceBeamPossibilities(lines, start) + 1 // +1 for initial

}

func main() {
	input := readInput("input.txt")
	part1Ans := Part1(input)
	fmt.Printf("Part 1: %d\n", part1Ans)

	part2Ans := Part2(input)
	fmt.Printf("Part 2: %d\n", part2Ans)
}
