package main

import (
	"testing"
)

const (
	inp = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"
)

// TestHelloEmpty calls greetings.Hello with an empty string,
// checking for an error.
func TestPart1(t *testing.T) {
	want := 1227775554
	got := Part1(inp)
	if got != want {
		t.Errorf("part1(%q) = %d, want %d", inp, got, want)
	}

}

func TestPart2(t *testing.T) {
	want := 4174379265
	got := Part2(inp)
	if got != want {
		t.Errorf("part2(%q) = %d, want %d", inp, got, want)
	}

}
