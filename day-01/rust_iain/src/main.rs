use std::env;
use std::fs::read_to_string;
mod utils;

fn left(start: u32, steps: u32) -> (u32, u32) {
    // Get the end position and the number of times zero was crossed.
    let position = (start + (100 - (steps % 100))) % 100;
    let mut a = 0;
    if ((steps % 100) > start) && (start > 0) {
        a = 1;
    } 
    let zeros = steps / 100;
    (position, zeros + a)
}

fn right(start: u32, steps: u32) -> (u32, u32) {
    // Get the end position and the number of times zero was crossed.
    let position = (start + steps) % 100;
    let mut a = 0;
    if (start + (steps % 100)) > 100 {
        a = 1;
    }
    let zeros = steps  / 100;
    (position, zeros + a)
}

fn part_one(input: String) -> u32 {
    let mut zeros = 0;
    let mut position = 50;
    input.lines().for_each(|_line| {
        println!("position {}", position);
        let steps = _line[1..].parse::<u32>().unwrap();
        position = if _line.starts_with("L") {
            left(position, steps).0
        } else {
            right(position, steps).0
        };
        if position == 0 {
            zeros += 1;
        }
    });
    zeros
}

fn part_two(input: String) -> u32 {
    let mut zeros = 0;
    let mut position = 50;
    input.lines().for_each(|_line| {
        println!("position {}", position);
        let steps = _line[1..].parse::<u32>().unwrap();
        let result = if _line.starts_with("L") {
            left(position, steps)
        } else {
            right(position, steps)
        };
        position = result.0;
        zeros += result.1;
        if position == 0 {
            zeros += 1;
        }
    });
    zeros
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let input = read_to_string("input.txt").unwrap();
    if args[args.len() - 1] == "one" {
        println!("{}", part_one(input));
        return;
    }
    println!("{}", part_two(input));
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_left() {
        assert_eq!(left(5, 4).0, 1);
        assert_eq!(left(0, 1).0, 99);
        assert_eq!(left(0, 101).0, 99);
        assert_eq!(left(50, 1).0, 49);
    }

    #[test]
    fn test_left_part_two() {
        assert_eq!(left(5, 4).1, 0);
        assert_eq!(left(1, 2).1, 1);
        assert_eq!(left(1, 1).1, 0);
        assert_eq!(left(0, 100).1, 1);
        assert_eq!(left(50, 150).1, 1);
        assert_eq!(left(0, 1).1, 0);
        assert_eq!(left(0, 0).1, 0);
    }

    #[test]
    fn test_right() {
        assert_eq!(right(99, 1).0, 0);
        assert_eq!(right(99, 3).0, 2);
        assert_eq!(right(99, 200).0, 99);
    }

    #[test]
    fn test_right_part_two() {
        assert_eq!(right(50, 1000).1, 10);
        assert_eq!(right(0, 100).1, 1);
        assert_eq!(right(99, 1).1, 0);
        assert_eq!(right(99, 2).1, 1);
        assert_eq!(right(50, 150).1, 1);
        assert_eq!(right(0, 1).1, 0);
        assert_eq!(right(0, 0).1, 0);
    }

    #[test]
    fn test_part_one() -> Result<(), String> {
        let input = read_to_string("example.txt").unwrap();
        let result = part_one(input);
        compare!(result, 3)
    }

    #[test]
    fn test_part_two() -> Result<(), String> {
        let input = read_to_string("example.txt").unwrap();
        let result = part_two(input);
        compare!(result, 6)
    }

    #[test]
    fn test_integer_div() {
        let a: u32 = 107;
        let b: u32 = 100;
        let c = a / b;
        println!("107 / 100 is {}", c);
    }
}
