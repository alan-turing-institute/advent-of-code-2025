module Pos = struct
  type t = int * int
  let compare = compare
end

module PosSet = Set.Make(Pos)

let read_rolls filename =
  let lines =
    In_channel.open_text filename
    |> In_channel.input_all
    |> String.trim
    |> String.split_on_char '\n'
  in
  let _, rolls =
    List.fold_left (fun (y, acc) line ->
        let _, new_rolls =
          String.fold_left (fun (x, r) ch ->
              match ch with
              | '@' -> (x + 1, PosSet.add (x,y) r)
              | _ -> (x + 1, r)
            )
            (0, PosSet.empty)
            line
        in
        (y + 1, PosSet.union acc new_rolls))
      (0, PosSet.empty)
      lines
  in
  rolls
