(* Part Two Module *)

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

(* This is a thing that works *)
let explode n : int list =
  Int.to_string n
  |> String.to_seq
  |> List.of_seq
  |> List.map
    (fun ch -> (int_of_char ch) - 48)

let group_digits n_groups digit_list =
  let group_size = (List.length digit_list) / n_groups in
  let rec aux acc tail =
    if List.is_empty tail then
      acc
    else
      let next_digits = List.take group_size tail
      and rest_digits = List.drop group_size tail in
      aux (next_digits :: acc) rest_digits
  in
  aux [] digit_list

(* NOTE: using polymorphic ( = ) *)
let is_invalid (ns : int list list) : bool =
  let first_n = List.hd ns in
  List.for_all (( = ) first_n ) ns

let is_prime n =
  if n = 1 then false else
  List.init (n - 2) (fun x -> x + 2)
  |> List.for_all (fun x -> (n mod x) != 0)

let prime_factors n =
  List.init n (fun x -> x + 1)
  |> List.filter (fun x -> (n mod x) = 0)
  |> List.filter is_prime

let sum_id_range (low, high) =
  range low high
  |> List.filter (fun n ->
      n_digits n
      |> prime_factors
      |> List.map
        (fun n_groups -> explode n |> group_digits n_groups)
      |> List.exists is_invalid)
  |> List.fold_left ( + ) 0

let sum_id_ranges id_ranges =
  id_ranges
  |> List.map sum_id_range
  |> List.fold_left ( + ) 0
