let () =
  let rolls = Utils.read_rolls "input" in

  print_string "Solution to Part 1: ";

  rolls
  |> Part_one.count_accessible_rolls
  |> print_int;
  print_newline ();

  print_string "Solution to Part 2: ";

  rolls
  |> Part_two.total_rolls 0
  |> print_int;
  print_newline ();
