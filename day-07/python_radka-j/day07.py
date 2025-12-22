from collections import defaultdict

with open("input.txt") as f:
    lines = f.read().splitlines()

start_idx = lines[0].index("S")

# ==========================================
# PART 1
# ==========================================

# track positions of active beams
beam_locations = [start_idx]

n_splits = 0
for line in lines[1:]:

    # count number of beam splits
    beams_to_split = []
    for beam_loc in beam_locations:
        if line[beam_loc] == "^":
            beams_to_split.append(beam_loc)
    n_splits += len(beams_to_split)

    # update active beam positions after splits
    for beam_loc in beams_to_split:
        beam_locations.remove(beam_loc)
        if beam_loc - 1 >= 0:
            beam_locations.append(beam_loc - 1)
        if beam_loc + 1 < len(line):
            beam_locations.append(beam_loc + 1)
    beam_locations = list(set(beam_locations))

print("part 1: ", n_splits)

# ==========================================
# PART 2
#
# NOTE: DFS to count number of possible paths is too slow
#  ==========================================

# keep track of active beams and number of ways to get there
beams = defaultdict(int)

# start with a single timeline at start position
beams[start_idx] = 1

for line in lines[1:]:
    # keep active beams to add/remove after splits
    beams_to_add = defaultdict(int)
    beams_to_remove = []

    for beam_loc, count in beams.items():
        # if have a split, maintain number of possible paths to get to each beam
        if line[beam_loc] == "^":
            if beam_loc - 1 >= 0:
                beams_to_add[beam_loc - 1] += count
            if beam_loc + 1 < len(line):
                beams_to_add[beam_loc + 1] += count
            beams_to_remove.append(beam_loc)

    # update active beam positions and number of timelines to each after splits
    for beam, count in beams_to_add.items():
        beams[beam] += count
    for beam in beams_to_remove:
        beams[beam] = 0


print("part 2: ", sum(beams.values()))
