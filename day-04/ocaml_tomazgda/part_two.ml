open Utils
open Part_one

let rec total_rolls acc rolls =
  let accessible_rolls =
    rolls |> PosSet.filter (fun f -> (adjacent_rolls f rolls) < 4)
  in
  if (PosSet.cardinal accessible_rolls) = 0 then acc
  else
    let remaining_rolls =
      PosSet.diff rolls accessible_rolls
    in
    total_rolls (PosSet.cardinal accessible_rolls + acc) remaining_rolls
