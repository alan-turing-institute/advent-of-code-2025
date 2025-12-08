package main

import (
	"testing"
)

const (
	inp = `987654321111111
811111111111119
234234234234278
818181911112111`
)

func TestPart1(t *testing.T) {
	want := 357
	got := Part1(inp)
	if got != want {
		t.Errorf("part1(%q) = %d, want %d", inp, got, want)
	}

}

func TestPart2(t *testing.T) {
	want := 3121910778619
	got := Part2(inp)
	if got != want {
		t.Errorf("part2(%q) = %d, want %d", inp, got, want)
	}

}
