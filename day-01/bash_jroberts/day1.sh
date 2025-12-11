
CURRENT=50

PART1_COUNT=0
PART2_COUNT=0

while IFS= read -r line; do

    # Direction is first character, steps is the rest
    DIRECTION=${line:0:1}
    STEPS=${line:1}

    # Separate steps into hundreds and units, e.g. 256 -> 2, 56
    HUNDREDS=$((STEPS / 100))
    UNITS=$((STEPS % 100))
    PREVIOUS=$CURRENT

    # Increment the current position using the units only
    if [[ $DIRECTION == "L" ]]; then
        CURRENT=$((CURRENT - UNITS))
    elif [[ $DIRECTION == "R" ]]; then
        CURRENT=$((CURRENT + UNITS))
    fi

    # Check whether the units caused us to point at 0
    if [[ PREVIOUS -ne 0 && UNITS -gt 0 && ($CURRENT -le 0 || $CURRENT -ge 100) ]]; then
        PART2_COUNT=$((PART2_COUNT + 1))
    fi

    # Add the remaining passes by 0 due to hundreds
    PART2_COUNT=$((PART2_COUNT + HUNDREDS))

    # Reset current to be within 0-99
    CURRENT=$((CURRENT % 100))
    # deal with negative modulo allowed by bash
    if [[ $CURRENT -lt 0 ]]; then
        CURRENT=$((CURRENT + 100))
    fi

    # Check whether pointing at 0 at the end of the step
    if [[ $CURRENT -eq 0 ]]; then
        PART1_COUNT=$((PART1_COUNT + 1))
    fi

done < input.txt

echo "Part 1:" $PART1_COUNT
echo "Part 2:" $PART2_COUNT
