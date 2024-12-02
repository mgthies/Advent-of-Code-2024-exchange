import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

/**
 * see: https://adventofcode.com/2024/day/2
 *
 */
public class Y24Day01 {

	
	
	public static void mainPart1(String inputfile) throws FileNotFoundException {
		
		List<Integer> leftNumbers = new ArrayList<>();
		List<Integer> rightNumbers = new ArrayList<>();
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isEmpty()) {
					continue;
				}
				String[] numbers = line.split("   ");
				int left = Integer.parseInt(numbers[0]);
				int right = Integer.parseInt(numbers[1]);
				leftNumbers.add(left);
				rightNumbers.add(right);
			}
		}
		Collections.sort(leftNumbers);
		Collections.sort(rightNumbers);
		int diff = 0;
		for (int i=0; i<leftNumbers.size(); i++) {
			diff = diff + Math.abs(rightNumbers.get(i)-leftNumbers.get(i));
		}
		System.out.println("Sum differences sorted list: "+diff);
	}

	
	
	public static void mainPart2(String inputfile) throws FileNotFoundException {
		
		List<Integer> leftNumbers = new ArrayList<>();
		List<Integer> rightNumbers = new ArrayList<>();
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isEmpty()) {
					continue;
				}
				String[] numbers = line.split("   ");
				int left = Integer.parseInt(numbers[0]);
				int right = Integer.parseInt(numbers[1]);
				leftNumbers.add(left);
				rightNumbers.add(right);
			}
		}
		long similarity = 0;
		for (int i=0; i<leftNumbers.size(); i++) {
			int left = leftNumbers.get(i);
			long countRight = rightNumbers.stream().filter(right -> right == left).count();
			similarity = similarity + left * countRight;
		}
		System.out.println("Similarity: "+similarity);
	}

	public static void main(String[] args) throws FileNotFoundException {
		System.out.println("--- PART I  ---");
//		mainPart1("exchange/day01/feri/input-example.txt");
		mainPart1("exchange/day01/feri/input.txt");     
		System.out.println("---------------");
		System.out.println();
		System.out.println("--- PART II ---");
//		mainPart2("exchange/day01/feri/input-example.txt");
		mainPart2("exchange/day01/feri/input.txt");
		System.out.println("---------------");
	}

	
}
