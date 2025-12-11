input <- readLines("input.txt") |>
  strsplit(": ")
names(input) <- sapply(input, {\(x) x[1]})
input <- lapply(input, {\(x) strsplit(x[2], " ")[[1]]})

# Part 1
count_paths <- function(input, curr="you", counter=0) {
  next_paths <- input[[curr]]
  if (length(next_paths) == 1) if (next_paths == "out") return(counter+1)
  next_paths <- next_paths[next_paths != "you"]
  return(sum(sapply(next_paths, {\(x) Tailcall(count_paths, input, x, counter)})))
}
part1 <- count_paths(input)


# Part 2
library(memoise)
count_paths <- function(input, curr="svr", passed=c(0,0), counter=0) {
  if (curr == "dac") passed[1] <- 1
  if (curr == "fft") passed[2] <- 1
  next_paths <- input[[curr]]
  if (length(next_paths) == 1) if (next_paths == "out") return(counter + Reduce(`&`, passed))
  next_paths <- next_paths[next_paths != "svr"]
  return(sum(sapply(next_paths, {\(x) Tailcall(count_paths, input, x, passed, counter)})))
}
count_paths <- memoise(count_paths)
part2 <- count_paths(input)

# Out
cat("Part 1: ", part1, "\nPart 2: ", as.character(part2))
