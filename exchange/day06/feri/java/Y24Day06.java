import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

/**
 * see: https://adventofcode.com/2024/day/6
 *
 */
public class Y24Day06 {

	
	public static final int DIR_NORTH = 0;
	public static final int DIR_EAST = 1;
	public static final int DIR_SOUTH = 2;
	public static final int DIR_WEST = 3;

	public static final char[] DIR_CHARS = {'^', '>', 'v', '<'}; 
	public static final Pos[] DIR_VECTORS = {new Pos(0,-1), new Pos(1,0),new Pos(0,1), new Pos(-1,0)}; 

	public static int turnRight(int dir) { return (dir+1)%4; }
	public static int uTurn(int dir) { return (dir+2)%4; }
	public static int turnLeft(int dir) { return (dir+3)%4; }
	
	record Pos(int x, int y) {
		@Override public String toString() {
			return "("+x+","+y+")";
		}
		public Pos add(Pos other) {
			return add(other.x, other.y);
		}
		public Pos add(int dx, int dy) {
			return new Pos(x+dx, y+dy);
		}
		public Pos move(int dir) {
			return add(DIR_VECTORS[dir]);
		}
	}
	
	public static class Lab {
		private List<String> rows;
		int maxX;
		int maxY;
		
		public Pos guardPos;
		public int guardDir;
		
		public int ticks;
		
		public Map<Pos, Character> visitedPositions;
		
		public Lab() {
			rows = new ArrayList<>();
			maxX = 0;
			maxY = 0;
			guardPos = null;
			guardDir = -1;
			visitedPositions = null;
			ticks = 0;
		}
		public Lab(Lab other) {
			this.rows = new ArrayList<>(other.rows);
			this.maxX = other.maxX;
			this.maxY = other.maxY;
			this.guardPos = other.guardPos;
			this.guardDir = other.guardDir;
			this.visitedPositions = new HashMap<>(other.visitedPositions);
			this.ticks = other.ticks;
		}
		public void addRow(String row) {
			rows.add(row);
			maxX = row.length();
			maxY = rows.size();
		}
		public char get(Pos pos) {
			return get(pos.x, pos.y);
		}
		public char get(int x, int y) {
			if ((x<0) || (x>=maxX) || (y<0) || (y>=maxY)) {
				return '+';
			}
			return rows.get(y).charAt(x);
		}
		public boolean isFree(int x, int y) {
			return get(x, y) == '.';
		}
		public boolean isBlocked(Pos pos) {
			return get(pos) == '#';
		}
		public boolean isOutside(Pos pos) {
			return get(pos) == '+';
		}
		public boolean isLoop() {
			Character c = visitedPositions.get(guardPos); 
			return (c != null) && (c == DIR_CHARS[guardDir]);
		}
		public void findGuardPos() {
			for (int y=0; y<maxY; y++) {
				for (int x=0; x<maxX; x++) {
					if (get(x,y) == '^') {
						guardPos = new Pos(x,y);
						guardDir = DIR_NORTH;
						visitedPositions = new HashMap<>();
						visitedPositions.put(guardPos, '^');
						return;
					}
				}
			}
			throw new RuntimeException("start pos not found");
		}
		public boolean tick() {
			ticks++;
			Pos nextPos = guardPos.move(guardDir);
			while (isBlocked(nextPos)) {
				guardDir = turnRight(guardDir);
				nextPos = guardPos.move(guardDir);
			}
			guardPos = nextPos;
			if (isLoop()) {
				return false;
			}
			if (isOutside(guardPos)) {
				return false;
			}
			visitedPositions.put(guardPos, DIR_CHARS[guardDir]);
			return true;
		}
		@Override
		public String toString() {
			StringBuilder result = new StringBuilder();
			result.append("SHAPE ("+maxX+","+maxY+"), TICKS "+ticks+", POS "+guardPos+", DIR "+DIR_CHARS[guardDir]+"\n");
			for (int y=0; y<maxY; y++) {
				for (int x=0; x<maxX; x++) {
					Pos p = new Pos(x,y);
					char c = get(p);
					if (visitedPositions.containsKey(p)) {
						c = visitedPositions.get(p);
					}
					result.append(c);
				}
				result.append('\n');
			}
			return result.toString();
		}
		public void set(int x, int y, char c) {
			String row = rows.get(y);
			String newRow = row.substring(0,x) + c + row.substring(x+1);
			rows.set(y, newRow);
		}
	}
	
	
	public static void mainPart1(String inputfile) throws FileNotFoundException {
		
		Lab lab = new Lab();
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isBlank()) {
					continue;
				}
				lab.addRow(line);
			}
		}
		lab.findGuardPos();
		while (lab.tick()) {
//			System.out.println(lab);
		}
		System.out.println(lab); 
		System.out.println(lab.visitedPositions.size()); 
	}

	

	public static void mainPart2(String inputfile) throws FileNotFoundException {
		
		Lab lab = new Lab();
		try (Scanner scanner = new Scanner(new File(inputfile))) {
			while (scanner.hasNext()) {
				String line = scanner.nextLine().trim();
				if (line.isBlank()) {
					continue;
				}
				lab.addRow(line);
			}
		}
		lab.findGuardPos();
		int countLoops = 0;
		for (int y=0; y<lab.maxY; y++) {
			for (int x=0; x<lab.maxX; x++) {
				if (lab.isFree(x,y)) {
					Lab testLab = new Lab(lab);
//					System.out.println("BEFORE "+new Pos(x,y)+": "+testLab);
					testLab.set(x,y,'#');
//					System.out.println("AFTER "+new Pos(x,y)+": "+testLab);
					while (testLab.tick()) {
					}
					if (testLab.isLoop()) {
						countLoops++;
					}
				}
			}
		}
		System.out.println("LOOPS: "+countLoops);
	}


	
	public static void main(String[] args) throws FileNotFoundException {
		System.out.println("--- PART I  ---");
//		mainPart1("exchange/day06/feri/input-example.txt");
		mainPart1("exchange/day06/feri/input.txt");     
		System.out.println("---------------");
		System.out.println();
		System.out.println("--- PART II ---");
//		mainPart2("exchange/day06/feri/input-example.txt");
		mainPart2("exchange/day06/feri/input.txt");
		System.out.println("---------------");
	}

	
}
