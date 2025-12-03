input <- readLines("input.txt") |> 
  lapply(function(x) strsplit(x, "")[[1]] |> as.numeric())

get_nth_index <- function(s, n, ndigits, prev_index=NULL) {
  start <- max(1, prev_index + 1)
  which.max(s[start:(length(s) + n - ndigits)]) + max(0, prev_index)
}

get_n_digit_joltage <- function(s, ndigits, n=1, indices=numeric(0)) {
  indices <- c(indices, get_nth_index(s, n, ndigits, indices[length(indices)]))
  remainder <- s[(indices[length(indices)] + 1):length(s)]
  if (length(indices) + length(remainder) == ndigits) return(c(s[indices], remainder))
  if (length(indices) == ndigits) return(s[indices])
  Tailcall(get_n_digit_joltage, s, ndigits, n+1, indices)
}

solve_day_3 <- function(input, ndigit) {
  sapply(input, function(x) get_n_digit_joltage(x, ndigit) |> paste0(collapse="") |> as.numeric()) |> sum()
}

solution <- c(solve_day_3(input, 2), solve_day_3(input, 12)) |> as.character()
cat("Part 1: ", solution[1], "\nPart 2: ", solution[2])




