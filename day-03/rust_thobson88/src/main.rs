use std::{
    fs::File,
    io::{BufRead, BufReader},
};

fn main() {
    let filename = "data.txt";

    // Part 1
    let result: u32 = parse(filename).map(best_joltage).sum();
    println!("Part 1: {result}");

    // Part 2
    let result: u64 = parse(filename).map(best_joltage2).sum();
    println!("Part 2: {result}");
}

fn best_joltage(s: String) -> u32 {
    best_two_indices(&s)
        .iter()
        .fold(String::new(), |buf, x| {
            buf + &s.chars().nth(*x).unwrap().to_string()
        })
        .parse()
        .unwrap()
}

fn best_two_indices(s: &str) -> [usize; 2] {
    let a = fhd(s);
    if a == s.len() - 1 {
        return [fhd(&s[..a]), a];
    }
    [a, a + 1 + fhd(&s[(a + 1)..])]
}

fn best_joltage2(s: String) -> u64 {
    best_twelve_indices(&s)
        .iter()
        .fold(String::new(), |buf, x| {
            buf + &s.chars().nth(*x).unwrap().to_string()
        })
        .parse()
        .unwrap()
}

fn best_twelve_indices(s: &str) -> [usize; 12] {
    let mut ret: Vec<usize> = vec![];
    let mut i = best_index(s, 12);
    let mut acc = i;
    for n in (1..12).rev() {
        ret.push(acc);
        i = best_index(&s[(acc + 1)..], n);
        acc += i + 1;
    }
    ret.push(acc);
    ret.try_into().unwrap()
}

// The best index, given that `n` more digits must be chosen.
fn best_index(s: &str, n: usize) -> usize {
    let a = fhd(s);
    if a > s.len() - n {
        return best_index(&s[..a], n - (s.len() - a));
    }
    a
}

// Index of the first highest digit.
fn fhd(s: &str) -> usize {
    let digits: Vec<u32> = s
        .chars()
        .into_iter()
        .map(|c| c.to_digit(10).unwrap())
        .collect();
    let max = digits.iter().max().unwrap();
    digits.iter().position(|d| d == max).unwrap()
}

fn parse(filename: &str) -> impl Iterator<Item = String> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader.lines().map(|l| l.unwrap())
}
