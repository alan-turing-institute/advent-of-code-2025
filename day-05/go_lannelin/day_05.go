package main

import (
	"fmt"
	"log"
	"os"
	"sort"
	"strconv"
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

func parseRanges(input string) [][2]int {
	lines := strings.Split(input, "\n")
	ranges := make([][2]int, len(lines))
	for i, line := range lines {
		parts := strings.Split(line, "-")
		start, err := strconv.Atoi(parts[0])
		if err != nil {
			log.Fatalf("invalid range start: %v", err)
		}
		end, err := strconv.Atoi(parts[1])
		if err != nil {
			log.Fatalf("invalid range end: %v", err)
		}
		ranges[i] = [2]int{start, end}
	}
	return ranges

}

func parseIngredients(input string) []int {
	lines := strings.Split(input, "\n")
	ingredients := make([]int, len(lines))
	for i, line := range lines {
		ing, err := strconv.Atoi(line)
		if err != nil {
			log.Fatalf("invalid ingredient: %v", err)
		}
		ingredients[i] = ing
	}
	return ingredients
}

func parseInput(input string) ([][2]int, []int) {
	// split by double new line for ranges, ingredients
	sections := strings.Split(input, "\n\n")
	if len(sections) != 2 {
		log.Fatalf("expected 2 sections (ranges, ingredients), got %d", len(sections))
	}
	ranges := parseRanges(sections[0])
	ingredients := parseIngredients(sections[1])

	return ranges, ingredients

}

func Part1(input string) int {
	ranges, ingredients := parseInput(input)
	count := 0
	for _, ingredient := range ingredients {
		for _, r := range ranges {
			if ingredient >= r[0] && ingredient <= r[1] {
				//fmt.Printf("fresh: %d . in range %d-%d\n", ingredient, r[0], r[1])
				count++
				break // no need to check other ranges
			}
		}
	}
	return count
}

func Part2(input string) int {
	ranges, _ := parseInput(input)

	// total items in all ranges (accounting for overlaps)
	count := 0
	// order ranges by start
	sort.Slice(ranges, func(i, j int) bool {
		if ranges[i][0] == ranges[j][0] {
			return ranges[i][1] < ranges[j][1]
		}
		return ranges[i][0] < ranges[j][0]
	})
	fmt.Printf("sorted ranges\n")
	for _, r := range ranges {
		fmt.Printf("  %d-%d\n", r[0], r[1])
	}
	// remove overlaps
	// add range 0
	count += ranges[0][1] - ranges[0][0] + 1
	for i := 1; i < len(ranges); i++ {
		// non overwrite
		// if current starts before end of prev
		if ranges[i][0] <= ranges[i-1][1] {
			// if ends before end of prev, entirely contained
			// "remove" by replacing with prev vals and skip
			if ranges[i][1] <= ranges[i-1][1] {
				ranges[i][0] = ranges[i-1][0]
				ranges[i][1] = ranges[i-1][1]
				continue
			}
			// set to end of prev
			ranges[i][0] = ranges[i-1][1] + 1
		}
		count += ranges[i][1] - ranges[i][0] + 1
	}

	fmt.Printf("\n\nedited ranges\n")
	for _, r := range ranges {
		fmt.Printf("  %d-%d\n", r[0], r[1])
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
