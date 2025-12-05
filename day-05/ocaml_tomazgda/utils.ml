module Id_range = struct
  type t = int * int
  let compare = compare
end

module RangeSet = Set.Make (Id_range)
module IntSet = Set.Make (Int)

let read_db filename =
  In_channel.open_text filename
  |> In_channel.input_all
  |> String.trim
  |> String.split_on_char '\n'
  |> List.filter (fun x -> x <> "")

let parse_id_range line =
  match
  String.split_on_char '-' line
  |> List.map int_of_string
  with
  | low :: high :: [] -> (low, high)
  | _ -> failwith "could not parse id range"

let ids_and_ranges_of_db =
  List.fold_left (fun (ids, id_ranges) x ->
      match int_of_string_opt x with
      | Some n -> let new_ids = IntSet.add n ids in
        (new_ids, id_ranges)
      | None -> let new_id_ranges = RangeSet.add (parse_id_range x) id_ranges in
        (ids, new_id_ranges))
    (IntSet.empty, RangeSet.empty)
