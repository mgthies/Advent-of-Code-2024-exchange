"""Advent of Code 2024, Tag 6
Lösung von Matthias Baake

Pylint und mypy sind zufrieden :-)
"""

import datetime
from copy import deepcopy
from pathlib import Path

DIRECTORY = Path("2024")
PATH1 = DIRECTORY / Path("data_06-example.txt")
PATH2 = DIRECTORY / Path("data_06-matthias-baake.txt")

PATH = PATH2


def read_file(path: Path) -> tuple[int, int, list[str]]:
    """Datei einlesen

    Die Markierung für die Startposition wird durch X ersetzt.

    Args:
        path: Path der einzulesenden Datei

    Returns:
        X-Position der Wache
        Y-Position der Wache
        Karte als Liste von str
    """
    pos_x = None
    pos_y = None
    with open(path, encoding="utf-8") as file:
        area_l: list[str] = []
        for line in file:
            curr_line = line.rstrip()
            if (x := line.find("^")) >= 0:
                pos_x = x
                pos_y = len(area_l)
                curr_line = curr_line[:pos_x] + "X" + curr_line[pos_x + 1 :]
            area_l.append(curr_line)
    assert pos_x is not None and pos_y is not None

    return pos_x, pos_y, area_l


def move_around(pos_x: int, pos_y: int, area_l: list[str]) -> tuple[list[str], bool]:
    """Wache bewegen, bis Ende erreicht

    Das Ende wurde erreicht, wenn der Bereich der Karte verlassen
    oder eine Schleife erkannt wurde.

    Args:
        pos_x: X-Position der Wache
        pos_y: Y-Position der Wache
        area_l: Karte des Bereichs

    Returns:
        Karte, in der alle Positionen, die die Wache betreten hat, mit X
            markiert sind
        Flag, ob eine Schleife erkannt wurde
    """
    width = len(area_l[0])
    height = len(area_l)
    direction = "N"
    move_d = {
        "N": (0, -1, "E"),
        "S": (0, 1, "W"),
        "W": (-1, 0, "N"),
        "E": (1, 0, "S"),
    }
    states_s = set()

    while True:
        dx, dy, next_dir = move_d[direction]
        new_x = pos_x + dx
        new_y = pos_y + dy

        if not (0 <= new_x < width and 0 <= new_y < height):
            # Bereich verlassen
            return area_l, False

        new_char = area_l[new_y][new_x]
        if new_char in ".X":  # Weg ist frei
            # Weitergehen
            pos_x = new_x
            pos_y = new_y
            area_l[new_y] = area_l[new_y][:new_x] + "X" + area_l[new_y][new_x + 1 :]

            state = (pos_x, pos_y, direction)
            if state in states_s:
                # Schleife erkannt
                return area_l, True
            states_s.add(state)
        elif new_char == "#":  # Weg ist blockiert
            # Nicht weitergehen, nach rechts drehen
            direction = next_dir


# Teilaufgabe 1
def task1(pos_x: int, pos_y: int, area_l: list[str]) -> None:
    """Lösung für Teilaufgabe 1 ermitteln und ausgeben

    Args:
        pos_x: X-Position der Wache
        pos_y: Y-Position der Wache
        area_l: Karte des Bereichs
    """
    new_area_l, _ = move_around(pos_x, pos_y, deepcopy(area_l))

    pos_visited = sum(
        map(
            lambda line: sum(map(lambda char: 1 if char == "X" else 0, line)),
            new_area_l,
        )
    )
    print(f"Ergebnis Teilaufgabe 1 für {PATH.name}: {pos_visited}")
    # PATH1: 41
    # PATH2: 4776


# Teilaufgabe 2
def task2(pos_x: int, pos_y: int, area_l: list[str]) -> None:
    """Lösung für Teilaufgabe 2 ermitteln und ausgeben

    Args:
        pos_x: X-Position der Wache
        pos_y: Y-Position der Wache
        area_l: Karte des Bereichs
    """
    width = len(area_l[0])
    height = len(area_l)
    num_possible = 0
    for obs_x in range(width):
        for obs_y in range(height):
            if area_l[obs_y][obs_x] != ".":
                # Startposition der Wache (X) oder Blockierung (#) an
                # dieser Stelle vorhanden
                pass
            else:
                new_area_l = deepcopy(area_l)
                new_area_l[obs_y] = (
                    new_area_l[obs_y][:obs_x] + "#" + new_area_l[obs_y][obs_x + 1 :]
                )
                _, loop_detected = move_around(pos_x, pos_y, new_area_l)
                if loop_detected:
                    num_possible += 1
    print(f"Ergebnis Teilaufgabe 2 für {PATH.name}: {num_possible}")
    # PATH1: 6
    # PATH2: 1586


def main() -> None:
    """Datei einlesen und beide Teilaufgaben lösen"""
    start_dt = datetime.datetime.now()

    pos_x, pos_y, area_l = read_file(PATH)
    task1(pos_x, pos_y, area_l)
    end1_dt = datetime.datetime.now()

    task2(pos_x, pos_y, area_l)
    end2_dt = datetime.datetime.now()

    print(
        "Zeit für das Einlesen der Datei und Teilaufgabe 1: "
        f"{(end1_dt-start_dt).total_seconds()} Sekunden"
    )
    # PATH1: 0.0 Sekunden
    # PATH2: 0.0 Sekunden
    print(f"Zeit für Teilaufgabe 2: {(end2_dt-end1_dt).total_seconds()} Sekunden")
    # PATH1: 0.0 Sekunden
    # PATH2: 54.8 Sekunden
    print(f"Gesamtzeit: {(end2_dt-start_dt).total_seconds()} Sekunden")
    # PATH1: 0.0 Sekunden
    # PATH2: 54.8 Sekunden


if __name__ == "__main__":
    main()
