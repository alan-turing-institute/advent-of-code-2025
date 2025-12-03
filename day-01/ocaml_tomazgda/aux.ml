let parse_rotation rotation =
  let direction = String.sub rotation 0 1
  and distance =
    (String.length rotation) - 1
    |> String.sub rotation 1
    |> int_of_string
  in
  match direction with
  | "R" -> distance
  | "L" -> - distance
  | _ -> failwith "direction did not match R or L"

let read_rotations filename =
  In_channel.open_text filename
  |> In_channel.input_all
  |> String.split_on_char '\n'
  |> List.filter (fun x -> not (x = String.empty))
  |> List.map parse_rotation
