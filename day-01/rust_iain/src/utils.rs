use std::fs::read_to_string;

// A test macro that doesn't panic.
#[macro_export]
macro_rules! compare {
    ($result:expr, $expected:expr) => {
        if $result != $expected {
            Err(format!("Expected {}, got {}", $expected, $result))
        } else {
            Ok(())
        }
    };
}

#[allow(dead_code)]
pub(crate) fn example_input(day: &str) -> String {
    let result = read_to_string("example/".to_string() + day + ".txt").unwrap();
    if result.len() == 0 {
        panic!("No example found for day {}", day);
    }
    result
}

#[allow(dead_code)]
pub(crate) fn problem_input(day: &str) -> String {
    let result = read_to_string("input/".to_string() + day + ".txt").unwrap();
    if result.is_empty() {
        panic!("No input found for day {}", day);
    }
    result
}

#[allow(dead_code)]
// Rotate the matrix 90 degrees clockwise, so that we can check the columns.
pub(crate) fn rotate_90(cw: &[Vec<char>]) -> Vec<Vec<char>> {
    let mut ccw = vec![vec![' '; cw.len()]; cw[0].len()];
    for i in 0..cw.len() {
        for j in 0..cw[0].len() {
            ccw[j][cw.len() - i - 1] = cw[i][j];
        }
    }
    ccw
}

#[allow(dead_code)]
// Rotate the matrix 45 degrees clockwise, so that we can check the diagonals.
pub(crate) fn rotate_45(cw: &[Vec<char>]) -> Vec<Vec<char>> {
    let mut ccw = Vec::new();

    // starting at the first col, iterate down the rows, going up and right at 45 degrees
    for i in 0..cw.len() {
        let mut x = i; // rows 0 to n
        let mut y = 0; // first column
        let mut diagonal = Vec::new();
        diagonal.push(cw[x][y]);
        while x > 0 && y < cw[0].len() - 1 {
            x -= 1; // up
            y += 1; // right
            diagonal.push(cw[x][y]);
        }
        ccw.push(diagonal);
    }

    // starting at the last row, iterate across the columns, from col 1 (not 0) going up and right at 45 degrees
    for j in 1..cw[0].len() {
        let mut x = cw.len() - 1; // last row
        let mut y = j; // columns 1 to n
        let mut diagonal = Vec::new();
        diagonal.push(cw[x][y]);
        while x > 0 && y < cw[0].len() - 1 {
            x -= 1; // up
            y += 1; // right
            diagonal.push(cw[x][y]);
        }
        ccw.push(diagonal);
    }

    ccw
}

pub(crate) struct Array2D<T> {
    data: Vec<Vec<T>>,
}

impl<T> Array2D<T> {
    #[allow(dead_code)]
    // Create an Array2D with the given dimensions, filled with the default value of T.
    fn default(rows: usize, cols: usize) -> Self
    where
        T: Default,
    {
        let mut data = Vec::new();
        for _ in 0..rows {
            let mut row = Vec::new();
            for _ in 0..cols {
                row.push(T::default());
            }
            data.push(row);
        }
        Array2D { data }
    }
    #[allow(dead_code)]
    // Create an Array2D with the given dimensions, filled with the default value of T.
    pub fn new(d: Vec<Vec<T>>) -> Self {
        Array2D { data: d }
    }
}

impl<'a, T> IntoIterator for &'a Array2D<T> {
    type Item = ((&'a T, &'a T, &'a T), (&'a T, &'a T, &'a T));
    type IntoIter = Array2DIterator<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        Array2DIterator {
            array2d: self,
            i: 0,
            j: 0,
        }
    }
}

pub(crate) struct Array2DIterator<'a, T> {
    array2d: &'a Array2D<T>,
    i: usize,
    j: usize,
}

impl<'a, T> Iterator for Array2DIterator<'a, T> {
    type Item = ((&'a T, &'a T, &'a T), (&'a T, &'a T, &'a T));

    fn next(&mut self) -> Option<Self::Item> {
        // if we're at the end of the row, move to the next row
        if self.j + 2 >= self.array2d.data[0].len() {
            self.j = 0;
            self.i += 1;
        }

        // if we're at the end of the column, return None
        if self.i + 2 >= self.array2d.data.len() {
            return None;
        }

        // otherwise, return the slices
        let down_and_right = (
            &self.array2d.data[self.i][self.j],
            &self.array2d.data[self.i + 1][self.j + 1],
            &self.array2d.data[self.i + 2][self.j + 2],
        );
        let up_and_right = (
            &self.array2d.data[self.i + 2][self.j],
            &self.array2d.data[self.i + 1][self.j + 1],
            &self.array2d.data[self.i][self.j + 2],
        );

        self.j += 1;
        Some((down_and_right, up_and_right))
    }
}

#[allow(dead_code)]
fn main() {}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_rotate_90() {
        let cw = vec![vec!['a', 'b', 'c'], vec!['d', 'e', 'f']];
        let ccw = vec![vec!['d', 'a'], vec!['e', 'b'], vec!['f', 'c']];
        assert_eq!(rotate_90(&cw), ccw);
    }

    #[test]
    fn test_rotate_45_one() {
        let orig = vec![vec!['a', 'b', 'c'], vec!['d', 'e', 'f']];

        let rotated = vec![vec!['a'], vec!['d', 'b'], vec!['e', 'c'], vec!['f']];
        assert_eq!(rotate_45(&orig), rotated);
    }
    #[test]
    fn test_rotate_45_two() {
        let orig = vec![vec!['a'], vec!['d']];
        assert_eq!(rotate_45(&orig), orig);
    }

    #[test]
    fn test_array_2d() {
        let i = Array2D::<i32>::default(3, 2);
        assert_eq!(i.data, vec![vec![0, 0], vec![0, 0], vec![0, 0]]);

        let f: Array2D<f32> = Array2D::default(3, 2);
        assert_eq!(f.data, vec![vec![0.0, 0.0], vec![0.0, 0.0], vec![0.0, 0.0]]);
    }

    #[test]
    fn test_array_2d_iter() {
        // \0 \0 \0
        // \0 \0 \0
        // \0 \0 \0
        let mut a = Array2D::<char>::default(3, 3);
        a.data[0][0] = 'a';
        a.data[2][2] = 'b';
        let mut iter = a.into_iter();

        let a = (&'a', &'\0', &'b');
        let b = (&'\0', &'\0', &'\0');

        let some_slice = Some((a, b));
        assert_eq!(some_slice, iter.next());
        assert_eq!(None, iter.next());
    }

    #[test]
    fn test_new() {
        Array2D::new(vec![vec![1]]);
    }
}
