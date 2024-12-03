import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * see: https://adventofcode.com/2024/day/3
 *
 */
public class Y24Day03 {

	
	public static void mainPart1(String inputfile) throws FileNotFoundException {
		
		Pattern MUL_RX = Pattern.compile("mul[(]([0-9]+),([0-9]+)[)]");
		
		int sumMuls = 0;
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				Matcher matcher = MUL_RX.matcher(line);
				while (matcher.find()) {
//		            System.out.println("Found at index " + matcher.start() + " - " + matcher.group());
		            int x = Integer.parseInt(matcher.group(1));
		            int y = Integer.parseInt(matcher.group(2));
		            sumMuls += x*y;
		        }
			}
		}
		System.out.println("Sum of Muls: "+sumMuls);
	}

	

	public static void mainPart2(String inputfile) throws FileNotFoundException {
		
		Pattern MUL_RX = Pattern.compile("(mul[(]([0-9]+),([0-9]+)[)]|do[(][)]|don't[(][)])");
		
		int sumMuls = 0;
		boolean doSum = true;
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				Matcher matcher = MUL_RX.matcher(line);
				while (matcher.find()) {
//		            System.out.println("Found at index " + matcher.start() + " - " + matcher.group());
					if (matcher.group().equals("do()")) {
						doSum = true;
					}
					else if (matcher.group().equals("don't()")) {
						doSum = false;
					}
					else if (doSum) {
			            int x = Integer.parseInt(matcher.group(2));
			            int y = Integer.parseInt(matcher.group(3));
			            sumMuls += x*y;
					}
		        }
			}
		}
		System.out.println("Sum of Muls: "+sumMuls);
	}


	
	public static void main(String[] args) throws FileNotFoundException {
		System.out.println("--- PART I  ---");
//		mainPart1("exchange/day03/feri/input-example.txt");
		mainPart1("exchange/day03/feri/input.txt");     
		System.out.println("---------------");
		System.out.println();
		System.out.println("--- PART II ---");
//		mainPart2("exchange/day03/feri/input-example.txt");
		mainPart2("exchange/day03/feri/input.txt");
		System.out.println("---------------");
	}

	
}
