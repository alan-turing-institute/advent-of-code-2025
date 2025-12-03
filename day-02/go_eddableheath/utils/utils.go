package utils

import (
	"bufio"
	"log"
	"os"
)

func ReadInputFile(path string) ([]string, error) {
	/* Read the input file and return its lines as a slice of strings */

	file, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	lines := []string{}
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	if err := scanner.Err(); err != nil {
		return nil, err
	}

	return lines, nil
}
