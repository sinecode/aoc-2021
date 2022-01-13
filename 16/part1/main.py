import sys
from math import log2
from pathlib import Path


class BinIterator:
    "Used to iterate on the binary string"

    def __init__(self, bin_content):
        self.current_index = 0
        self.bin_content = bin_content

    def peek(self, num_bits: int) -> str:
        return self.bin_content[self.current_index:self.current_index + num_bits]

    def next(self, num_bits: int) -> str:
        out = self.bin_content[self.current_index:self.current_index + num_bits]
        self.current_index += num_bits
        return out

    def has_next(self):
        return self.current_index < len(self.bin_content)

class Packet:
    def __init__(self, version, type_id):
        self.version = version
        self.type_id = type_id

    def __str__(self):
        return f"{self.version=} {self.type_id=} {self.value=}"


class Operator:
    def __init__(self, version, bin_iterator):
        super().__init__(version, 4)
        self.subpackets = []


class Literal(Packet):
    def __init__(self, version, bin_iterator):
        super().__init__(version, 4)
        self.value = self._parse_value(bin_iterator)

    def _parse_value(self, bin_iterator):
        done_last_group = False
        bin_groups = []
        while not done_last_group:
            if bin_iterator.next(1) == '0':
                done_last_group = True
            bin_groups.append(bin_iterator.next(4))
        while bin_iterator.has_next() and bin_iterator.next(1) != '1':
            pass
        return int(''.join(bin_groups), 2)


def main(argv):
    if len(argv) < 2:
        print("Please provide the input file as argument", file=sys.stderr)
    file_content = Path(argv[1]).read_text().strip()
    bin_iterator = BinIterator(bin(int(file_content, 16))[2:].zfill(num_bits_in_hex_content(file_content)))

    parse(bin_iterator)
    print(packets)


def num_bits_in_hex_content(hex_content: str) -> int:
    return int(len(hex_content) * log2(16))

def parse(bin_iterator) -> Packet:
   version = int(bin_iterator.next(3), 2)
   type_id = int(bin_iterator.next(3), 2)
   if type_id == 4:
       packets.append(Literal(version, bin_iterator))
   else:
       if bin_iterator.next(1) == '0':
           bits_subpackets = bin_iterator.next(15)
           print(bits_subpackets)
           num_bits_subpackets = int(bits_subpackets, 2)
           print(f"{num_bits_subpackets=}")
           parse()
       else:
            ...




if __name__ == "__main__":
    main(sys.argv)
