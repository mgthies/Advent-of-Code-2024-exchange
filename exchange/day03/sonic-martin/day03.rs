use std::{env, fs};
use std::path::Path;
use regex::Regex;

fn main() -> Result<(), String> {
    // Arg parsing
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        return Err(String::from("Usage: cargo run --bin day03 INPUT"));
    }

    // Load file into string
    let input_path = Path::new(&args[1]);
    let input = fs::read_to_string(input_path).unwrap();


    // Parse instructions from input
    let mut instructions = vec![];
    let re = Regex::new(r"(?m)(?<mul>mul)\((?<first>[0-9]{1,3}),(?<second>[0-9]{1,3})\)|(?<do>do)\(\)|(?<dont>don't)\(\)").unwrap();
    for caps in re.captures_iter(&*input) {
        let func = caps.name("mul").or_else(|| caps.name("do")).or_else(|| caps.name("dont")).unwrap().as_str();
        match func {
            "mul" => {
                let first: i64 = caps.name("first").unwrap().as_str().parse().unwrap();
                let second: i64 = caps.name("second").unwrap().as_str().parse().unwrap();
                instructions.push((func, first, second))
            }
            "do" | "don't" => {
                instructions.push((func, 0, 0))
            }
            &_ => {}
        }
    }

    // Task 1: Scan the corrupted memory for uncorrupted mul instructions.
    // Multiply and add the results.
    let mut result1: i64 = 0;
    for instruction in instructions.clone() {
        result1 += instruction.1 * instruction.2
    }
    println!("Task 1: {}", result1);

    // Task 2: Take into account do() and don't()
    let mut is_enabled = true;
    let mut result2: i64 = 0;
    for instruction in instructions.clone() {
        let func = instruction.0;
        let first = instruction.1;
        let second = instruction.2;
        match func {
            "mul" => {
                if is_enabled {
                    result2 += first * second
                }
            }
            "do" => {
                is_enabled = true;
            }
            "don't" => {
                is_enabled = false;
            }
            &_ => {}
        }
    }
    println!("Task 2: {}", result2);

    Ok(())
}