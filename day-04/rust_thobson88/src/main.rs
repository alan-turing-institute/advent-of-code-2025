use std::{fs::File, io::BufRead, io::BufReader};

fn main() {
    let filename = "data.txt";

    // Part 1
    let data = parse(filename);
    let (rows, cols) = (data.len(), data[0].len());
    let result = (0..cols)
        .into_iter()
        .flat_map(move |c| (0..rows).into_iter().map(move |r| (c, r)))
        .filter(|(c, r)| data[*r][*c])
        .map(|(c, r)| count_neighbours(&data, c, r, rows, cols))
        .filter(|count| *count < 4usize)
        .count();
    println!("Part 1: {result}");

    // Part 2
    let mut result = 0;
    let mut data = parse(filename);
    loop {
        let remove: Vec<(usize, usize)> = (0..cols)
            .into_iter()
            .flat_map(move |c| (0..rows).into_iter().map(move |r| (c, r)))
            .filter(|(c, r)| data[*r][*c])
            .filter(|(c, r)| count_neighbours(&data, *c, *r, rows, cols) < 4usize)
            .collect();
        let count = remove
            .iter()
            .map(|(c, r)| count_neighbours(&data, *c, *r, rows, cols))
            .filter(|count| *count < 4usize)
            .count();
        for (i, j) in remove {
            data[j][i] = false;
        }
        result += count;
        if count == 0 {
            break;
        }
    }
    println!("Part 2: {result}");
}

fn count_neighbours(data: &Vec<Vec<bool>>, c: usize, r: usize, rows: usize, cols: usize) -> usize {
    neighbours(c, r, rows, cols)
        .into_iter()
        .map(|(c, r)| data[r][c])
        .filter(|b| *b)
        .count()
}

fn neighbours(c: usize, r: usize, rows: usize, cols: usize) -> Vec<(usize, usize)> {
    adj(c, cols)
        .flat_map(|x| adj(r, rows).map(move |y| (x, y)))
        .filter(|(x, y)| (*x, *y) != (c, r))
        .collect()
}

// Do the usize::try_from one index at a time so we can use flat_map.
fn adj(i: usize, lim: usize) -> impl Iterator<Item = usize> {
    (-1..2)
        .into_iter()
        .map(move |x| x + i as isize)
        .flat_map(|x| usize::try_from(x))
        .filter(move |x| *x < lim)
}

fn parse(filename: &str) -> Vec<Vec<bool>> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader
        .lines()
        .map(|s| s.unwrap().chars().map(|c| c == '@').collect())
        .collect()
}
