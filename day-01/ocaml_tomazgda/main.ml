(* Day 1 | Plan

   Part 1 :

   The puzzle input is a list of rotations. A rotation is a string
   where the first character indicates a direction (left L or right R)
   and the remaining characters correspond to a distance.

   E.g. "L68" is a rotation -> rotate left by distance 68

   We apply a rotation to a position on a dial with positions from 0
   to 100.

   Starting at position 50, we successively apply rotations, counting
   the number of times each rotation led us to a position of 0.

   The module Part_one should have a signature like:

   val apply_rotation : position -> rotation -> position

   val count_zeroes : rotation list -> t

   val solve : rotation list -> unit

   Part 2:

   Now as we apply successivel rotations we count every time each
   rotation causes the dial to pass 0. We are going to need a function
   that takes a position and a rotation and returns the number of times
   zero is passed.

   val zero_passes : position -> rotation -> int

   count_zeroes should then use this function to accumulate zeroes.
*)

let () =
  print_string "Solution to Part 1: ";

  Aux.read_rotations "input"
  |> Part_one.solve
  |> print_int;
  print_newline ();

  print_string "Solution to Part 2: ";

  Aux.read_rotations "input"
  |> Part_two.solve
  |> print_int;
  print_newline ();

