input <- readLines("test.txt") |> {\(x) strsplit(x, ",")[[1]]}()

get_ids_from_range <- function(id_range) {
  strsplit(id_range, "-")[[1]] |> {\(x) x[1]:x[2]}()
}

check_invalid <- function(id, n_repeats) {
  seq <- substr(id, 1, nchar(id) %/% n_repeats)
  return(paste0(rep(seq, n_repeats), collapse="") == id)
}

get_ids_to_add <- function(id, out) {
  if (check_invalid(id, 2)) return(c(id, id))
  if (nchar(id) > 2) {
    for (n_repeat in 3:nchar(id)) if(check_invalid(id, n_repeat)) return(c(0, id))
  }
  return(c(0,0))
}

solve_day_2 <- function(input, out) {
  if (length(input) == 0) return(out)
  ids <- get_ids_from_range(input[[1]])
  lapply(ids, function(x) update_out(x, out))
  out <- out + sapply(ids, function(x) get_ids_to_add(x, out)) |> rowSums()
  Tailcall(solve_day_2, input[-1], out)
}

solution <- solve_day_2(inputs, c(0,0))
cat("Part 1: ", solution[1], "\nPart 2: ", solution[2])




