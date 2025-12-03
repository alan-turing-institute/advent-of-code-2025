use std::{
    fs::File,
    io::{BufRead, BufReader},
    iter,
};

fn main() {
    let filename = "data.txt";

    // Part 1
    let passwd = parse(filename)
        .scan(50, |acc, x| {
            *acc = (*acc + x) % 100;
            Some(*acc)
        })
        .filter(|d| d == &0)
        .count();

    println!("Part 1: {passwd}");

    // Part 2
    let rotations: Vec<i32> = parse(filename)
        .map(|n| {
            if n.abs() > 100 {
                let count = usize::try_from(n.abs() / 100).expect("abs value used");
                return iter::repeat(0)
                    .take(count)
                    .chain(iter::once(n % 100))
                    .collect::<Vec<_>>()
                    .into_iter();
            }
            iter::once(n).collect::<Vec<_>>().into_iter()
        })
        .flatten()
        .collect();

    let positions = iter::once(50)
        .chain(rotations.clone())
        .scan(0, |acc, x| {
            *acc = (*acc + x) % 100;
            Some(*acc)
        })
        .collect::<Vec<_>>();

    let passwd = positions
        .windows(2)
        .zip(rotations)
        .filter(|(w, r)| {
            w[1] == 0
                || w[0] == w[1]
                || (w[0] > 0 && w[1] < 0)
                || (w[0] < 0 && w[1] > 0)
                || (w[1] < w[0] && *r > 0)
                || (w[0] < w[1] && *r < 0)
        })
        .count();

    println!("Part 2: {passwd}");
}

fn parse(filename: &str) -> impl Iterator<Item = i32> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader
        .lines()
        .map(|line| line.unwrap().replace("L", "-").replace("R", "+"))
        .map(|s| s.parse::<i32>().unwrap())
}
