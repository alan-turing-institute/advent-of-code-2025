package main

import (
	"fmt"
	"log"
	"math"
	"os"
	"sort"
	"strings"
)

// wrap around sort.Interface to keep track of indices for argsort
type Slice struct {
	sort.Interface
	idx []int
}

// TODO make this a pointer receiver with *Slice to avoid many copy()s?
func (s Slice) Swap(i, j int) {
	s.Interface.Swap(i, j)
	s.idx[i], s.idx[j] = s.idx[j], s.idx[i]
}

func NewSlice(data sort.Interface) Slice {
	s := Slice{
		Interface: data,
		idx:       make([]int, data.Len()),
	}
	for i := range s.idx {
		s.idx[i] = i
	}
	return s
}

func readInput(filePath string) string {
	b, err := os.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}
	// return split by line
	return string(b)
}

func parseInput(input string) [][3]int {
	lines := strings.Split(input, "\n")
	boxes := make([][3]int, len(lines))
	for i, line := range lines {
		var x, y, z int
		fmt.Sscanf(line, "%d,%d,%d", &x, &y, &z)
		boxes[i] = [3]int{x, y, z}
	}
	return boxes
}

func euclideanDistance(a []int, b []int) float64 {
	if len(a) != 3 || len(b) != 3 {
		log.Fatal("Both points must be same length")
	}
	distance := 0
	for i := 0; i < len(a); i++ {
		distance += (a[i] - b[i]) * (a[i] - b[i])
	}
	return math.Sqrt(float64(distance))

}

func multiplyInts(ints []int) int {
	result := 1
	for _, v := range ints {
		result *= v
	}
	return result
}

func ijToIndex(i, j, n int) int {
	// only using upper right triangle
	// so for n=5 (5x5)
	// we have only 10 inds (staring 0)
	// -1  0  1  2  3
	// -1 -1  4  5  6
	// -1 -1 -1  7  8
	// -1 -1 -1 -1  9
	// -1 -1 -1 -1 -1
	//e.g. for n=5 (1,2) -> 5 and (2,4) -> 8 and (3,4) -> 9
	if i >= j {
		log.Fatal("ijToIndex only works for upper right triangle where i < j")
	}
	return i*n + j - ((i + 2) * (i + 1) / 2)
}
func indexToIJ(index, n int) (int, int) {
	// reverse of ijToIndex - TODO improve
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			if ijToIndex(i, j, n) == index {
				return i, j
			}
		}
	}
	panic("index not found")
}

func dfs(node int, vertices map[int][]int, component map[int]bool) {
	if _, ok := component[node]; ok {
		return
	}
	component[node] = true
	children, ok := vertices[node]
	if ok {
		for _, child := range children {
			dfs(child, vertices, component)
		}
	}
}

func calcComponentSizes(nNodes int, vertices map[int][]int) []int {
	visited := make(map[int]bool)
	componentSizes := make([]int, 0)
	for startNode := range nNodes {
		if _, ok := visited[startNode]; ok {
			continue
		}
		component := map[int]bool{}
		dfs(startNode, vertices, component)
		for node := range component {
			visited[node] = true
		}
		componentSizes = append(componentSizes, len(component))

	}
	return componentSizes
}

func Part1(input string, topN int) int {
	boxes := parseInput(input)
	// we don't know length of boxes at compile so use a slice
	// only need upper right triangle of distances matrix
	distances := make([]float64, (len(boxes)*len(boxes)-len(boxes))/2)

	//distances is a 2D array stored in 1D slice
	// element (i,j) is at index i*len(boxes) + j
	for i := 0; i < len(boxes); i++ {
		// fill upper right triangle
		for j := i + 1; j < len(boxes); j++ {
			distances[ijToIndex(i, j, len(boxes))] = euclideanDistance(boxes[i][:], boxes[j][:])
			// ignore left lower triangle, leave them as 0
		}
	}

	// wrap distances in `Slice` to track indices
	distancesSlice := NewSlice(sort.Float64Slice(distances))
	// sort distances
	sort.Sort(distancesSlice)

	topNSmallestInds := distancesSlice.idx[:topN]

	// build graph from topN smallest distances
	vertices := map[int][]int{}
	for node := range len(boxes) {
		vertices[node] = []int{}
	}
	for _, index := range topNSmallestInds {
		i, j := indexToIJ(index, len(boxes))
		vertices[i] = append(vertices[i], j)
		vertices[j] = append(vertices[j], i)

	}
	componentSizes := calcComponentSizes(len(boxes), vertices)

	// sort sizes
	sort.Ints(componentSizes)
	// mulitply top 3 sizes
	return multiplyInts(componentSizes[len(componentSizes)-3:])

}

func Part2(input string) int {
	boxes := parseInput(input)
	// we don't know length of boxes at compile so use a slice
	// only need upper right triangle of distances matrix
	distances := make([]float64, (len(boxes)*len(boxes)-len(boxes))/2)

	//distances is a 2D array stored in 1D slice
	// element (i,j) is at index i*len(boxes) + j
	for i := 0; i < len(boxes); i++ {
		// fill upper right triangle
		for j := i + 1; j < len(boxes); j++ {
			distances[ijToIndex(i, j, len(boxes))] = euclideanDistance(boxes[i][:], boxes[j][:])
			// ignore left lower triangle, leave them as 0
		}
	}

	// wrap distances in `Slice` to track indices
	distancesSlice := NewSlice(sort.Float64Slice(distances))
	// sort distances
	sort.Sort(distancesSlice)

	sortedDistanceInds := distancesSlice.idx

	vertices := map[int][]int{}
	for node := range len(boxes) {
		vertices[node] = []int{}
	}

	for _, ind := range sortedDistanceInds {
		i, j := indexToIJ(ind, len(boxes))
		vertices[i] = append(vertices[i], j)
		vertices[j] = append(vertices[j], i)

		// to DFS
		componentSizes := calcComponentSizes(len(boxes), vertices)
		sort.Ints(componentSizes)
		if componentSizes[len(componentSizes)-1] == len(boxes) {
			xi := boxes[i][0]
			xj := boxes[j][0]
			return xi * xj
		}

	}

	panic("no solution found")

}

func main() {
	input := readInput("input.txt")
	part1Ans := Part1(input, 1000)
	fmt.Printf("Part 1: %d\n", part1Ans)

	part2Ans := Part2(input)
	fmt.Printf("Part 2: %d\n", part2Ans)
}
