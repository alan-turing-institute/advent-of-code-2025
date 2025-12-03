let max_joltage_and_index bank : int * int =
  let indices = List.init (List.length bank) (fun x -> x) in
  List.fold_left2
    (fun (current_max, max_index) battery index ->
       if battery > current_max then
         battery, index
       else
         current_max, max_index)
    (0, 0)
    bank
    indices

let max_joltage_pair bank : int =
  let reduced_bank = List.take (List.length bank - 1) bank in
  let max_joltage, index = max_joltage_and_index reduced_bank in

  (max_joltage * 10 )
  +
  (List.drop (index + 1) bank |> max_joltage_and_index |> Pair.fst)

let max_joltage banks : int =
  banks
  |> List.map Aux.explode
  |> List.map max_joltage_pair
  |> List.fold_left ( + ) 0
