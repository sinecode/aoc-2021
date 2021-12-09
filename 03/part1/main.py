import sys
from pathlib import Path


def main():
    input_file = sys.argv[1]
    content = Path(input_file).read_text()
    binary_numbers = content.strip().split()
    if len(binary_numbers) == 0:
        print(0)
        return
    gamma_rate = ""
    epsilon_rate = ""
    num_bits = len(binary_numbers[0])
    for i in range(num_bits):
        counter = 0
        for binary_number in binary_numbers:
            if binary_number[i] == "0":
                counter -= 1
            else:
                counter += 1
        if counter < 0:
            gamma_rate += "0"
            epsilon_rate += "1"
        else:
            gamma_rate += "1"
            epsilon_rate += "0"
    print(int(gamma_rate, 2) * int(epsilon_rate, 2))


if __name__ == "__main__":
    main()
