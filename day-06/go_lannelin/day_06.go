package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

var operatorMap = map[string]func(int, int) int{
	"+": func(a, b int) int { return a + b },
	"*": func(a, b int) int { return a * b },
}

func readInput(filePath string) string {
	b, err := os.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}
	// return split by line
	return string(b)
}

type Problem struct {
	nums      []int
	operators string
}

func part1ParseInput(input string) []Problem {
	// assume all lines have same number of columns
	lines := strings.Split(input, "\n")
	operators := strings.Fields(lines[len(lines)-1])
	lines = lines[:len(lines)-1]
	partsByLine := make([][]string, len(lines))
	for i, line := range lines {
		partsByLine[i] = strings.Fields(line)
	}
	problems := make([]Problem, len(partsByLine[0]))
	// invert structure and form "problems"
	for colIdx := 0; colIdx < len(partsByLine[0]); colIdx++ {
		nums := make([]int, len(partsByLine))
		for rowIdx := 0; rowIdx < len(partsByLine); rowIdx++ {
			num, err := strconv.Atoi(partsByLine[rowIdx][colIdx])
			if err != nil {
				log.Fatalf("failed to parse number %q: %v", partsByLine[rowIdx][colIdx], err)
			}
			nums[rowIdx] = num
		}
		problems[colIdx] = Problem{
			nums:      nums,
			operators: operators[colIdx],
		}
	}

	return problems

}

func part2ExtractCols(lines []string) [][]string {
	nCols := len(strings.Fields(lines[0]))
	//calculate column delimits, chunking to separate cols
	cols := make([][]string, nCols)
	colStart := 0
	colCounter := 0
	// move cursor along LTR
	for cursor := 0; cursor <= len(lines[0]); cursor++ {
		spaceCounter := 0
		if cursor == len(lines[0]) {
			spaceCounter = len(lines) // force final col capture
		} else {
			for rIdx := 0; rIdx < len(lines); rIdx++ {
				if lines[rIdx][cursor] == ' ' {
					spaceCounter++
				}
			}
		}
		if spaceCounter == len(lines) {
			// loop again through rows to construct col
			col := make([]string, len(lines))
			for rIdx := 0; rIdx < len(lines); rIdx++ {
				col[rIdx] = lines[rIdx][colStart:cursor]
			}
			cols[colCounter] = col
			colCounter++
			colStart = cursor + 1
		}

	}
	return cols
}

func part2ParseInput(input string) []Problem {
	lines := strings.Split(input, "\n")
	// assume at least one line + operators
	operators := strings.Fields(lines[len(lines)-1])

	lines = lines[:len(lines)-1]

	cols := part2ExtractCols(lines)

	problems := make([]Problem, len(cols))

	// for each col, read top to bottom RTL
	for i, col := range cols {
		colWidth := len(col[0])
		nums := make([]int, colWidth)
		for colIdx := colWidth - 1; colIdx >= 0; colIdx-- {
			numStr := ""
			for rIdx := 0; rIdx < len(col); rIdx++ {
				if col[rIdx][colIdx] == ' ' {
					continue
				}
				numStr += string(col[rIdx][colIdx])
			}
			num, err := strconv.Atoi(numStr)
			if err != nil {
				log.Fatalf("failed to parse number %q: %v", numStr, err)
			}
			nums[colIdx] = num
			problems[i] = Problem{
				nums:      nums,
				operators: operators[i],
			}
		}
	}

	return problems

}

func calcTotal(problems []Problem) int {
	total := 0
	for _, problem := range problems {
		f := operatorMap[problem.operators]
		result := problem.nums[0]
		// no apply :(
		for i := 1; i < len(problem.nums); i++ {
			result = f(result, problem.nums[i])
		}
		total += result
	}

	return total
}

func Part1(input string) int {
	return calcTotal(part1ParseInput(input))
}

func Part2(input string) int {
	return calcTotal(part2ParseInput(input))

}

func main() {
	input := readInput("input.txt")
	part1Ans := Part1(input)
	fmt.Printf("Part 1: %d\n", part1Ans)

	part2Ans := Part2(input)
	fmt.Printf("Part 2: %d\n", part2Ans)
}
