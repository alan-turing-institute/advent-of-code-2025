
current=50

part1_count=0
part2_count=0

while IFS= read -r line; do

    # direction is first character, steps is the rest
    direction=${line:0:1}
    steps=${line:1}

    # Separate steps into hundreds and units, e.g. 256 -> 2, 56
    hundreds=$((steps / 100))
    units=$((steps % 100))
    previous=$current

    # Increment the current position using the units only
    if [[ $direction == "L" ]]; then
        current=$((current - units))
    elif [[ $direction == "R" ]]; then
        current=$((current + units))
    fi

    # Check whether the units caused us to point at 0
    if [[ previous -ne 0 && units -gt 0 && ($current -le 0 || $current -ge 100) ]]; then
        part2_count=$((part2_count + 1))
    fi

    # Add the remaining passes by 0 due to hundreds
    part2_count=$((part2_count + hundreds))

    # Reset current to be within 0-99
    current=$((current % 100))
    # deal with negative modulo allowed by bash
    if [[ $current -lt 0 ]]; then
        current=$((current + 100))
    fi

    # Check whether pointing at 0 at the end of the step
    if [[ $current -eq 0 ]]; then
        part1_count=$((part1_count + 1))
    fi

done < input.txt

echo "Part 1:" $part1_count
echo "Part 2:" $part2_count
