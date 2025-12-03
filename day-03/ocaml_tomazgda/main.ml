let () =
  print_string "Solution to Part 1: ";

  Aux.read_banks_from_file "input"
  |> Part_one.max_joltage
  |> print_int;
  print_newline ();

  print_string "Solution to Part 2: ";

  Aux.read_banks_from_file "input"
  |> Part_two.max_joltage
  |> print_int;
  print_newline ();
