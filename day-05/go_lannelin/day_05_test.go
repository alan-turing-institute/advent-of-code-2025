package main

import (
	"testing"
)

const (
	inp = `3-5
10-14
16-20
12-18

1
5
8
11
17
32`
)

func TestPart1(t *testing.T) {
	want := 3
	got := Part1(inp)
	if got != want {
		t.Errorf("part1(%q) = %d, want %d", inp, got, want)
	}

}

func TestPart2(t *testing.T) {
	want := 14
	got := Part2(inp)
	if got != want {
		t.Errorf("part2(%q) = %d, want %d", inp, got, want)
	}

}
