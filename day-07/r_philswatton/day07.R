input <- readLines("input.txt") |>
  strsplit("") |>
  Reduce(rbind, x=_)

# Convert input
m <- (input == "S") * 1
m[which(input == "^", arr.ind=TRUE)] <- NA

# Fn to iterate over rows, counting N beam in each slot
iterate_tachyon_beam <- function(m, i=1) {
  if (i == nrow(m)) return(m)
  m[i+1,][!is.na(m[i,])] <- (m[i+1,] + m[i,])[!is.na(m[i,])]
  if (sum(m[i+1,], na.rm=T) != sum(m[i,], na.rm=T)) {
    splitters <- which(is.na(m[i+1,]))
    splitters <- splitters[splitters %in% which(m[i,] > 0)]
    for (s in splitters) {
      m[i+1, s-1] <- m[i+1, s-1] + m[i, s]
      m[i+1, s+1] <- m[i+1, s+1] + m[i, s]
    }
  }
  Tailcall(iterate_tachyon_beam, m, i+1)
}
out <- iterate_tachyon_beam(m)

# Part 1: count splitters w/ a beam above it
splitter_inds <- which(is.na(out), arr.ind=TRUE)
splitter_inds[,1] <- splitter_inds[,1] - 1
part1 <- sum(out[splitter_inds] > 0)

# Part 2: count N beams at the end
part2 <- out[nrow(out),] |> sum() |> as.character()

# Out
cat("Part 1: ", as.character(part1), "\nPart 2: ", as.character(part2))





