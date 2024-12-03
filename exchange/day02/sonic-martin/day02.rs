use std::env;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() -> Result<(), String> {
    // Arg parsing
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        return Err(String::from("Usage: cargo run --bin day02 INPUT"));
    }

    // Prepare file for reading
    let input_path = Path::new(&args[1]);
    let file =
        File::open(input_path).map_err(|_| String::from("Failed to open the INPUT file."))?;
    let reader = io::BufReader::new(file);

    // Load and parse input from file
    let mut reports: Vec<Vec<i32>> = Vec::new();
    for (i, line) in reader.lines().enumerate() {
        let line = line.map_err(|_| format!("Failed to read line #{} from INPUT", i))?;
        let numbers: Vec<&str> = line.split_whitespace().collect();

        let mut levels = Vec::<i32>::new();
        for (j, number)  in numbers.iter().enumerate() {
            let level: i32 = number
                .parse()
                .map_err(|_| format!("Failed to parse number #{} on line #{}", j, i))?;
            levels.push(level);
        }
        reports.push(levels);
    }

    // Task 1: Check safety without Problem Dampener
    println!("Safe check without Problem Dampener");
    let mut safe_reports = 0;
    for (i, levels) in reports.iter().enumerate() {
        let is_safe = check_safety(levels.clone(), i+1);
        if is_safe {
            println!("Report #{} is safe.", i+1);
            safe_reports += 1;
        }
    }
    println!("Safe reports without Problem Dampener: {}", safe_reports);

    // Task 2: Check safety with Problem Dampener
    println!("Safe check with Problem Dampener");
    let mut safe_reports_dampener = 0;
    for (i, levels) in reports.iter().enumerate() {
        // Full check
        let is_safe = check_safety(levels.clone(), i+1);
        if is_safe {
            println!("Report #{} is safe without removing any level.", i+1);
            safe_reports_dampener += 1;
            continue;
        }
        // Try to remove any level, and check again
        for j in 0..levels.len() {
            let mut levels_copy = levels.clone();
            levels_copy.remove(j);
            let is_safe = check_safety(levels_copy, i+1);
            if is_safe {
                println!("Report #{} is safe by removing the {} level, {}.", i+1, j, levels[j]);
                safe_reports_dampener += 1;
                break;
            }
        }
    }
    println!("Safe reports with Problem Dampener: {}", safe_reports_dampener);

    Ok(())
}

fn check_safety(levels: Vec<i32>, line: usize) -> bool {
    let mut last_diff = 0;

    for i in 0..levels.len()-1 { // When we have 4 levels, we have 3 comparisons
        let diff = levels[i+1] - levels[i];
        // Any two adjacent levels differ by at least one and at most three.
        if diff.abs() < 1 || diff.abs() > 3 {
            println!("Report #{} is not safe due levels {} and {} differ with {} by less than one or more then three.", line, levels[i], levels[i+1], diff.abs());
            return false;
        }
        // The levels are either all increasing or all decreasing.
        if (last_diff  < 0 && diff > 0) || (last_diff > 0 && diff < 0) {
            println!("Report #{} is not safe due levels {}, {} and {} switch increasing/decreasing.", line, levels[i-1], levels[i], levels[i+1]);
            return false;
        }
        last_diff = diff;
    }

    true
}