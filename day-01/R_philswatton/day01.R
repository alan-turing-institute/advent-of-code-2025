inputs <- readLines("input.txt")

get_sign <- function(string) if (string == "L") return(-1) else return(1)

get_move <- function(input) {
  move_strings <- substring(input, c(1,2), c(1, nchar(input)))
  return(get_sign(move_strings[1]) * as.numeric(move_strings[2]))
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


