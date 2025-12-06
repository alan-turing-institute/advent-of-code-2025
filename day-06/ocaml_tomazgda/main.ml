let read_input filename =
  let lines =
    In_channel.open_text filename
    |> In_channel.input_all
    |> String.trim
    |> String.split_on_char '\n'
    |> List.map (String.split_on_char ' ')
    |> List.map (List.filter ( ( <> ) ""))
  in
  let values =
    lines
    |> fun xs -> List.take (List.length xs - 1) xs
    |> List.map (List.map int_of_string)
  and operations =
    lines
    |> fun xs -> List.nth xs (List.length xs - 1)
  in values, operations

let transpose horizontal_values =
  horizontal_values
  |> List.map List.to_seq
  |> List.to_seq
  |> Seq.transpose
  |> Seq.map List.of_seq
  |> List.of_seq

type expression =
  | Product of int list
  | Sum of int list

let eval_expression = function
  | Product values -> List.fold_left ( * ) 1 values
  | Sum values -> List.fold_left ( + ) 0 values

let create_expression values operation =
  match operation with
  | "*" -> Product (values)
  | "+" -> Sum (values)
  | _ -> failwith "could not parse operation"


let () =
  let horizontal_values, operations = read_input "test" in
  let vertical_values = transpose horizontal_values in

  print_string "Solution to Part 1: ";
  List.map2 create_expression vertical_values operations
  |> List.map eval_expression
  |> List.fold_left ( + ) 0
  |> print_int;
  print_newline ();
