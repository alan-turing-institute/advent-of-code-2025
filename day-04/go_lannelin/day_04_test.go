package main

import (
	"testing"
)

const (
	inp = `..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@.`
)

func TestPart1(t *testing.T) {
	want := 13
	got := Part1(inp)
	if got != want {
		t.Errorf("part1(%q) = %d, want %d", inp, got, want)
	}

}

func TestPart2(t *testing.T) {
	want := 43
	got := Part2(inp)
	if got != want {
		t.Errorf("part2(%q) = %d, want %d", inp, got, want)
	}

}
