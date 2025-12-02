package day2

import (
	"advent-of-code-2025/utils"
	"fmt"
	"strconv"
	"strings"
)

func Part1(inputPath string) (interface{}, error) {
	/*
		Part 2 solution:

		We have a list of number ranges as XXXX-YYYY, and we need to find all the number within these ranges
		with repeated digits or groups of digits.

		For each range we need to work out the all the patters we need to check for, i.e. for a 4 digit number we
		have:

			- ABAB
			- AAAA

		We can then iterate through the range and check each number against these patterns, saving those that match.

		Then return the sum of the numbers that matched.
	*/

	// Read input
	input, err := utils.ReadInputFile(inputPath)
	if err != nil {
		fmt.Printf("error here")
		return nil, err
	}
	fmt.Printf("Successfully read input\n")

	numRanges := strings.Split(input[0], ",")
	var numPairs [][]int
	for _, numRange := range numRanges {
		bounds := strings.Split(numRange, "-")
		var pair []int
		b1, err := strconv.Atoi(bounds[0])
		if err != nil {
			return nil, err
		}
		b2, err := strconv.Atoi(bounds[1])
		if err != nil {
			return nil, err
		}
		pair = append(pair, b1)
		pair = append(pair, b2)
		numPairs = append(numPairs, pair)
	}

	var repeatedNums []int
	for _, pair := range numPairs {
		repeatedNums = append(repeatedNums, checkRepeatedDigits(pair[0], pair[1])...)
	}

	sum := 0
	for _, val := range repeatedNums {
		sum += val
	}

	return sum, nil
}

func checkRepeatedDigits(num1, num2 int) []int {
	/* Check if a numbers in a given range have repeated digits or groups of digits */

	var repeatedNums []int
	for i := num1; i <= num2; i++ {
		digitCount := len(strconv.Itoa(i))
		numStr := strconv.Itoa(i)
		if digitCount%2 == 0 {
			half := digitCount / 2
			firstHalf := numStr[:half]
			secondHalf := numStr[half:]
			if firstHalf == secondHalf {
				// fmt.Printf("Found repeated digits: %d\n", i)
				repeatedNums = append(repeatedNums, i)
			}
		}
	}

	return repeatedNums
}

func Part2(inputPath string) (interface{}, error) {
	/* Placeholder for day 2 part 2 solution */
	return nil, nil
}
