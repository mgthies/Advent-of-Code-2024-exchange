import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * see: https://adventofcode.com/2024/day/4
 *
 */
public class Y24Day04withGUI {

	
	static Y24GUIOutput04 output;
	
	public static class CharMatrix {
		private List<String> rows;
		int numRows;
		int numCols;
		int[][] colors;
		public CharMatrix() {
			rows = new ArrayList<>();
			numRows = 0;
			numCols = 0;
		}
		public void addRow(String row) {
			rows.add(row);
			numRows = rows.size();
			numCols = row.length();
		}
		public String getWord(int x, int y, int len, int dx, int dy) {
			StringBuilder word = new StringBuilder();
			for (int i=0; i<len; i++) {
				word.append(get(x+i*dx, y+i*dy));
			}
			return word.toString();
		}
		public void colorWord(int x, int y, int len, int dx, int dy) {
			for (int i=0; i<len; i++) {
				setColor(x+i*dx, y+i*dy, 1);
			}
		}
		private void setColor(int x, int y, int col) {
			if (colors == null) {
				colors = new int[numRows][numCols];
			}
			if ((x<0) || (x>=numCols) || (y<0) || (y>=numRows)) {
				return;
			}
			colors[y][x] = col;
		}
		private int getColor(int x, int y) {
			if (colors == null) {
				colors = new int[numRows][numCols];
			}
			if ((x<0) || (x>=numCols) || (y<0) || (y>=numRows)) {
				return 0;
			}
			return colors[y][x];
		}
		public int countWordOccurrence(String searchWord) {
			int result = 0;
			for (int y=0; y<numRows; y++) {
				for (int x=0; x<numCols; x++) {
					for (int dy=-1; dy<=1; dy++) {
						for (int dx=-1; dx<=1; dx++) {
							if ((dx==0) && (dy==0)) {
								continue;
							}
							String word = getWord(x,y,searchWord.length(),dx, dy);
							if (word.equals(searchWord)) {
								result++;
								colorWord(x, y, searchWord.length(), dx, dy);
								showColorMatrix(result);
							}
						}
					}
				}
			}
			return result;
		}
		public int countMASOccurrence() {
			int result = 0;
			for (int y=0; y<numRows; y++) {
				for (int x=0; x<numCols; x++) {
					int cntMAS=0;
					String word1 = getWord( x-1,y-1, 3,  1,1  );
					String word2 = getWord( x-1,y+1, 3,  1,-1 );
					String word3 = getWord( x+1,y-1, 3, -1,1  );
					String word4 = getWord( x+1,y+1, 3, -1,-1 );
					if (word1.equals("MAS")) {
						cntMAS++;
					}
					if (word2.equals("MAS")) {
						cntMAS++;
					}
					if (word3.equals("MAS")) {
						cntMAS++;
					}
					if (word4.equals("MAS")) {
						cntMAS++;
					}
					if (cntMAS==2) {
						result++;
						if (word1.equals("MAS")) {
							colorWord( x-1,y-1, 3,  1,1  );
						}
						if (word2.equals("MAS")) {
							colorWord( x-1,y+1, 3,  1,-1 );
						}
						if (word3.equals("MAS")) {
							colorWord( x+1,y-1, 3, -1,1  );
						}
						if (word4.equals("MAS")) {
							colorWord( x+1,y+1, 3, -1,-1 );
						}
						showColorMatrix(result);
					}
				}
			}
			return result;
		}
		public char get(int x, int y) {
			if ((x<0) || (x>=numCols) || (y<0) || (y>=numRows)) {
				return '.';
			}
			return rows.get(y).charAt(x);
		}
		@Override
		public String toString() {
			StringBuilder result = new StringBuilder();
			result.append("SHAPE ("+numCols+","+numRows+")\n");
			for (String row:rows) {
				result.append(row).append('\n');
			}
			return result.toString();
		}
		private static String[] STYLE = {"b0", "b3", "b2"};
		private void showColorMatrix(int count) {
			StringBuilder text = new StringBuilder();
			text.append("FOUND: "+count+"\n");
			for (int y=0; y<numRows; y++) {
				int currentColor = 0;
				for (int x=0; x<numCols; x++) {
					char c = get(x,y);
					int col = getColor(x, y);
					if (col == 1) {
						setColor(x, y, 2);
					}
					// currentColor=-1;
					if (col != currentColor) {
						currentColor = col;
						text.append(output.style(STYLE[col]));
					}
					text.append(c);
				}
				text.append(output.style(STYLE[0]));
				text.append("\n");
			}
			output.addStep(text.toString());
		}
	}
	
	
	public static void mainPart1(String inputfile) throws FileNotFoundException {
		
		output = new Y24GUIOutput04("2024 Day 04 Part 1", true);
		
		CharMatrix matrix = new CharMatrix();
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isBlank()) {
					continue;
				}
				matrix.addRow(line);
			}
		}
		matrix.showColorMatrix(0);
		int countXMAS = matrix.countWordOccurrence("XMAS");
		System.out.println("The word XMAS occurrs "+countXMAS+" times"); 
	}

	

	public static void mainPart2(String inputfile) throws FileNotFoundException {
		
		output = new Y24GUIOutput04("2024 Day 04 Part 2", true);
		
		CharMatrix matrix = new CharMatrix();
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isBlank()) {
					continue;
				}
				matrix.addRow(line);
			}
		}
		matrix.showColorMatrix(0);
		int countMAS = matrix.countMASOccurrence();
		System.out.println("a cross MAS occurrs "+countMAS+" times"); 
	}


	
	public static void main(String[] args) throws FileNotFoundException {
		System.out.println("--- PART I  ---");
		mainPart1("exchange/day04/feri/input-example.txt");
//		mainPart1("exchange/day04/feri/input.txt");     
		System.out.println("---------------");
		System.out.println();
		System.out.println("--- PART II ---");
//		mainPart2("exchange/day04/feri/input-example.txt");
		mainPart2("exchange/day04/feri/input.txt");
		System.out.println("---------------");
	}

	
}
