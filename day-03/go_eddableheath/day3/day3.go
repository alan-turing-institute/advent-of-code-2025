package day3

import (
	"advent-of-code-2025/utils"
	"fmt"
	"strconv"
	"strings"
)

func Part1(inputPath string) (interface{}, error) {
	/*
		Day three part 1 solution:

		Each line has a series of digits, we need to identify the largest two digit number found
		in each line, and then sum these largest numbers from each line together to get the final result.

		First look for the largest single digit, that isn't the final digit, then find the subsequent largest
		digit.
	*/

	input, err := utils.ReadInputFile(inputPath)
	if err != nil {
		return nil, err
	}
	fmt.Printf("Successfully read input\n")

	sum := 0
	for _, line := range input {
		largestTwoDigitNumber, err := findLargestTwoDigitNumber(line)
		if err != nil {
			return nil, err
		}
		sum += largestTwoDigitNumber
	}

	return sum, nil
}

func findLargestTwoDigitNumber(line string) (int, error) {
	/*
		Find the largest two digit number in a string of digits.
	*/

	largestDigit := 0
	idxLargest := 0
	for i := 0; i < len(line)-1; i++ {
		digit, _ := strconv.Atoi(string(line[i]))
		if digit > largestDigit {
			largestDigit = digit
			idxLargest = i
		}
	}

	largestSecondDigit := 0
	for j := idxLargest + 1; j < len(line); j++ {
		digit, _ := strconv.Atoi(string(line[j]))
		if digit > largestSecondDigit {
			largestSecondDigit = digit
		}
	}

	twoDigitNumber, _ := strconv.Atoi(fmt.Sprintf("%d%d", largestDigit, largestSecondDigit))
	return twoDigitNumber, nil
}

func largest12DigitNumber(line string) (int, error) {
	/*
		Find the largest 12 digit number in a string of digits.
	*/

	largestLeadingDigit := 0
	idxLargest := 0
	for i := 0; i < len(line)-11; i++ {
		digit, _ := strconv.Atoi(string(line[i]))
		if digit > largestLeadingDigit {
			largestLeadingDigit = digit
			idxLargest = i
		}
	}

	if idxLargest+12 == len(line) {
		twelveDigitNumber, _ := strconv.Atoi(line[idxLargest:])
		return twelveDigitNumber, nil
	}

	var collectedDigits strings.Builder
	collectedDigits.WriteString(string(line[idxLargest]))
	// fmt.Printf("Initial collected digits %v\n", collectedDigits)
	digitsNeeded := 12
	currentLargestDigit := 0
	lastIdx := idxLargest
	for digitsNeeded > 1 {
		newLastIdx := lastIdx
		var searchDigits string
		if digitsNeeded-1 == 1 {
			searchDigits = line[lastIdx+1:]
		} else {
			searchDigits = line[lastIdx+1 : len(line)-(digitsNeeded-2)]
		}
		fmt.Printf("searchable digits with %v digits left: %v\n", digitsNeeded-1, searchDigits)
		currentLargestDigit = 0
		for j := 0; j < len(searchDigits); j++ {
			digit, _ := strconv.Atoi(string(searchDigits[j]))
			if digit > currentLargestDigit {
				currentLargestDigit = digit
				newLastIdx = j + lastIdx + 1
			}
		}
		collectedDigits.WriteString(string(line[newLastIdx]))
		digitsNeeded--
		lastIdx = newLastIdx
	}
	fmt.Printf("final number is number for line %v is %v\n", line, collectedDigits.String())

	collectedDigitsStr := collectedDigits.String()
	if len(collectedDigitsStr) != 12 {
		fmt.Printf("could not collect 12 digits from line %s, got %v digits\n", line, len(collectedDigitsStr))
	}

	return strconv.Atoi(collectedDigitsStr)
}

func Part2(inputPath string) (interface{}, error) {
	/*
		Day three part 2 solution:

		Now we have to do the same but for the largest 12 digit number in each line.
	*/

	input, err := utils.ReadInputFile(inputPath)
	if err != nil {
		return nil, err
	}
	fmt.Printf("Successfully read input\n")

	sum := 0
	for _, line := range input {
		num, _ := largest12DigitNumber(line)
		sum += num
	}

	return sum, nil
}
