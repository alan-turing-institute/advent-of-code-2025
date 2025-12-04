input <- readLines("input.txt") |>
  strsplit("") |>
  Reduce(rbind, x=_)

pad <- function(x) rbind(0, cbind(0, x, 0), 0)

shift <- expand.grid(row=-1:1, col=-1:1)[-5,]

sum_neighbours <- function(m) {
  lapply(1:8, function(i) m[2:(nrow(m) -1) + shift[i,1], 2:(ncol(m) -1) + shift[i,2]]) |>
    Reduce(`+`, x=_)
}

find_update_grid <- function(input, iter=1, out=c(0,0)) {
  m <- (input == "@") * 1
  accessible <- sum_neighbours(pad(m)) < 4 & m == 1
  count <- sum(accessible)
  indices <- which(accessible, arr.ind=TRUE)
  if (!any(accessible)) return(out)
  if (iter == 1) out <- out + count
  if (iter > 1) out[2] <- out[2] + count
  if (count > 1) input[indices] <- "." else input[indices[1], indices[2]] <- "."
  Tailcall(find_update_grid, input, iter+1, out)
}

solution <- find_update_grid(input)
cat("Part 1: ", solution[1], "\nPart 2: ", solution[2])




