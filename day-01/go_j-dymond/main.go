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

func get_password_simple(rotations []string, starting_loc int) int{
	var position = starting_loc;
	var password = 0;

	for _,rotation := range rotations {
		var direction byte = rotation[0]
		magnitude, _ := strconv.Atoi(rotation[1:])
		if direction == 'L' {
			position -= magnitude
		}
		if direction == 'R' {
			position += magnitude
		}
		if position%100 == 0 {
			password += 1
		}
	}

	return password
}

func get_password_complex(rotations []string, starting_loc int) int{
	var new_rotations []string

	for _,rotation := range rotations {
		var direction = rotation[0]
		magnitude, _ := strconv.Atoi(rotation[1:])
		for i:=0; i<magnitude; i++ {
			new_rotations = append(new_rotations, string(direction)+"1")
		}
	}
	return get_password_simple(new_rotations, starting_loc)
}

func main() {
	rotations := LoadInput("day-01/go_j-dymond/data.txt", "\n")
	println("Part 2:", get_password_simple(rotations, 50))
	println("Part 2:", get_password_complex(rotations, 50))
}