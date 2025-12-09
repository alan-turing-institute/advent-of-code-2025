input <- readLines("input.txt") |>
  strsplit(",") |>
  Reduce(rbind, x=_) |>
  apply(2, as.numeric)

# Fn for computing area of rectangles given two matrices of corners
compute_rect_area <- function(a, b) (abs(a - b) + 1) |> apply(1, prod)

# Prepare pairs of inputs and compute rectangle areas
n <- nrow(input)
m <- matrix(numeric(n*n), nrow=n, ncol=n)
pairs <- which(lower.tri(m), arr.ind=TRUE)
rectangle_areas <- compute_rect_area(input[pairs[,1],], input[pairs[,2],])

# Part 1 Answer
part1 <- max(rectangle_areas)

# Fn which given two corners, produces a line
make_line <- function(input, i, offset=1) {
  c(input[i,1], input[i,2], input[i+offset,1], input[i+offset,2])
}

# Fn which retrieves all lines from the input
get_lines <- function(input, i=1, out=NULL) {
  if (i==1) out <- make_line(input, i)
  else if (i==nrow(input)) {
    out <- rbind(out, make_line(input, i, 1-nrow(input)))
    return(out)
  }
  else out <- rbind(out, make_line(input, i))
  Tailcall(get_lines, input, i+1, out)
}

# Retrieve lines
lines <- get_lines(input)

# Fn which checks if a rectangle contains a line
check_intersection <- function(a, b) {
  coords <- cbind(a, b)
  apply(coords, 1, function(x) {
    rect_left <- min(x[c(1,3)])
    rect_right <- max(x[c(1,3)])
    rect_bottom <- min(x[c(2,4)])
    rect_top <- max(x[c(2,4)])
    all(
      (lines[,1] >= rect_right & lines[,3] >= rect_right) |
      (lines[,1] <= rect_left & lines[,3] <= rect_left) |
      (lines[,2] >= rect_top & lines[,4] >= rect_top) |
      (lines[,2] <= rect_bottom & lines[,4] <= rect_bottom)
    )
  })
}

# Part 2 answer
valid_rects <- check_intersection(input[pairs[,1],], input[pairs[,2],])
part2 <- rectangle_areas[valid_rects] |> max()

# Out
cat("Part 1: ", part1, "\nPart 2: ", part2)
