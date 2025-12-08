use std::{
    collections::HashMap,
    fs::File,
    io::{BufRead, BufReader},
};

const START: char = 'S';
const SPLIT: char = '^';

fn main() {
    let filename = "data.txt";

    // Part 1
    let data = parse(filename);
    let cols = data[0].len();
    let result = (0..data.len())
        .into_iter()
        .flat_map(move |r| (0..cols).into_iter().map(move |c| (r, c)))
        .filter(|(r, c)| data[*r][*c] == SPLIT)
        .filter(|(r, c)| is_beam(r - 1, *c, &data))
        .count();
    println!("Part 1: {result}");

    // Part 2
    let mut mem: HashMap<usize, HashMap<usize, u64>> = HashMap::new();
    let result: u64 = (0..data[0].len())
        .into_iter()
        .filter(|c| data[0][*c] == START)
        .map(|c| step(1, c, &data, &mut mem))
        .sum();
    println!("Part 2: {result}");
}

fn step(
    r: usize,
    c: usize,
    data: &Vec<Vec<char>>,
    mem: &mut HashMap<usize, HashMap<usize, u64>>,
) -> u64 {
    if let Some(inner) = mem.get(&r) {
        if let Some(x) = inner.get(&c) {
            return *x;
        }
    } else {
        mem.insert(r, HashMap::new());
    }
    if r == data.len() - 1 {
        return 1;
    }
    if data[r + 1][c] != SPLIT {
        let ret = step(r + 1, c, data, mem);
        mem.get_mut(&r).unwrap().insert(c, ret);
        return ret;
    }
    // No splitters on the edge.
    let ret = step(r + 1, c - 1, data, mem) + step(r + 1, c + 1, data, mem);
    mem.get_mut(&r).unwrap().insert(c, ret);
    ret
}

fn is_beam(r: usize, c: usize, data: &Vec<Vec<char>>) -> bool {
    if r == 0 {
        return data[r][c] == START;
    }
    if data[r][c] != SPLIT && is_beam(r - 1, c, data) {
        return true;
    }
    if c >= 1 && data[r][c - 1] == SPLIT && is_beam(r - 1, c - 1, data) {
        return true;
    }
    if c + 1 <= data[0].len() && data[r][c + 1] == SPLIT && is_beam(r - 1, c + 1, data) {
        return true;
    }
    false
}

fn parse(filename: &str) -> Vec<Vec<char>> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader
        .lines()
        .map(|s| s.unwrap().chars().collect())
        .collect()
}
