package main

import (
	"os"
	"strconv"
	"strings"
)

func LoadInput(path string, split_str string) []string {
	content, err := os.ReadFile(path)
	if err != nil {
		panic(err)
	}
	lines := strings.Split(string(content), split_str)
	return lines
}

func ArgMax(nums []int) int {
    maxIdx := 0
    for i, v := range nums {
        if v > nums[maxIdx] {
            maxIdx = i
        }
    }
    return maxIdx
}

func get_largest_digit_sequence(sequence []int, n int) int{
	var largest = []string{}
	var max_val int = 0
	var max_index int = 0
	var start int = 0
	for i := 1 ; i <= n && len(sequence) > 0; i++ {
		end := len(sequence) - (n - i)
        searchSlice := sequence[start:end]
		max_index = ArgMax(searchSlice)
		max_val = sequence[max_index]
		largest = append(largest, strconv.Itoa(max_val))
		sequence = sequence[max_index+1:]
	}
	joltageStr := strings.Join(largest, "")
    joltage, _ := strconv.Atoi(joltageStr)
    return joltage
}

func run (batteries []string, n_values int) int {
	var total_joltage int = 0
	for _, battery := range batteries {
			battery_values := strings.Split(battery, "")
			battery_ints := make([]int, len(battery_values))
			for i, v := range battery_values {
				battery_ints[i], _ = strconv.Atoi(v)
			}
			total_joltage += get_largest_digit_sequence(battery_ints, n_values)
		}
	return total_joltage
}

func main() {
	var batteries []string = LoadInput("day-03/go_j-dymond/data.txt", "\n")
	println("Part 1:", run(batteries, 2))
	println("Part 2:", run(batteries, 12))
}