package main

import (
	"os"
	"strconv"
	"strings"
)


func LoadInput(path string, split_str string) []string {
	content, err := os.ReadFile(path)
	if err != nil {
		//Do something
		panic(err)
	}
	lines := strings.Split(string(content), split_str)
	return lines
}

func simple_repeat_checker(number int, repeat_count int) bool {
	numStr := strconv.Itoa(number)
	if len(numStr)%repeat_count != 0 {
		return false
	}
	mid := len(numStr) / repeat_count
	sub := numStr[:mid]
	repeated := strings.Repeat(sub, repeat_count)
    return repeated == numStr
}


func get_repeated_digit_sequence(digit_range string, part_2 bool) int {
	var value int = 0
	var repeated bool = false

	parts := strings.Split(digit_range, "-")
	start,_ := strconv.Atoi(parts[0])
	end,_ := strconv.Atoi(parts[1])
	if part_2 {
		for i := start; i <= end; i++ {
			for length := 2; length <= len([]rune(strconv.Itoa(i))); length++ {
				repeated = simple_repeat_checker(i,length)
				if repeated {
					value += i
					break
				}
		}
		}
	} else {
		for i := start; i <= end; i++ {
			repeated = simple_repeat_checker(i,2)
			if repeated {
				value += i
			}
		}
	}
	return value
}



func main(){
	var digit_ranges []string = LoadInput("day-02/go_j-dymond/data.txt", ",")
	var invalid_ID_sum int = 0

	for _, digit_range := range digit_ranges {
	 	invalid_ID_sum += get_repeated_digit_sequence(digit_range, false)
	}
	println("Part 1:", invalid_ID_sum)

	invalid_ID_sum = 0
	for _, digit_range := range digit_ranges {
	 	invalid_ID_sum += get_repeated_digit_sequence(digit_range, true)
	}
	println("Part 2:", invalid_ID_sum)

}