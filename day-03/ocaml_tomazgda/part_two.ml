(* this looks like a recursive problem *)

(* 1. find the largest number in the first (n - 12) digits, and its location
   2. find the next largest number to the right of the location of the first
   3. repeat this process for 12 numbers
*)

let all_but_last n bank : int list =
  List.take (List.length bank - n + 1) bank

(* find largest number in the first 12 positions of bank and its location *)
let choose_max batteries : int * int =
  let indices = List.init (List.length batteries) (fun x -> x) in
  List.fold_left2
    (fun (current_max, max_index) battery index ->
       if battery > current_max then
         battery, index
       else
         current_max, max_index)
    (0, 0)
    batteries
    indices

(* that's our entry point *)

let build_max_joltage bank : int =
  let rec aux acc batteries length =
    if length = 0 then
      acc
    else
      let next_battery, pos =
        all_but_last length batteries |> choose_max
      in
      aux (next_battery :: acc) (List.drop (pos + 1) batteries) (length - 1)
  in
  aux [] bank 12
  |> List.rev
  |> Aux.unexplode

let max_joltage banks : int =
  banks
  |> List.map Aux.explode
  |> List.map build_max_joltage
  |> List.fold_left ( + ) 0

