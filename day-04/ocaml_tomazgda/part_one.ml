open Utils

let adjacent_positions (x, y) =
  [(-1, -1); (-1, 0); (-1, 1); (0, -1); (0, 1); (1, -1); (1, 0); (1, 1)]
  |> List.map (fun (dx, dy) -> (x+dx, y+dy))

let adjacent_rolls forklift rolls =
  forklift
  |> adjacent_positions
  |> List.map (fun p -> PosSet.mem p rolls)
  |> List.map Bool.to_int
  |> List.fold_left ( + ) 0

let count_accessible_rolls rolls =
  rolls
  |> PosSet.filter (fun f -> (adjacent_rolls f rolls) < 4)
  |> PosSet.cardinal
