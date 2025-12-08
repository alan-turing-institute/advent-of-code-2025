package main

import (
	"fmt"
	"log"
	"math"
	"os"
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

func parseInput(input string) [][]uint8 {
	//split by line
	banks := strings.Split(input, "\n")
	intBanks := make([][]uint8, len(banks))
	for i, bank := range banks {
		intBanks[i] = make([]uint8, len(bank))
		for j, char := range bank {
			num, err := strconv.Atoi(string(char))
			if err != nil {
				fmt.Printf("couldn't parse bank item to int: %s", string(char))
				panic(err)
			}
			intBanks[i][j] = uint8(num)
		}
	}
	return intBanks
}

func argMaxUInt8(seq []uint8) int {
	if len(seq) == 0 {
		panic("cannot get max of empty sequence")
	}
	max := uint8(0)
	argMax := 0
	for i, val := range seq {
		if val > max {
			max = val
			argMax = i
		}
	}
	return argMax

}

func maxJoltage(bank []uint8, nBatteries int) int {

	if len(bank) < nBatteries {
		panic(fmt.Sprintf("not enough batteries in bank. Wanted %d, got %d", nBatteries, len(bank)))
	}

	if nBatteries == 1 {
		m := bank[argMaxUInt8(bank)]
		return int(m)
	}

	leftCandidates := bank[:len(bank)-(nBatteries-1)]
	leftInd := argMaxUInt8(leftCandidates)
	leftmostValue := int(bank[leftInd]) * int(math.Pow(10.0, float64(nBatteries-1)))
	return leftmostValue + maxJoltage(bank[leftInd+1:], nBatteries-1)

}

func totalJoltage(input string, nBatteries int) int {
	totalJoltage := 0
	for _, bank := range parseInput(input) {
		totalJoltage += maxJoltage(bank, nBatteries)
	}
	return totalJoltage
}

func Part1(input string) int {
	return totalJoltage(input, 2)
}

func Part2(input string) int {
	return totalJoltage(input, 12)

}

func main() {
	input := readInput("input.txt")
	part1Ans := Part1(input)
	fmt.Printf("Part 1: %d\n", part1Ans)

	part2Ans := Part2(input)
	fmt.Printf("Part 2: %d\n", part2Ans)
}
