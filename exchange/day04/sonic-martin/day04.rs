use std::fs::File;
use std::io::BufRead;
use std::path::Path;
use std::{env, io};

type Point = (i32, i32);
type Flow = Vec<Point>;
type Flows = Vec<Flow>;
type Puzzle = Vec<Vec<char>>;

fn main() -> Result<(), String> {
    // Arg parsing
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        return Err(String::from("Usage: cargo run --bin day04 INPUT"));
    }

    // Prepare file for reading
    let input_path = Path::new(&args[1]);
    let file =
        File::open(input_path).map_err(|_| String::from("Failed to open the INPUT file."))?;
    let reader = io::BufReader::new(file);

    // Load and parse input from file
    let mut puzzle: Puzzle = Vec::new();
    for line in reader.lines() {
        puzzle.push(line.unwrap().chars().collect());
    }

    // Task 1: Go through every character in the word search and look via the flow rules for the searched word.
    let word = "XMAS";
    let flows1: Flows = vec![
        vec![(0, 0), (1, 0), (2, 0), (3, 0)],    // Horizontal forwards
        vec![(3, 0), (2, 0), (1, 0), (0, 0)],    // Horizontal reverse
        vec![(0, 0), (0, 1), (0, 2), (0, 3)],    // Vertical forwards
        vec![(0, 3), (0, 2), (0, 1), (0, 0)],    // Vertical reverse
        vec![(0, 0), (1, 1), (2, 2), (3, 3)],    // Diagonal forwards
        vec![(3, 3), (2, 2), (1, 1), (0, 0)],    // Diagonal reverse
        vec![(0, 0), (1, -1), (2, -2), (3, -3)], // Diagonal forwards
        vec![(3, -3), (2, -2), (1, -1), (0, 0)], // Diagonal reverse
    ];
    let mut occurrences1: i32 = 0;
    for row in 0..puzzle.len() {
        for column in 0..puzzle[row].len() {
            for flow in &flows1 {
                let point = (column as i32, row as i32);
                if check(word, point, flow, &puzzle) {
                    occurrences1 += 1;
                }
            }
        }
    }
    println!("Task 1: {}", occurrences1);

    // Task 2: Go through every "X" character, and from thereone look for two diagonal "MAS".
    let word = "MAS";
    let char_in_the_middle = 'A';
    let flows2: Flows = vec![
        vec![(-1, -1), (0, 0), (1, 1)], // Diagonal forwards
        vec![(1, 1), (0, 0), (-1, -1)], // Diagonal reverse
        vec![(-1, 1), (0, 0), (1, -1)], // Diagonal forward
        vec![(1, -1), (0, 0), (-1, 1)], // Diagonal reverse
    ];
    let mut occurrences2: i32 = 0;
    for row in 0..puzzle.len() {
        for column in 0..puzzle[row].len() {
            let char = puzzle[row][column];
            if char != char_in_the_middle {
                continue;
            }
            let mut local_occurrences: i32 = 0;
            for flow in &flows2 {
                let point = (column as i32, row as i32);
                if check(word, point, flow, &puzzle) {
                    local_occurrences += 1;
                }
            }
            if local_occurrences == 2 {
                occurrences2 += 1;
            }
        }
    }
    println!("Task 2: {}", occurrences2);

    Ok(())
}

// Check looks up for a given flow rule if the searched word is in the puzzle.
fn check(word: &str, start: Point, flow: &Flow, puzzle: &Puzzle) -> bool {
    let chars_searched = word.chars().collect::<Vec<char>>();

    for (i, delta) in flow.iter().enumerate() {
        let x = start.0 + delta.0;
        let y = start.1 + delta.1;

        if x < 0 || y < 0 || y >= puzzle.len() as i32 || x >= puzzle[y as usize].len() as i32 {
            return false;
        }

        let char_searched = chars_searched[i];
        let char_is = puzzle[y as usize][x as usize];
        if char_searched != char_is {
            return false;
        }
    }
    
    true
}
