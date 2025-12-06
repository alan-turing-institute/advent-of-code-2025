val read_input : string -> int list list * string list
(* read all but the last line of the input into a list of lists of integers,
 * and the last line into a list of strings (operations) *)

val transpose : int list list -> int list list
(* take a list of list of integers and tranpose them, i.e the first integers
 * in each list form their own list, the second their own, and so on. *)

type expression
(* A variant which is either a product of a list of integers or a sum of a list
 * of integers *)

val eval_expression : expression -> int
(* sums a Sum expression and takes the product of a Product expression *)

val create_expression : int list -> string -> expression
(* produces an expression out of some values and a string: hopefully either
 * '+' or '*' *)

(* Part Two *)

(* val cephalopod_translate : int list -> int list *)
(* (\* translate the human read values to cephalopod values, by transposing lists *)
(*  * of their digits *\) *)
