let () =
  print_string "Solution to Part 1: ";

  Aux.read_id_ranges "input"
  |> Part_one.sum_id_ranges
  |> print_int;
  print_newline ();

  print_string "Solution to Part 2: ";

  Aux.read_id_ranges "input"
  |> Part_two.sum_id_ranges
  |> print_int;
  print_newline ();

