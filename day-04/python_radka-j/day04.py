with open("input.txt") as f:
    lines = f.read().splitlines()

grid = [list(line) for line in lines]


def go_up(row, col):
    return [row - 1, col]


def go_down(row, col):
    return [row + 1, col]


def go_right(row, col):
    return [row, col + 1]


def go_left(row, col):
    return [row, col - 1]


def go_up_right(row, col):
    return [row - 1, col + 1]


def go_up_left(row, col):
    return [row - 1, col - 1]


def go_down_right(row, col):
    return [row + 1, col + 1]


def go_down_left(row, col):
    return [row + 1, col - 1]


def count_neighbour_rolls(row, col):

    neighbour_roll_count = 0

    for direction in [
        go_down,
        go_up,
        go_right,
        go_left,
        go_down_right,
        go_down_left,
        go_up_right,
        go_up_left,
    ]:
        check_row, check_col = direction(row, col)
        if (
            check_row >= 0
            and check_row < len(grid)
            and check_col >= 0
            and check_col < len(grid[0])
        ):
            if grid[check_row][check_col] == "@":
                neighbour_roll_count += 1

    return neighbour_roll_count


accessible = 0
for i, row in enumerate(grid):
    for j, val in enumerate(row):
        if val == "@":
            if count_neighbour_rolls(i, j) < 4:
                accessible += 1

print("part 1: ", accessible)

# PART 2: same as above but repeat until there are no more rolls to remove
stop = False
accessible = 0
while not stop:
    removed_this_round = 0
    for i, row in enumerate(grid):
        for j, val in enumerate(row):
            if val == "@":
                if count_neighbour_rolls(i, j) < 4:
                    removed_this_round += 1
                    grid[i][j] = "."
    accessible += removed_this_round
    if removed_this_round == 0:
        stop = True

print("part 2: ", accessible)
