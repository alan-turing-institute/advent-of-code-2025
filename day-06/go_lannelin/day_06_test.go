package main

import (
	"testing"
)

const (
	inp = `123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  `
)

func TestPart1(t *testing.T) {
	want := 4277556
	got := Part1(inp)
	if got != want {
		t.Errorf("part1(%q) = %d, want %d", inp, got, want)
	}

}

func TestPart2(t *testing.T) {
	want := 3263827
	got := Part2(inp)
	if got != want {
		t.Errorf("part2(%q) = %d, want %d", inp, got, want)
	}

}
