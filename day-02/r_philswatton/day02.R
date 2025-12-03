input <- readLines("input.txt") |> {\(x) strsplit(x, ",")[[1]]}()

get_ids_from_range <- function(id_range) {
  strsplit(id_range, "-")[[1]] |> {\(x) x[1]:x[2]}()
}

check_invalid <- function(id, n_repeats) {
  seq <- substr(id, 1, nchar(id) %/% n_repeats)
  return(paste0(rep(seq, n_repeats), collapse="") == id)
}

get_ids_to_add <- function(id) {
  if (check_invalid(id, 2)) return(c(id, id))
  if (nchar(id) > 2) {
    for (n_repeat in 3:nchar(id)) if(check_invalid(id, n_repeat)) return(c(0, id))
  }
  return(c(0,0))
}

solve_day_2 <- function(id_range) {
  ids <- get_ids_from_range(id_range)
  sapply(ids, function(x) get_ids_to_add(x)) |> rowSums()
}

solution <- lapply(input, solve_day_2) |> Reduce(`+`, x=_)
cat("Part 1: ", solution[1], "\nPart 2: ", solution[2])




