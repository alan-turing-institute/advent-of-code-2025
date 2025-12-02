(* Part One Module *)

let apply_rotation position rotation =
  (position + rotation) mod 100

let count_zeroes rotations =
  let f (count, position) rotation =
    let next_position = apply_rotation position rotation in
    let extra = if next_position = 0 then 1 else 0 in
    (count + extra, next_position)
  in
  List.fold_left f (0, 50) rotations

let solve rotations =
  let final_count, _final_position = count_zeroes rotations in
  final_count
