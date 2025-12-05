use std::{
    fs::File,
    io::{BufRead, BufReader},
};

fn main() {
    let filename = "data.txt";

    let ids = parse_ids(filename);
    let ranges = parse_ranges(filename);

    // Part 1
    let result = ids
        .iter()
        .filter(|id| ranges.iter().any(|range| in_range(**id, *range)))
        .count();
    println!("Part 1: {result}");

    // Part 2
    let mut ranges = parse_ranges(filename);
    ranges.sort_by(|a, b| {
        if a.0 == b.0 {
            return a.1.cmp(&b.1);
        }
        a.0.cmp(&b.0)
    });

    let result = ranges
        .iter()
        .fold((0, 0, 0), |mut acc, x| {
            if x.0 > acc.1 {
                // No overlap with preceding ranges.
                acc.2 += x.1 - (x.0 - 1);
            } else if x.1 > acc.1 {
                // Partial overlap.
                acc.2 += x.1 - acc.1;
            }
            (x.0.min(acc.0), x.1.max(acc.1), acc.2)
        })
        .2;
    println!("Part 2: {result}");
}

fn in_range(id: u64, range: (u64, u64)) -> bool {
    id >= range.0 && id <= range.1
}

fn parse_ids(filename: &str) -> Vec<u64> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader
        .lines()
        .map(|s| s.unwrap())
        .skip_while(|s| s != "")
        .flat_map(|s| s.parse::<u64>())
        .collect()
}

fn parse_ranges(filename: &str) -> Vec<(u64, u64)> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader
        .lines()
        .map(|s| s.unwrap())
        .take_while(|s| s != "")
        .map(|s| {
            s.split("-")
                .map(|x| x.parse::<u64>().unwrap())
                .collect::<Vec<u64>>()
        })
        .map(|v| (v[0], v[1]))
        .collect()
}
