import sys
from math import prod
from pathlib import Path
from typing import Dict, Iterable, List


def main(argv):
    if len(argv) < 2:
        print("Please provide the input file as argument", file=sys.stderr)
    file_content = Path(argv[1]).read_text().strip()
    location_matrix = parse_location_matrix(file_content)
    basins: Dict[Point, int] = {}
    for row in location_matrix:
        for point in row:
            if point.value == 9:
                # location with 9 as value do not count as being in any basin.
                continue
            low_point = find_nearest_low_point(point, location_matrix)
            if low_point in basins:
                basins[low_point] += 1
            else:
                basins[low_point] = 1
    print(prod(sorted(basins.values(), reverse=True)[:3]))


class Point:
    def __init__(self, x: int, y: int, value: int):
        self.x = x
        self.y = y
        self.value = value

    def __hash__(self) -> int:
        return hash(f"({self.x}, {self.y})")

    def __repr__(self) -> str:
        return f"Point({self.x}, {self.y}, {self.value})"


def parse_location_matrix(str_data: str) -> List[List[Point]]:
    return [
        [Point(i, j, int(value)) for j, value in enumerate(line)]
        for i, line in enumerate(str_data.split("\n"))
    ]


def find_nearest_low_point(point: Point, location_matrix: List[List[Point]]) -> Point:
    while True:
        lowest_neighbor = min(neighbors(location_matrix, point), key=lambda p: p.value)
        if point.value <= lowest_neighbor.value:
            break
        else:
            point = lowest_neighbor
    return point


def neighbors(location_matrix: List[List[Point]], point: Point) -> Iterable[Point]:
    rst: List[Point] = []
    if point.y > 0:
        rst.append(location_matrix[point.x][point.y - 1])
    if point.x > 0:
        rst.append(location_matrix[point.x - 1][point.y])
    if point.y < len(location_matrix[0]) - 1:
        rst.append(location_matrix[point.x][point.y + 1])
    if point.x < (len(location_matrix) - 1):
        rst.append(location_matrix[point.x + 1][point.y])
    return rst


if __name__ == "__main__":
    main(sys.argv)
