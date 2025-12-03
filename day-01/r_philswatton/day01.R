inputs <- readLines("input.txt")

get_move <- function(input) {
  move <- substring(input, c(1,2), c(1, nchar(input)))
  if (move[1] == "L") return(-as.numeric(move[2])) else return(as.numeric(move[2]))
}

rotate_dial <- function(position, move) (position + move) %% 100

solve_day_1 <- function(position, inputs, zeros, clicks) {
  if (length(inputs) == 0) return(c(zeros, clicks))
  move <- get_move(inputs[[1]])
  new_position <- rotate_dial(position, move)
  if(new_position == 0) zeros <- zeros + 1
  clicks <- clicks + abs(move) %/% 100
  if (move < 0 & (new_position > position | new_position == 0) & position != 0) clicks <- clicks + 1
  if (move > 0 & new_position < position) clicks <- clicks + 1
  Tailcall(solve_day_1, new_position, inputs[-1], zeros, clicks)
}

solution <- solve_day_1(50, inputs, 0, 0)
cat("Part 1: ", solution[1], "\nPart 2: ", solution[2])


