import numpy as np

with open("input.txt") as f:
    lines = f.read().splitlines()

# ===============================
# PART 1: simple maths
# ===============================

operators = lines[-1].split()

nums = np.array([[int(val) for val in line.split()] for line in lines[:-1]])

tot = 0
for i, operator in enumerate(operators):
    vals = nums[:, i]
    if operators == "*":
        tot += vals.prod()
    else:
        tot += vals.sum()

print("part 1: ", tot)

# ===============================
# PART 2
# ===============================

# extract indexes of operators (corresponds to start indexes of values to operate on)
idx = [i for i, x in enumerate(lines[-1]) if x in ["*", "+"]]

# also include end of input
# add 2 here because we subtract 1 later to skip blank columns between inputs
idx.append(len(lines[-1]) + 2)

# preserve column structure of input unlike in part 1 where we stripped whitespace
operands = np.array([list(line) for line in lines[:-1]])

tot = 0
for i, operator in enumerate(operators):
    # get the part of the input to apply this operator to
    vals = operands[:, idx[i] : idx[i + 1] - 1]  # the last column is empty
    if operator == "+":
        res = 0
    else:
        res = 1
    # numbers are now read top to bottom (i.e., by column)
    # have to extract each number from this part of the input
    for col in range(vals.shape[1]):
        num = int("".join(vals[:, col]))
        if operator == "+":
            res += num
        else:
            res *= num
    tot += res

print("part 2: ", tot)
