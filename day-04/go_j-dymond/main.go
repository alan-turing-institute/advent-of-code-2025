package main

import (
	"os"
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

func generate_grid(sequences []string) [][]rune{
	var grid [][]rune
	for _, sequence := range sequences {
		row := []rune(sequence)
		grid = append(grid, row)
	}
	return grid
}

func get_cell_value(x int, y int, grid [][]rune) rune {
	if x < 0 || y < 0 || x >= len(grid) || y >= len(grid[0]) {
		return '.'
	}
	return grid[x][y]
}

func find_adjacent_cells(x int, y int, grid [][]rune) int {
	var cells int = 0
	var cell_value rune
	directions := [][]int{
		{-1, 0}, // up
		{1, 0},  // down
		{0, -1}, // left
		{0, 1},  // right
		{-1, -1}, // diagonal left up
		{1, 1},  // diagonal right up
		{1, -1}, // diagonal left down
		{-1, 1},  // diagonal right down
	}
	for _, dir := range directions {
		newX := x + dir[0]
		newY := y + dir[1]
		cell_value = get_cell_value(newX, newY, grid)
		if cell_value == '@'{
			cells += 1
		}
	}
	return cells
}

func run(sequences []string, part_2 bool) int {
	var total_removed int = 0
	var none_found bool = false
	grid := generate_grid(sequences)
	next_grid := generate_grid(sequences)
	for !none_found {
		var accessible int = 0
		for row_index, row := range grid {
			for column_index, cell := range row {
				cell = grid[row_index][column_index]
				if cell == '@'{
					adjacent_cells := find_adjacent_cells(row_index, column_index, grid)
					if adjacent_cells < 4 {
						accessible += 1
						next_grid[row_index][column_index] = '.'
					}
				}
			}
		}

		total_removed += accessible
		if accessible == 0 || !part_2 {
			none_found = true
			break
		} else {
			grid = next_grid
		}
	}
	return total_removed
}

func main() {
	var sequences []string = LoadInput("day-04/go_j-dymond/data.txt", "\n")

	println("part1: ", run(sequences, false))
	println("part2: ", run(sequences, true))
}
