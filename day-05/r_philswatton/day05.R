input <- readLines("input.txt")
sep_index <- which(input == "")
id_ranges <- input[1:(sep_index - 1)] |>
  sapply({\(x) strsplit(x, "-")[[1]] |> as.numeric()}) |>
  t()
ids <- input[(sep_index + 1):length(input)] |> as.numeric()

# Part 1
id_is_fresh <- function(id, ranges) any(id >= ranges[,1] & id <= ranges[,2])
part1 <- sum(ids * sapply(ids, function(x) id_is_fresh(x, id_ranges)) > 0)

# Part 2
sorted_ranges <- id_ranges[order(id_ranges[,1], id_ranges[,2]),]
count_possible_fresh <- function(ranges, i=1, prev_end=0, count=0) {
  if (i > nrow(ranges)) return(count)
  if (ranges[i,2] > prev_end) count <- count + ranges[i,2] - max(ranges[i,1] - 1, prev_end)
  prev_end <- max(ranges[i,2], prev_end)
  Tailcall(count_possible_fresh, ranges, i+1, prev_end, count)
}
part2 <- count_possible_fresh(sorted_ranges)

# Out
cat("Part 1: ", part1, "\nPart 2: ", as.character(part2))


