with open("input.txt") as f:
    lines = f.read().splitlines()

curr_val = 50
n_stop_at_zero = 0
n_passed_zero = 0

for line in lines:
    direction = line[0]
    clicks = int(line[1:])

    for i in range(clicks):
        if direction == "R":
            curr_val += 1
        elif direction == "L":
            curr_val -= 1
        curr_val %= 100
        if curr_val == 0:
            n_passed_zero += 1

    if curr_val == 0:
        n_stop_at_zero += 1

print("part 1: ", n_stop_at_zero)
print("part 2: ", n_passed_zero)
