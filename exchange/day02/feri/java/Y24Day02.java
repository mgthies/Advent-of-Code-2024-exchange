import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

/**
 * see: https://adventofcode.com/2024/day/2
 *
 */
public class Y24Day02 {

	
	
	public static void mainPart1(String inputfile) throws FileNotFoundException {
		
		int cntSafe = 0;
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isEmpty()) {
					continue;
				}
				String[] stringReport = line.split(" ");
				List<Integer> report = Arrays.asList(stringReport).stream().map(level -> Integer.parseInt(level)).toList();
				if (checkSafe(report)) {
					cntSafe++;
				}
			}
		}
		System.out.println("Number of safe lines: "+cntSafe);
	}

	

	private static boolean checkSafe(List<Integer> report) {
		int direction = (int)Math.signum(report.get(1) - report.get(0));
		for (int i=1; i<report.size(); i++) {
			int diff = report.get(i) - report.get(i-1);
			diff = diff * direction;
			if ((diff < 1) || (diff > 3)) {
				return false;
			}
		}
		return true;
	}



	public static void mainPart2(String inputfile) throws FileNotFoundException {
		int cntSafe = 0;
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isEmpty()) {
					continue;
				}
				String[] stringReport = line.split(" ");
				List<Integer> report = Arrays.asList(stringReport).stream().map(level -> Integer.parseInt(level)).toList();
				if (checkDampenerSafe(report)) {
					cntSafe++;
				}
			}
		}
		System.out.println("Number of safe lines after dampening: "+cntSafe);
	}

	private static boolean checkDampenerSafe(List<Integer> report) {
		if (checkSafe(report)) {
			return true;
		}
		for (int indexToRemove=0; indexToRemove<report.size(); indexToRemove++) {
			List<Integer> dampenedReport = new ArrayList<>(report);
			dampenedReport.remove(indexToRemove);
			if (checkSafe(dampenedReport)) {
				return true;
			}
		}
		return false;
	}


	
	public static void main(String[] args) throws FileNotFoundException {
		System.out.println("--- PART I  ---");
//		mainPart1("exchange/day02/feri/input-example.txt");
		mainPart1("exchange/day02/feri/input.txt");     
		System.out.println("---------------");
		System.out.println();
		System.out.println("--- PART II ---");
//		mainPart2("exchange/day02/feri/input-example.txt");
		mainPart2("exchange/day02/feri/input.txt");
		System.out.println("---------------");
	}

	
}
