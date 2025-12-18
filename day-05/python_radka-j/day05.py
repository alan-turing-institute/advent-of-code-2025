# =============================
# READ AND PROCESS DATA
# =============================

with open("input.txt") as f:
    lines = f.read().splitlines()

# split data into ranges and ingredient IDs
split_idx = 171

ranges = []
for line in lines[:split_idx]:
    low, high = line.split("-")
    ranges.append([int(low), int(high)])

ingredients = [int(val) for val in lines[split_idx + 1 :]]

# sort and merge overlapping ranges
ranges = sorted(ranges)
merged_ranges = [ranges[0]]
for curr_range in ranges[1:]:
    prev_range = merged_ranges[-1]
    if prev_range[-1] >= curr_range[0]:
        merged_ranges.pop(-1)
        merged_ranges.append([prev_range[0], max([prev_range[-1], curr_range[1]])])
    else:
        merged_ranges.append(curr_range)

# =============================
# PART 1
# =============================

fresh_count = 0
for ingredient in ingredients:
    for range in merged_ranges:
        low, high = range
        if ingredient >= low and ingredient <= high:
            fresh_count += 1
            break

print("part 1: ", fresh_count)

# =============================
# PART 2
# =============================

fresh_count = 0
for range in merged_ranges:
    low, high = range
    fresh_count += high - low + 1
print("part 2: ", fresh_count)
