import sys

import numpy as np
import matplotlib.pyplot as plt

def main():
    bed_path_1 = sys.argv[1]
    bed_path_2 = sys.argv[2]

    variants = {}
    for line in open(bed_path_1):
        line = line.strip('\n').split('\t')
        idx, count = line[3], int(line[-1])
        variants[idx] = (count, -1)

    for line in open(bed_path_2):
        line = line.strip('\n').split('\t')
        idx, count = line[3], int(line[-1])
        if idx in variants:
            variants[idx] = (variants[idx][0], count)
        else:
            variants[idx] = (-1, count)

    none = 0
    both = 0
    only1 = 0
    only2 = 0
    total = 0
    for (c1,c2) in variants.values():
        total += 1
        if c1 <= 0 and c2 <= 0:
            none += 1
        elif c1 > 0 and c2 > 0:
            both += 1
        elif c1 > 0 and c2 <= 0:
            only1 += 1
        elif c1 <= 0 and c2 > 0:
            only2 += 1

    print("None", "Both", "1", "2", "Total", sep=',')
    print(none, both, only1, only2, total, sep=',')

if __name__ == "__main__":
    main()
