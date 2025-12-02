let parse_id_range id_range : int * int =
  match String.split_on_char '-' id_range with
  | low :: high :: [] -> (int_of_string low), (int_of_string high)
  | _ -> failwith "id_range did not match correct form"

let read_id_ranges filename : (int * int) list =
  In_channel.open_text filename
  |> In_channel.input_all
  |> String.trim
  |> String.split_on_char ','
  |> List.map parse_id_range
