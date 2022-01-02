import sys
from pathlib import Path
from typing import Iterable, List


def main(argv):
    if len(argv) < 2:
        print("Please provide the input file as argument", file=sys.stderr)
    file_content = Path(argv[1]).read_text().strip()
    location_matrix = parse_location_matrix(file_content)
    total = 0
    for i, row in enumerate(location_matrix):
        for j, point in enumerate(row):
            if point < min(neighbors(location_matrix, i, j)):
                total += risk_level(point)
    print(total)


def parse_location_matrix(str_data: str) -> List[List[int]]:
    return [[int(c) for c in line] for line in str_data.split("\n")]


def neighbors(location_matrix: List[List[int]], i: int, j: int) -> Iterable[int]:
    rst: List[int] = []
    if j > 0:
        rst.append(location_matrix[i][j - 1])
    if i > 0:
        rst.append(location_matrix[i - 1][j])
    if j < len(location_matrix[0]) - 1:
        rst.append(location_matrix[i][j + 1])
    if i < (len(location_matrix) - 1):
        rst.append(location_matrix[i + 1][j])
    return rst


def risk_level(point: int) -> int:
    return point + 1


if __name__ == "__main__":
    main(sys.argv)
