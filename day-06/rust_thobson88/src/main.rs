use std::{
    fs::File,
    io::{BufRead, BufReader},
};

fn main() {
    let filename = "data.txt";

    let operators = parse_operators(filename);

    // Part 1
    let figures = parse_figures(filename);
    let result: u64 = fold_all(figures, operators.clone());
    println!("Part 1: {result}");

    // Part 2
    let figures = parse_ceph(filename);
    let result: u64 = fold_all(figures, operators);
    println!("Part 2: {result}");
}

fn fold_all(figures: Vec<Vec<u64>>, operators: Vec<bool>) -> u64 {
    figures
        .clone()
        .into_iter()
        .zip(operators.iter())
        .fold(0, |mut acc, x| {
            if *x.1 {
                acc += x.0.into_iter().reduce(|a, b| a * b).unwrap()
            } else {
                acc += x.0.into_iter().reduce(|a, b| a + b).unwrap();
            }
            acc
        })
}

fn figure_lines(filename: &str) -> Vec<String> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    let lines: Vec<String> = reader.lines().map(|s| s.unwrap()).collect();
    lines.into_iter().rev().skip(1).rev().collect()
}

fn parse_figures(filename: &str) -> Vec<Vec<u64>> {
    transpose(
        figure_lines(filename)
            .iter()
            .map(|s| {
                s.split_whitespace()
                    .into_iter()
                    .map(|x| x.parse::<u64>().unwrap())
                    .collect()
            })
            .collect(),
    )
}

fn parse_ceph(filename: &str) -> Vec<Vec<u64>> {
    // Find the empty columns.
    let is_empty: Vec<Vec<bool>> = figure_lines(filename)
        .iter()
        .map(|s| {
            s.chars()
                .into_iter()
                .map(|c| c.to_string() == " ")
                .collect()
        })
        .collect();
    let is_empty: Vec<bool> = is_empty.iter().skip(1).fold(is_empty[0].clone(), |acc, x| {
        acc.iter().zip(x).map(|(a, b)| *a && *b).collect()
    });
    let blocks: Vec<Vec<String>> = figure_lines(filename)
        .iter()
        .map(|s| chunks(s, is_empty.clone()))
        .collect();

    let blocks: Vec<Vec<String>> = transpose(blocks)
        .iter()
        .map(|b| {
            (0..b[0].len())
                .into_iter()
                .map(|i| {
                    b.iter()
                        .map(move |s| s.to_string().chars().nth(i).unwrap())
                        .collect()
                })
                .collect()
        })
        .collect();

    blocks
        .iter()
        .map(|b| b.iter().map(|s| s.trim().parse::<u64>().unwrap()).collect())
        .collect()
}

fn chunks(s: &str, split: Vec<bool>) -> Vec<String> {
    let split_at: Vec<_> = (0..split.len()).into_iter().filter(|i| split[*i]).collect();
    let mut ret = vec![s[..split_at[0]].to_string()];
    for w in split_at.iter().collect::<Vec<_>>().windows(2) {
        ret.push(s[(w[0] + 1)..*w[1]].to_string());
    }
    ret.push(s[(split_at.last().unwrap() + 1)..].to_string());
    ret
}

fn transpose<T: Clone>(blocks: Vec<Vec<T>>) -> Vec<Vec<T>> {
    (0..blocks[0].len())
        .map(|i| {
            blocks
                .iter()
                .map(|inner| inner[i].clone())
                .collect::<Vec<T>>()
        })
        .collect()
}

fn parse_operators(filename: &str) -> Vec<bool> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    let line = reader.lines().last().unwrap().unwrap();
    line.split_whitespace()
        .into_iter()
        .map(|s| s == "*")
        .collect()
}
