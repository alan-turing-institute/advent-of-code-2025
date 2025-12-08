use std::{
    collections::HashSet,
    fs::File,
    hash::Hash,
    io::{BufRead, BufReader},
};

fn main() {
    let filename = "data.txt";

    // Part 1
    let points = parse(filename);
    let mut pairs = HashSet::new();
    for p1 in points.clone() {
        for p2 in points.clone() {
            if p1 != p2 {
                pairs.insert(Pair { p1, p2 });
            }
        }
    }

    let mut sorted = pairs.iter().collect::<Vec<_>>();
    sorted.sort_by(|a, b| f32::total_cmp(&a.sep(), &b.sep()));

    let mut n = 1000;
    if filename == "dummy_data.txt" {
        n = 10;
    }

    let mut circs = Box::new(vec![]);
    let mut it = sorted.clone().into_iter().take(n);
    while let Some(pair) = it.next() {
        circs = join(pair, circs);
    }
    // Sort descending.
    circs.sort_by(|a, b| b.len().cmp(&a.len()));
    let result: usize = circs.iter().take(3).map(|c| c.len()).product();

    println!("Part 1: {result}");

    // Part 2
    let mut it = sorted.into_iter().skip(n);
    loop {
        let pair = it.next().unwrap();
        circs = join(pair, circs);
        if circs.len() == 1 && circs[0].len() == points.len() {
            println!("Part 2: {}", pair.p1.0 * pair.p2.0);
            break;
        }
    }
}

#[derive(Clone, Copy, Debug, Hash, PartialEq, Eq)]
struct Point(i64, i64, i64);

#[derive(Debug, Eq)]
struct Pair {
    p1: Point,
    p2: Point,
}

impl PartialEq for Pair {
    fn eq(&self, other: &Self) -> bool {
        (self.p1 == other.p1 && self.p2 == other.p2) || (self.p1 == other.p2 && self.p2 == other.p1)
    }
}

impl Hash for Pair {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        let mut v = vec![self.p1, self.p2];
        v.sort_by(|a, b| {
            if a.0 != b.0 {
                return a.0.cmp(&b.0);
            }
            if a.1 != b.1 {
                return a.1.cmp(&b.1);
            }
            a.2.cmp(&b.2)
        });
        v[0].hash(state);
        v[1].hash(state);
    }
}

impl Pair {
    fn sep(&self) -> f32 {
        return (((self.p2.0 - self.p1.0).pow(2)
            + (self.p2.1 - self.p1.1).pow(2)
            + (self.p2.2 - self.p1.2).pow(2)) as f32)
            .sqrt();
    }
}

fn join<'a>(
    pair: &'a Pair,
    mut circuits: Box<Vec<HashSet<&'a Point>>>,
) -> Box<Vec<HashSet<&'a Point>>> {
    let mut found = false;
    for set in &mut *circuits {
        if set.contains(&pair.p1) && set.contains(&pair.p2) {
            found = true;
            break; // Do nothing if both are already in a circuit.
        }
        if set.contains(&pair.p1) {
            found = true;
            set.insert(&pair.p2);
            break;
        }
        if set.contains(&pair.p2) {
            found = true;
            set.insert(&pair.p1);
            break;
        }
    }
    if !found {
        let mut s = HashSet::new();
        s.insert(&pair.p1);
        s.insert(&pair.p2);
        circuits.push(s);
    }
    merge(circuits)
}

fn merge(mut sets: Box<Vec<HashSet<&Point>>>) -> Box<Vec<HashSet<&Point>>> {
    for i in 1..sets.len() {
        for j in 0..i {
            if sets[i].intersection(&sets[j]).count() > 0 {
                let mut a = sets.remove(i);
                let b = sets.remove(j);
                a.extend(b);
                sets.push(a);
                return merge(sets);
            }
        }
    }
    sets
}

fn parse(filename: &str) -> Vec<Point> {
    let file = File::open(filename).unwrap();
    let reader = BufReader::new(file);
    reader
        .lines()
        .map(|s| {
            let v: Vec<i64> = s
                .unwrap()
                .split(",")
                .map(|st| st.parse::<i64>().unwrap())
                .collect();
            Point(v[0], v[1], v[2])
        })
        .collect()
}
