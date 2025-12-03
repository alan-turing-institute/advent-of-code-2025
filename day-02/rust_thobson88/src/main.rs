use std::{
    fs::File,
    io::{BufRead, BufReader},
    ops::Range,
};

fn main() {
    let filename = "data.txt";

    // Part 1
    let result: u64 = parse(filename)
        .flat_map(parse_range)
        .filter(is_invalid)
        .sum();

    println!("Part 1: {result}");

    // Part 2
    let result: u64 = parse(filename)
        .flat_map(parse_range)
        .filter(is_invalid2)
        .sum();

    println!("Part 2: {result}");
}

fn is_invalid(n: &u64) -> bool {
    let s = n.to_string();
    let p = s.split_at(s.len() / 2);
    p.0 == p.1
}

fn is_invalid2(n: &u64) -> bool {
    let s = n.to_string();
    for l in 1..((s.len() / 2) + 1) {
        let chunks: Vec<String> = s
            .chars()
            .collect::<Vec<char>>()
            .chunks(l)
            .map(|c| c.into_iter().collect())
            .collect();
        if chunks.iter().all(|s| *s == chunks[0]) {
            return true;
        }
    }
    false
}

fn parse_range(s: String) -> Range<u64> {
    let r: Vec<u64> = s.split("-").map(|x| x.parse::<u64>().unwrap()).collect();
    r[0]..(r[1] + 1)
}

fn parse(filename: &str) -> impl Iterator<Item = String> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader.lines().map(|l| l.unwrap()).flat_map(|s| {
        s.split(",")
            .map(|st| st.to_string())
            .collect::<Vec<String>>()
            .into_iter()
    })
}
