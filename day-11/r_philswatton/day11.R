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






# Memoisation by hand ----

count_paths <- function(input, curr="svr", passed=c(0,0), counter=0) {
  if (curr == "dac") passed[1] <- 1
  if (curr == "fft") passed[2] <- 1
  next_paths <- input[[curr]]
  if (length(next_paths) == 1) if (next_paths == "out") return(counter + Reduce(`&`, passed))
  next_paths <- next_paths[next_paths != "svr"]
  return(sum(sapply(next_paths, {\(x) Tailcall(count_paths, input, x, passed, counter)})))
}





# Cacheing
library(cachem)
# ?cache_mem

# Get global environment inside fn
tester <- function() {
  return(parent.env(environment()))
}
tester()

# Catch fn call
get_args <- function(...) {
  mc <- match.call(...)
  
}



# Attempt 1 ----

memo <- function(fn) {
  cache <- cachem::cache_mem(max_size=1024 * 1024^2)
  return(function(...) {
    call <- as.list(match.call()[-1])
    key <- rlang::hash(call)
    res <- cache$get(key)
    if (inherits(res, "key_missing")) {
      res <- fn(...)
    }
    cache$set(key, res)
    return(res)
  })
}

count_paths <- memo(count_paths)
part2 <- count_paths(input)


f <- function(x) {
  Sys.sleep(1)
  mean(x)
}
f2 <- memo(f)
system.time(f2(1:10))
system.time(f2(1:10))
f2(1:10)


## Attempt 2 ----

cache <- cachem::cache_mem(1024^2)
count_paths_memo <- function(input, curr="svr", passed=c(0,0), counter=0) {
  mc <- match.call()
  args <- lapply(as.list(mc[-1]), eval, parent.frame())
  key <- rlang::hash(args)
  res <- cache$get(key)
  if (inherits(res, "key_missing")) {
    if (curr == "dac") passed[1] <- 1
    if (curr == "fft") passed[2] <- 1
    next_paths <- input[[curr]]
    if (length(next_paths) == 1) if (next_paths == "out") return(counter + Reduce(`&`, passed))
    next_paths <- next_paths[next_paths != "svr"]
    res <- sum(sapply(next_paths, function(x) Tailcall(count_paths_memo, input, x, passed, counter)))
    cache$set(key, res)
  }
  return(res)
}
count_paths_memo(input) |> as.character()



## Attempt 3: Write my own memoise function ----

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
count_paths(input) |> as.character()


