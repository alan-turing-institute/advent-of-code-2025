open Utils

let () =
  let ids, ranges = ids_and_ranges_of_db (read_db "input") in
  print_string "Solution to Part 1: ";
  Part_one.n_fresh_ingredients ids ranges
  |> print_int;
  print_newline ();

  print_string "Solution to Part 2: ";
  Part_two.disjoint_ranges ranges
  |> Part_two.count_fresh
  |> print_int;
  print_newline ();
