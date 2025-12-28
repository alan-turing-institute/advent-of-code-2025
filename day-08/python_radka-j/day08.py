import numpy as np
from scipy.spatial.distance import cdist

with open("input.txt") as f:
    lines = f.read().splitlines()

# create numpy array of all 3D positions
boxes = []
for line in lines:
    boxes.append(np.array([int(val) for val in line.split(",")]))
boxes_np = np.array(boxes)

# compute all pairwise distances (set diagonal to inf)
distances = cdist(boxes_np, boxes_np)
np.fill_diagonal(distances, np.inf)

# track how many and which boxes are connected to each other in a circuit
visited = []
n_connected = 0
circuits: list[list] = []

# ======================================================
# PART 2: continue until all boxes form a single circuit
# ======================================================
while True:
    n_connected += 1

    sizes = [len(circuit) for circuit in circuits]

    # stop if all boxes are connected
    if len(sizes) > 0:
        if max(sizes) == len(boxes):
            break

    # ======================================================
    # PART 1
    # once make the first 1000 connections, get top 3 circuit sizes
    # ======================================================
    if n_connected == 1000:
        total = 1
        for n in sorted(sizes)[-3:]:
            total *= n
        print("part 1: ", total)

    # get minimum available distance and remove from options
    box1, box2 = np.unravel_index(np.argmin(distances), distances.shape)
    distances[box1, box2] = np.inf
    distances[box2, box1] = np.inf

    # option 1: start a new circuit
    if box1 not in visited and box2 not in visited:
        circuits.append([box1, box2])
        visited.append(box1)
        visited.append(box2)

    # option 2: both boxes are already part of a circuit - connect them
    elif box1 in visited and box2 in visited:
        for i, circuit in enumerate(circuits):
            if box1 in circuit:
                idx1 = i
            if box2 in circuit:
                idx2 = i
        if idx1 == idx2:
            continue
        else:
            # join the two existing circuits
            joined_circuit = circuits[idx1] + circuits[idx2]
            if idx1 > idx2:
                circuits.pop(idx1)
                circuits.pop(idx2)
            else:
                circuits.pop(idx2)
                circuits.pop(idx1)
            circuits.append(joined_circuit)

    # option 3: append the unconnected box to an existing circuit
    else:
        for circuit in circuits:
            if box1 in circuit and box2 not in circuit:
                circuit.append(box2)
                visited.append(box2)
                break
            elif box1 not in circuit and box2 in circuit:
                circuit.append(box1)
                visited.append(box1)
                break

x1, _, _ = boxes[box1]
x2, _, _ = boxes[box2]
print("part 2: ", x1 * x2)
