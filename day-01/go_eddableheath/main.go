package main

import (
	"advent-of-code-2025/day1"
	"flag"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	// Parse command-line arguments
	flag.Parse()
	args := flag.Args()
	if len(args) != 3 {
		fmt.Println("Usage: go run . day_X part_Y path/to/input")
		fmt.Println("Example: go run . day_1 part_1 day_1/input.txt")
		os.Exit(1)
	}

	dayArg := args[0]
	partArg := args[1]
	inputPath := args[2]

	// Extract day and part numbers and check they are actually numbers
	dayStr := strings.TrimPrefix(dayArg, "day_")
	partStr := strings.TrimPrefix(partArg, "part_")

	dayNum, err := strconv.Atoi(dayStr)
	if err != nil {
		fmt.Printf("Invalid day format: %s. Expected format: day_X\n", dayArg)
		os.Exit(1)
	}

	partNum, err := strconv.Atoi(partStr)
	if err != nil {
		fmt.Printf("Invalid part format: %s. Expected format: part_Y\n", partArg)
		os.Exit(1)
	}

	switch dayNum {
	case 1:
		runDay1(partNum, inputPath)
	default:
		fmt.Printf("Day %d not implemented yet\n", dayNum)
		os.Exit(1)
	}

}

func runDay1(part int, inputPath string) {
	/* Run the specified part of day 1 */
	switch part {
	case 1:
		result, err := day1.Part1(inputPath)
		if err != nil {
			fmt.Printf("Error: %v\n", err)
			os.Exit(1)
		}
		fmt.Println(result)
	case 2:
		results, err := day1.Part2(inputPath)
		if err != nil {
			fmt.Printf("Error: %v\n", err)
			os.Exit(1)
		}
		fmt.Println(results)
	}
}
