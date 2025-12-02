(* Part Two Module *)

let apply_rotation position rotation =
  (position + rotation) mod 100

let zero_passes position rotation =
  let full_rotations = abs (rotation / 100)
  and partial_rotation = abs (rotation mod 100) in
  let extra =
    if rotation > 0 then
      (position + partial_rotation) >= 100
      |> Bool.to_int
    else
      (partial_rotation > position)
      |> Bool.to_int
  in
  full_rotations + extra

let count_zeroes rotations =
  let f (count, position) rotation =
    let next_position = apply_rotation position rotation in
    let extra = zero_passes position rotation in
    (count + extra, next_position)
  in
  List.fold_left f (0, 50) rotations

let solve rotations =
  let final_count, _final_position = count_zeroes rotations in
  final_count
