IFS=',' read -ra ranges <<< "$(cat input.txt)"
part1_count=0
part2_count=0
for range in "${ranges[@]}"; do
    IFS='-' read -ra bounds <<< "$range"
    lower=${bounds[0]}
    upper=${bounds[1]}

    for ((digit=$lower; digit<=$upper; digit++)); do
        n_digits=${#digit}

        for ((n_parts=2; n_parts<=n_digits; n_parts++)); do
            if (( n_digits % n_parts != 0 )); then
                continue
            fi

            digits_per_part=$((n_digits / n_parts))
            all_match=true
            for ((i=0; i<(n_digits-digits_per_part); i+=digits_per_part)); do
                first_part=${digit:i:digits_per_part}

                for ((j=i+digits_per_part; j<n_digits; j+=digits_per_part)); do
                    next_part=${digit:j:digits_per_part}

                    # 10# to force base 10 interpretation to avoid issues with leading zeros
                    if [[ (10#$first_part -ne 10#$next_part) || (10#$first_part -eq 0) ]]; then
                        all_match=false
                        break 2
                    fi
                done
            done

            if [[ "$all_match" == "true" ]]; then
                echo "MATCH $digit : $n_parts x $first_part"
                part2_count=$((part2_count + digit))
                if ((n_parts == 2)); then
                    part1_count=$((part1_count + digit))
                fi
                break
            fi

        done
    done
done
echo "Part 1: " $part1_count
echo "Part 2: " $part2_count