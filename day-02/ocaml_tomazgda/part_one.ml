(* Part One Module *)

(* an inclusive range *)
let range low high : int list =
  List.init (high - low + 1) (fun x -> x + low)

(* the number of digits in an integer *)
let n_digits n : int =
  Int.to_float n
  |> Float.log10
  |> Float.floor
  |> Float.to_int
  |> Int.succ

(* if an integer has an even number of digits *)
let even_digits n : bool =
  (n_digits n) mod 2 = 0

(* split an even integer into its digits *)
let split n : int * int =
  let digits = n_digits n
    |> Int.to_float
  in
  let divisor = Float.pow 10. (digits /. 2.)
    |> Float.to_int
  in
  let h1 = n / divisor in
  let h2 = n - (h1 * divisor) in
  h1,h2

(* if the two halves of  *)
let is_invalid id : bool =
  let h1, h2 = split id in
  h1 = h2

(* sum invalid ids in a given range *)
let sum_id_range (low, high) =
  range low high
  |> List.filter even_digits
  |> List.filter is_invalid
  |> List.fold_left ( + ) 0

(* sum the sum of invalid ids in a given range *)
let sum_id_ranges id_ranges =
  id_ranges
  |> List.map sum_id_range
  |> List.fold_left ( + ) 0
