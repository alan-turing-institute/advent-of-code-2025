input <- readLines("input.txt")
ops <- strsplit(input[length(input)], " +", perl=TRUE)[[1]]

# Fn which applies the ith op to the ith ele of a list and returns the sum
apply_ops_to_rows <- function(numbers, ops) {
  sapply(1:length(numbers), {\(i) Reduce(as.name(ops[i]), numbers[[i]])}) |> sum()
}

# Part 1
numbers_part1 <- input[1:(length(input)-1)] |>
  strsplit(" +", perl=TRUE) |>
  sapply({\(x) as.numeric(x[x != ""])})
numbers_part1 <- split(numbers_part1, row(numbers_part1))
part1 <- apply_ops_to_rows(numbers_part1, ops)

# Part 2
input_t <- input |>
  strsplit("") |>
  Reduce(rbind, x=_) |>
  t()
numbers_vec <- apply(input_t[,-ncol(input_t)], 1, {\(x) paste0(x, collapse="")}) |>
  as.numeric()
numbers_part2 <- split(numbers_vec[!is.na(numbers_vec)], cumsum(is.na(numbers_vec))[!is.na(numbers_vec)])
part2 <- apply_ops_to_rows(numbers_part2, ops)

# Out
cat("Part 1: ", as.character(part1), "\nPart 2: ", as.character(part2))
