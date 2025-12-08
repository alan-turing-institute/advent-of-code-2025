package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func readInput(filePath string) string {
	b, err := os.ReadFile(filePath) // just pass the file name
	if err != nil {
		log.Fatal(err)
	}
	return string(b)
}

func makeRange(start, end int) []int {
	r := make([]int, end-start+1)
	for i := range r {
		r[i] = start + i
	}
	return r
}

func expandRange(r string) []int {
	parts := strings.Split(r, "-")
	if len(parts) != 2 {
		fmt.Printf("couldn't parse range: %s", r)
		panic("invalid range")
	}
	start, err := strconv.Atoi(parts[0])
	if err != nil {
		fmt.Printf("couldn't parse start of range: %s", parts[0])
		panic(err)
	}
	end, err := strconv.Atoi(parts[1])
	if err != nil {
		fmt.Printf("couldn't parse end of range: %s", parts[1])
		panic(err)
	}

	return makeRange(start, end)
}

func parseInput(input string) []int {
	//split by comma
	stringRanges := strings.Split(input, ",")
	//expand each range
	ranges := make([][]int, len(stringRanges))
	for i, stringRange := range stringRanges {
		ranges[i] = expandRange(stringRange)
	}
	//flatten
	flat := []int{}
	for _, r := range ranges {
		flat = append(flat, r...)
	}
	return flat
}

func chunkString(s string, size int) []string {
	var chunks []string
	for i := 0; i < len(s); i += size {
		chunks = append(chunks, s[i:i+size])
	}
	return chunks
}

func chunksAllMatch(chunks []string) bool {
	if len(chunks) == 0 {
		return false
	}
	first := chunks[0]
	for _, chunk := range chunks[1:] {
		if chunk != first {
			return false
		}
	}
	return true
}

func Part1(input string) int {
	total := 0
	for _, productID := range parseInput(input) {
		s := strconv.Itoa(productID)
		if len(s)%2 != 0 {
			continue
		}
		mid := len(s) / 2
		if chunksAllMatch(chunkString(s, mid)) {
			total += productID
		}
	}
	return total

}

func Part2(input string) int {
	total := 0
	for _, productID := range parseInput(input) {
		s := strconv.Itoa(productID)
		l := len(s)
		mid := l / 2
		for i := 1; i <= mid; i++ {
			// check divisibility then chunk match
			if (l%i == 0) && chunksAllMatch(chunkString(s, i)) {
				total += productID
				break
			}
		}
	}
	return total

}

func main() {
	input := readInput("input.txt")
	part1Ans := Part1(input)
	fmt.Printf("Part 1: %d\n", part1Ans)

	part2Ans := Part2(input)
	fmt.Printf("Part 2: %d\n", part2Ans)
}
