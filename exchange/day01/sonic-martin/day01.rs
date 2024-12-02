use std::env;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() -> Result<(), String> {
    // Arg parsing
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        return Err(String::from("Usage: cargo run --bin day01 INPUT"));
    }

    // Prepare file for reading
    let input_path = Path::new(&args[1]);
    let file =
        File::open(input_path).map_err(|_| String::from("Failed to open the INPUT file."))?;
    let reader = io::BufReader::new(file);

    // Load and parse input from file
    let mut left_list = Vec::<i32>::new();
    let mut right_list = Vec::<i32>::new();
    for (i, line) in reader.lines().enumerate() {
        let line = line.map_err(|_| String::from("Failed to read a line from the INPUT file."))?;
        let numbers: Vec<&str> = line.split_whitespace().collect();
        if numbers.len() != 2 {
            return Err(format!("Invalid INPUT line #{}: {}", i, line));
        }
        let left: i32 = numbers[0]
            .parse()
            .map_err(|_| format!("Failed to parse left number on line #{}", i))?;
        let right: i32 = numbers[1]
            .parse()
            .map_err(|_| format!("Failed to parse right number on line #{}", i))?;
        left_list.push(left);
        right_list.push(right);
    }
    if left_list.len() != right_list.len() {
        return Err(String::from("Number of left and right lines do not match."));
    }

    // Sort input
    left_list.sort_unstable();
    right_list.sort_unstable();

    // Task 1: Calculate the diffs by line and sum them up
    let mut diffs = Vec::<i32>::new();
    for i in 0..left_list.len() {
        let diff = (left_list[i] - right_list[i]).abs();
        diffs.push(diff);
    }
    let total_distance = diffs.iter().sum::<i32>();
    println!("Total distance: {}", total_distance);

    // Task 2: Similarity score, by summing up for each number in the left its occurrences in the
    // right by multiplying the number with that count.
    let mut similarity_score: i32 = 0;
    for number in left_list {
        let occurrences = right_list.iter().filter(|&&x| x == number).count() as i32;
        similarity_score += number * occurrences;
    }
    println!("Similarity score: {}", similarity_score);

    Ok(())
}
