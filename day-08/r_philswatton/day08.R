input <- readLines("input.txt") |>
  strsplit(",") |>
  Reduce(rbind, x=_)
rownames(input) <- NULL

# Make distance matrix and get order of pairs from closest to most distant
d <- dist(input)
o <- order(d)

# Fn for returning row ids for pair
make_which <- function(n) {
  m <- matrix(1:n, n, n)
  idx <- which(lower.tri(m), arr.ind = TRUE)
  function(k) idx[k,]
}
which_dist <- make_which(nrow(input))

# Fn which runs for `niter` iterations, producing groups
find_group <- function(groups, value) sapply(groups, {\(x) value %in% x})
iterate_group <- function(groups, o, niter=NULL, i=1) {
  if (!is.null(niter)) if (i > niter) return(groups)
  pair <- which_dist(o[1])
  group_idx <- c(which(find_group(groups, pair[1])), which(find_group(groups, pair[2])))
  new_group <- unique(unlist(groups[group_idx]))
  groups <- groups[-group_idx]
  groups[[length(groups) + 1]] <- new_group
  if (is.null(niter)) if (length(groups) == 1) return(pair)
  Tailcall(iterate_group, groups, o[-1], niter, i+1)
}

# Part 1
part1 <- iterate_group(as.list(1:nrow(input)), o, 1000) |>
  sapply(length) |>
  sort(TRUE) |>
  {\(x) prod(x[1:3])}()

# Part 2
part2 <- input[iterate_group(as.list(1:nrow(input)), o)] |>
  as.numeric() |>
  prod()

# Out
cat("Part 1: ", part1, "\nPart 2: ", part2)



