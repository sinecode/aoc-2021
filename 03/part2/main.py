import sys
from pathlib import Path


def compute_rating(binary_numbers, most_frequent=True):
    if len(binary_numbers) == 0:
        raise ValueError("There must be at least one binary number")
    num_bits = len(binary_numbers[0])
    rating = ""
    binary_numbers_left = [b for b in binary_numbers]
    for i in range(num_bits):
        rating = binary_numbers_left[0]
        if len(binary_numbers_left) == 1:
            break
        counter = {"0": 0, "1": 0}
        for binary_number in binary_numbers_left:
            counter[binary_number[i]] += 1
        if most_frequent:
            bit_criteria = "0" if counter["0"] > counter["1"] else "1"
        else:
            bit_criteria = "1" if counter["0"] > counter["1"] else "0"
        binary_numbers_left = [
            b for b in binary_numbers_left if b[i] == bit_criteria
        ]
        rating = binary_numbers_left[0]
    return rating


def main():
    input_file = sys.argv[1]
    content = Path(input_file).read_text()
    binary_numbers = content.strip().split()
    oxygen_generator_rating = compute_rating(
        binary_numbers, most_frequent=True
    )
    co2_scrubber_rating = compute_rating(binary_numbers, most_frequent=False)
    print(int(oxygen_generator_rating, 2) * int(co2_scrubber_rating, 2))


if __name__ == "__main__":
    main()
