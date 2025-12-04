package day4

import (
	"advent-of-code-2025/utils"
	"fmt"
)

type matrix struct {
	rows int
	cols int
	data []int
}

func newMatrix(rows, cols int) *matrix {
	return &matrix{
		rows: rows,
		cols: cols,
		data: make([]int, rows*cols),
	}
}

func (m *matrix) At(row, col int) int {
	return m.data[row*m.cols+col]
}

func (m *matrix) Set(row, col, value int) {
	m.data[row*m.cols+col] = value
}

func Part1(inputPath string) (interface{}, error) {
	/*
		Day four part 1 solution:
	*/
	// Read input
	input, err := utils.ReadInputFile(inputPath)
	if err != nil {
		return "", err
	}
	fmt.Printf("Successfully read input\n")

	// Process input into matrix
	inputMatrix := newMatrix(len(input), len(input[0]))
	for r, line := range input {
		for c, char := range line {
			switch char {
			case '.':
				inputMatrix.Set(r, c, 0)
			case '@':
				inputMatrix.Set(r, c, 1)
			}
		}
	}

	rolls := 0
	for r := 0; r < inputMatrix.rows; r++ {
		for c := 0; c < inputMatrix.cols; c++ {
			if inputMatrix.At(r, c) == 1 {
				subM := extractSubMatrix(inputMatrix, r, c, 1)
				sum := 0
				for _, d := range subM.data {
					sum += d
				}
				if sum-1 < 4 {
					rolls++
				}
			}
		}
	}

	return rolls, nil
}

func extractSubMatrix(m *matrix, rowIdx int, colIdx int, size int) *matrix {
	// Catch exceptions for edges and corners
	switch rowIdx {
	case 0:
		switch colIdx {
		case 0:
			subM := newMatrix(size+1, size+1)
			for rIdx := 0; rIdx <= size; rIdx++ {
				for cIdx := 0; cIdx <= size; cIdx++ {
					subM.Set(rIdx, cIdx, m.At(rIdx, cIdx))
				}
			}
			return subM
		case m.cols - 1:
			subM := newMatrix(size+1, size+1)
			for rIdx := 0; rIdx <= size; rIdx++ {
				for cIdx := colIdx - size; cIdx <= colIdx; cIdx++ {
					subM.Set(rIdx, cIdx-(colIdx-size), m.At(rIdx, cIdx))
				}
			}
			return subM
		default:
			subM := newMatrix(size+1, size*2+1)
			for rIdx := 0; rIdx <= size; rIdx++ {
				for cIdx := colIdx - size; cIdx <= colIdx+size; cIdx++ {
					subM.Set(rIdx, cIdx-(colIdx-size), m.At(rIdx, cIdx))
				}
			}
			return subM
		}
	case m.rows - 1:
		switch colIdx {
		case 0:
			subM := newMatrix(size+1, size+1)
			for rIdx := rowIdx - size; rIdx <= rowIdx; rIdx++ {
				for cIdx := 0; cIdx <= size; cIdx++ {
					subM.Set(rIdx-(rowIdx-size), cIdx, m.At(rIdx, cIdx))
				}
			}
			return subM
		case m.cols - 1:
			subM := newMatrix(size+1, size+1)
			for rIdx := rowIdx - size; rIdx <= rowIdx; rIdx++ {
				for cIdx := colIdx - size; cIdx <= colIdx; cIdx++ {
					subM.Set(rIdx-(rowIdx-size), cIdx-(colIdx-size), m.At(rIdx, cIdx))
				}
			}
			return subM
		default:
			subM := newMatrix(size+1, size*2+1)
			for rIdx := rowIdx - size; rIdx <= rowIdx; rIdx++ {
				for cIdx := colIdx - size; cIdx <= colIdx+size; cIdx++ {
					subM.Set(rIdx-(rowIdx-size), cIdx-(colIdx-size), m.At(rIdx, cIdx))
				}
			}
			return subM
		}
	default:
		switch colIdx {
		case 0:
			subM := newMatrix(size*2+1, size+1)
			for rIdx := rowIdx - size; rIdx <= rowIdx+size; rIdx++ {
				for cIdx := 0; cIdx <= size; cIdx++ {
					subM.Set(rIdx-(rowIdx-size), cIdx, m.At(rIdx, cIdx))
				}
			}
			return subM
		case m.cols - 1:
			subM := newMatrix(size*2+1, size+1)
			for rIdx := rowIdx - size; rIdx <= rowIdx+size; rIdx++ {
				for cIdx := colIdx - size; cIdx <= colIdx; cIdx++ {
					subM.Set(rIdx-(rowIdx-size), cIdx-(colIdx-size), m.At(rIdx, cIdx))
				}
			}
			return subM
		default:
			subM := newMatrix(size*2+1, size*2+1)
			for rIdx := rowIdx - size; rIdx <= rowIdx+size; rIdx++ {
				for cIdx := colIdx - size; cIdx <= colIdx+size; cIdx++ {
					subM.Set(rIdx-(rowIdx-size), cIdx-(colIdx-size), m.At(rIdx, cIdx))
				}
			}
			return subM
		}
	}
}

func Part2(inputPath string) (interface{}, error) {
	/*
		Day four part 2 solution:
	*/
	// Read input
	input, err := utils.ReadInputFile(inputPath)
	if err != nil {
		return "", err
	}
	fmt.Printf("Successfully read input\n")

	// Process input into matrix
	inputMatrix := newMatrix(len(input), len(input[0]))
	for r, line := range input {
		for c, char := range line {
			switch char {
			case '.':
				inputMatrix.Set(r, c, 0)
			case '@':
				inputMatrix.Set(r, c, 1)
			}
		}
	}

	totalRemoved := 0
	for {
		removedThisRound := 0
		for r := 0; r < inputMatrix.rows; r++ {
			for c := 0; c < inputMatrix.cols; c++ {
				if inputMatrix.At(r, c) == 1 {
					subM := extractSubMatrix(inputMatrix, r, c, 1)
					sum := 0
					for _, d := range subM.data {
						sum += d
					}
					if sum-1 < 4 {
						inputMatrix.Set(r, c, 0)
						removedThisRound++
					}
				}
			}
		}
		if removedThisRound == 0 {
			break
		}
		totalRemoved += removedThisRound

	}

	return totalRemoved, nil
}
