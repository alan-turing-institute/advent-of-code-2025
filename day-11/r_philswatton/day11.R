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
library(cachem)

count_paths <- function(input, curr="svr", passed=c(0,0), counter=0) {
  if (curr == "dac") passed[1] <- 1
  if (curr == "fft") passed[2] <- 1
  next_paths <- input[[curr]]
  if (length(next_paths) == 1) if (next_paths == "out") return(counter + Reduce(`&`, passed))
  next_paths <- next_paths[next_paths != "svr"]
  return(sum(sapply(next_paths, {\(x) Tailcall(count_paths, input, x, passed, counter)})))
}

memo <- function(fn) {
  inner_env <- new.env(parent = parent.frame())
  inner_env$`_fn` <- fn
  cache <- cachem::cache_mem(max_size=1024 * 1024)
  return(function(...) {
    mc <- match.call()
    call <- lapply(as.list(mc[-1]), eval, parent.frame())
    key <- rlang::hash(call)
    res <- cache$get(key)
    if (inherits(res, "key_missing")) {
      mc[[1]] <- inner_env$`_fn`
      res <- eval(mc, parent.frame())
    }
    cache$set(key, res)
    return(res)
  })
}
count_paths <- memo(count_paths)

part2 <- count_paths(input)

# Out
cat("Part 1: ", part1, "\nPart 2: ", as.character(part2))

