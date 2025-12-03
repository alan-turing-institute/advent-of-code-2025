let read_banks_from_file filename : string list =
  In_channel.open_text filename
  |> In_channel.input_all
  |> String.trim
  |> String.split_on_char '\n'

let explode n_as_string : int list =
  n_as_string
  |> String.fold_left (fun acc ch ->
      let digit = (int_of_char ch) - 48 in
      digit :: acc) []
  |> List.rev

let unexplode digits : int =
  let indices = List.init (List.length digits) (fun x -> x)
    |> List.rev
  in
  List.fold_left2
    (fun acc n i ->
       let multiplier =
         Int.to_float i
         |> Float.pow 10.
         |> Float.to_int
       in
       acc + n * multiplier)
    0
    digits
    indices
