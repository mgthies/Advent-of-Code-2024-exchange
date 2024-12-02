import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

import static java.lang.Integer.parseInt;
import static java.lang.Math.abs;

/**
 * see: https://adventofcode.com/2018/day/1
 *
 */
public class Y24Day02 {



	public static void mainPart1(String inputfile) throws FileNotFoundException {

		long solution = 0;
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				boolean safe = false;
				boolean decrease = false;
				String line = scanner.nextLine().trim();
				if (line.isEmpty()) {
					continue;
				}
				String[] s = line.split(" ");
				System.out.println("### " + line + " ###");
				if (parseInt(s[1]) - parseInt(s[0]) < 0) {decrease = true;}
				else if (parseInt(s[1]) - parseInt(s[0]) == 0) {
					System.out.println(line + ": Unsafe because " + s[0] + " " + s[1] + " is neither an increase or a decrease");
				}
				for (int i = 1; i < s.length; i++) {
					int delta = parseInt(s[i]) - parseInt(s[i - 1]);
					if (delta == 0) {
						System.out.println(line + ": Unsafe because " + s[i-1] + " " + s[i] + " is neither an increase or a decrease");
						safe = false;
						break;
					}
					if (!decrease && delta < 0) {
						System.out.println(line + ": Unsafe because " + " is increasing but " + s[i-1] + " " + s[i] + "is decreasing.");
						safe = false;
						break;
					}
					if (decrease && delta > 0) {
						System.out.println(line + ": Unsafe because " + " is decreasing but " + s[i-1] + " " + s[i] + "is increasing.");
						safe = false;
						break;
					}

					if (!decrease) {
						if (1 <= abs(delta) && abs(delta) <= 3) {
							safe = true;
						} else {
							safe = false;
							System.out.println(line + ": Unsafe because " + s[i - 1] + " " + s[i] + " is an increase of " + abs(parseInt(s[i]) - parseInt(s[i - 1])) + ".");
							break;
						}
					} else {
						if (1 <= abs(delta) && abs(delta) <= 3) {
							safe = true;
						} else {
							safe = false;
							System.out.println(line + ": Unsafe because " + s[i - 1] + " " + s[i] + " is an decrease of " + abs(parseInt(s[i]) - parseInt(s[i - 1])) + ".");
							break;
						}
					}
				}
				if (safe) {
					solution++;
					System.out.println(line + ": Safe because the levels are all increasing by 1, 2, or 3.");
				}
			}
		}
		System.out.println("resulting frequency: "+solution);
	}
	public static void mainPart2(String inputfile) throws FileNotFoundException {

		long solution = 0;
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				boolean safe = false;
				boolean decrease = false;
				String line = scanner.nextLine().trim();
				if (line.isEmpty()) {
					continue;
				}
				String[] s = line.split(" ");
				System.out.println("### " + line + " ###");
				if (parseInt(s[1]) - parseInt(s[0]) < 0) {decrease = true;}
				else if (parseInt(s[1]) - parseInt(s[0]) == 0) {
					System.out.println(line + ": Unsafe because " + s[0] + " " + s[1] + " is neither an increase or a decrease");
				}
				for (int i = 1; i < s.length; i++) {
					int delta = parseInt(s[i]) - parseInt(s[i - 1]);
					if (delta == 0) {
						System.out.println(line + ": Unsafe because " + s[i-1] + " " + s[i] + " is neither an increase or a decrease");
						safe = false;
						break;
					}
					if (!decrease && delta < 0) {
						System.out.println(line + ": Unsafe because " + " is increasing but " + s[i-1] + " " + s[i] + "is decreasing.");
						safe = false;
						break;
					}
					if (decrease && delta > 0) {
						System.out.println(line + ": Unsafe because " + " is decreasing but " + s[i-1] + " " + s[i] + "is increasing.");
						safe = false;
						break;
					}

					if (!decrease) {
						if (1 <= abs(delta) && abs(delta) <= 3) {
							safe = true;
						} else {
							safe = false;
							System.out.println(line + ": Unsafe because " + s[i - 1] + " " + s[i] + " is an increase of " + abs(parseInt(s[i]) - parseInt(s[i - 1])) + ".");
							break;
						}
					} else {
						if (1 <= abs(delta) && abs(delta) <= 3) {
							safe = true;
						} else {
							safe = false;
							System.out.println(line + ": Unsafe because " + s[i - 1] + " " + s[i] + " is an decrease of " + abs(parseInt(s[i]) - parseInt(s[i - 1])) + ".");
							break;
						}
					}
				}
				if (safe) {
					solution++;
					System.out.println(line + ": Safe because the levels are all increasing by 1, 2, or 3.");
				}
			}
		}
		System.out.println("resulting frequency: "+solution);
	}

	
	
	public static void mainPart2old(String inputfile) throws FileNotFoundException {
		
		List<Long> changes = readChanges(inputfile);
		long duplicateFrequency = findFirstDuplicateFrequency(changes);
		System.out.println("first duplicate frequency: "+duplicateFrequency);
		
	}

	private static List<Long> readChanges(String inputfile) throws FileNotFoundException {
		List<Long> changes = new ArrayList<>();
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isEmpty()) {
					continue;
				}
				changes.add(Long.parseLong(line));
			}
		}
		return changes;
	}
	
	private static long findFirstDuplicateFrequency(List<Long> changes) {
		Set<Long> existingFrequencies = new HashSet<>();
		long frequency = 0;
		long cnt = 0;
		existingFrequencies.add(frequency);
		while (true) {
			for (long change:changes) {
				frequency += change;
				cnt++;
				if (!existingFrequencies.add(frequency)) {
					System.out.println("CNT: "+cnt);
					return frequency;
				}
			}
		}
	}


	
	public static void main(String[] args) throws FileNotFoundException {
		System.out.println("--- PART I  ---");
		//mainPart1("/Users/mth/Projekte/2024/advent/Advent-of-Code-2024-exchange/exchange/day02/mgthies/sample2.txt");
		//mainPart1("/Users/mth/Projekte/2024/advent/Advent-of-Code-2024-exchange/exchange/day02/mgthies/data02_2024.csv");
		System.out.println("---------------");
		System.out.println();
		System.out.println("--- PART II ---");
		mainPart2("/Users/mth/Projekte/2024/advent/Advent-of-Code-2024-exchange/exchange/day02/mgthies/sample2.txt");
		// mainPart2("/Users/mth/Projekte/2024/advent/Advent-of-Code-2024-exchange/exchange/day02/mgthies/data02_2024.csv");
		System.out.println("---------------");
	}

	
}
