with open("input.txt") as f:
    id_pairs = f.read().split(",")


def is_invalid(id, n_repeats=2):
    """Check if ID repeats the same digit."""
    id_str = str(id)
    split = len(id_str) // n_repeats
    if n_repeats * id_str[:split] == id_str:
        return True
    return False


invalid_part1 = 0
invalid_part2 = 0
for id_pair in id_pairs:
    id_start, id_end = [int(id) for id in id_pair.split("-")]
    for id in range(id_start, id_end + 1):
        if is_invalid(id):
            invalid_part1 += id
            invalid_part2 += id
        else:
            for n_repeats in range(3, len(str(id)) + 1):
                if is_invalid(id, n_repeats):
                    invalid_part2 += id
                    break

print("part 1: ", invalid_part1)
print("part 2: ", invalid_part2)
