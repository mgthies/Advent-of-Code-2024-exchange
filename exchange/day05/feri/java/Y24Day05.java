import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

/**
 * see: https://adventofcode.com/2024/day/5
 *
 */
public class Y24Day05 {

	
	public static void mainPart1(String inputfile) throws FileNotFoundException {
		
		Map<Integer, Set<Integer>> afterRules = new HashMap<>();
		Map<Integer, Set<Integer>> beforeRules = new HashMap<>();
		
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			String line = scanner.nextLine().trim();
			while (!line.isBlank()) {
				int left = Integer.parseInt(line.split("[|]")[0]);
				int right = Integer.parseInt(line.split("[|]")[1]);
				afterRules.computeIfAbsent(left, k->new HashSet<>()).add(right);
				beforeRules.computeIfAbsent(right, k->new HashSet<>()).add(left);
				line = scanner.nextLine().trim();
			}
			System.out.println(afterRules);
			System.out.println(beforeRules);
			System.out.println();
			int solution = 0;
			while (scanner.hasNext()) {
				line = scanner.nextLine().trim();
				List<Integer> nums = Arrays.asList(line.split(",")).stream().map(s -> Integer.parseInt(s)).toList();
				int median = nums.get(nums.size()/2);
				System.out.println(nums+" -> median "+median);
				boolean valid = true;
				Set<Integer> beforeNums = new HashSet<>();
				Set<Integer> afterNums = new HashSet<>(nums);
				for (int num:nums) {
					afterNums.remove(num);
					Set<Integer> numsExpectedAfter = afterRules.computeIfAbsent(num, k->Collections.emptySet());
					Set<Integer> numsExpectedBefore = beforeRules.computeIfAbsent(num, k->Collections.emptySet());
					Set<Integer> wrongBeforeNums = new HashSet<>(beforeNums);
					Set<Integer> wrongAfterNums = new HashSet<>(afterNums);
					wrongBeforeNums.retainAll(numsExpectedAfter);
					wrongAfterNums.retainAll(numsExpectedBefore);
					if (!wrongBeforeNums.isEmpty() || !wrongAfterNums.isEmpty()) {
						valid = false;
						break;
					}
					beforeNums.add(num);
				}
				if (valid) {
					solution += median;
					System.out.println("  OK -> SUM "+solution);
				}
			}
			System.out.println("Solution: "+solution);
		}
	}

	

	public static void mainPart2(String inputfile) throws FileNotFoundException {
		
		Map<Integer, Set<Integer>> beforeRules = new HashMap<>();
		
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			String line = scanner.nextLine().trim();
			while (!line.isBlank()) {
				int left = Integer.parseInt(line.split("[|]")[0]);
				int right = Integer.parseInt(line.split("[|]")[1]);
				beforeRules.computeIfAbsent(right, k->new HashSet<>()).add(left);
				line = scanner.nextLine().trim();
			}
			System.out.println(beforeRules);
			System.out.println();
			int solution = 0;
			while (scanner.hasNext()) {
				line = scanner.nextLine().trim();
				List<Integer> nums = Arrays.asList(line.split(",")).stream().map(s -> Integer.parseInt(s)).toList();
				nums = new ArrayList<>(nums);
				System.out.println(nums);
				Set<Integer> afterNums = new HashSet<>(nums);
				boolean corrected = false;
				for (int i=0; i<nums.size(); i++) {
					int num = nums.get(i); 
					afterNums.remove(num);
					Set<Integer> numsExpectedBefore = beforeRules.computeIfAbsent(num, k->Collections.emptySet());
					Set<Integer> wrongAfterNums = new HashSet<>(afterNums);
					wrongAfterNums.retainAll(numsExpectedBefore);
					if (!wrongAfterNums.isEmpty()) {
						nums.remove(i);
						nums.add(num);
						afterNums.add(num);
						corrected = true;
						i = i-1;
						continue;
					}
				}
				if (corrected) {
					int median = nums.get(nums.size()/2);
					solution += median;
					System.out.println("  SORTED: "+nums+" median "+median+" SUM "+solution);
				}
			}
			System.out.println("Solution: "+solution);
		}
	}


	
	public static void main(String[] args) throws FileNotFoundException {
		System.out.println("--- PART I  ---");
//		mainPart1("exchange/day05/feri/input-example.txt");
		mainPart1("exchange/day05/feri/input.txt");     
		System.out.println("---------------");
		System.out.println();
		System.out.println("--- PART II ---");
//		mainPart2("exchange/day05/feri/input-example.txt");
		mainPart2("exchange/day05/feri/input.txt");
		System.out.println("---------------");
	}

	
}
