extract_brackets <- function(x, b, split) {
  contents <- gsub(paste0("\\", b[1], "|\\", b[2]), "", x[1]) |> strsplit(split)
  return(contents[[1]])
}

input <- readLines("input.txt") |>
  strsplit(" ") |>
  lapply(function(x) {
    target <- (extract_brackets(x[1], c("[", "]"), "") == "#") * 1
    zeros <- target * 0
    buttons <- lapply(
      2:(length(x) - 1),
      function(i) {
        button_indices <- extract_brackets(x[i], c("(", ")"), ",") |> as.numeric()
        button <- zeros
        button[button_indices + 1] <- 1
        return(button)
      }
    )
    joltages <- extract_brackets(x[length(x)], c("{", "}"), ",") |>
      as.numeric()
    return(list(start=zeros, target=target, buttons=buttons, joltages=joltages))
  })


# Part 1: BFS + XOR
xor <- function(a,b) (a | b) & !(a & b)

search_over_buttons <- function(schematic, states) {
  new_states <- list()
  for (i in 1:length(states)) {
    state <- states[[i]]
    for (j in 1:length(schematic$buttons)) {
      button <- schematic$buttons[[j]]
      new_state <- state
      new_state$state <- xor(new_state$state, button) * 1
      new_state$presses <- new_state$presses + 1
      if (all(new_state$state == schematic$target)) return(new_state$presses)
      if (j == 1) new_states[[i]] <- list(new_state)
      else new_states[[i]][[j]] <- new_state
    }
  }
  new_states <- unlist(new_states, recursive=FALSE)
  Tailcall(search_over_buttons, schematic, new_states)
}

shortest_n_presses <- function(schematic, increment=FALSE) {
  init_states <- list(list(state=schematic$start, presses=0))
  search_over_buttons(schematic, init_states)
}

part1 <- sapply(input, shortest_n_presses) |> sum()


# Part 2: linear programming
library(lpSolve) # caved in on base R only

solve_for_buttions <- function(schematic) {
  buttons <- schematic$buttons |> Reduce(cbind, x=_)
  solution <- lp(
    direction="min",
    objective.in=rep(1, ncol(buttons)),
    const.mat=buttons,
    const.dir=rep("==", nrow(buttons)),
    const.rhs=schematic$joltage,
    all.int=TRUE
  )
  return(solution$objval) # solution$solution for presses per button
}

part2 <- sapply(input, solve_for_buttions) |> sum()

# Out
cat("Part 1: ", part1, "\nPart 2: ", part2)


