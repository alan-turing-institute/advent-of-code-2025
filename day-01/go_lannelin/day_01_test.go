package main

import (
	"testing"
)

const (
	inp = `L68
L30
R48
L5
R60
L55
L1
L99
R14
L82`
)

// TestHelloEmpty calls greetings.Hello with an empty string,
// checking for an error.
func TestPart1(t *testing.T) {
	want := 3
	got := Part1(inp)
	if got != want {
		t.Errorf("part1(%q) = %d, want %d", inp, got, want)
	}

}

func TestPart2(t *testing.T) {
	want := 6
	got := Part2(inp)
	if got != want {
		t.Errorf("part2(%q) = %d, want %d", inp, got, want)
	}

}
