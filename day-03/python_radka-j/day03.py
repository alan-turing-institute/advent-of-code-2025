with open("input.txt") as f:
    lines = f.read().splitlines()

# =======================================================
# PART 1
# =======================================================

tot_joltage = 0
for bank in lines:
    vals = list(bank)

    # get the largest digit in list excluding last item
    first_val = max(vals[:-1])

    # get the largest digit in the remainder of list
    first_val_idx = vals.index(first_val)
    second_val = max(vals[first_val_idx + 1 :])

    # add to joltage
    joltage = int("".join([first_val, second_val]))
    tot_joltage += joltage

print("part 1: ", tot_joltage)

# =======================================================
# PART 2
# =======================================================

tot_joltage = 0
for bank in lines:
    vals = list(bank)

    # similar to above but search for 12 digits
    # use two pointers to scan list and search for max vals within range
    best_vals = []
    start_idx, end_idx = 0, len(vals) - 11
    for i in range(12):
        max_val = max(vals[start_idx : end_idx + i])
        best_vals.append(max_val)
        max_val_idx = vals[start_idx : end_idx + i].index(max_val)
        start_idx += max_val_idx + 1
    tot_joltage += int("".join(best_vals))

print("part 2: ", tot_joltage)
