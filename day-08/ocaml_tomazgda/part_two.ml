(* Part Two gives you the last edge "hashed", i.e. 123 not (1,2,3). 
 * To pick out the x coordinate, I looked through the input myself *)

open Part_one

let rec kruskal2 forrest remaining_edges =
  match remaining_edges with
  | [] -> failwith "We should not be here"
  | (v1, v2) :: tl ->
    if Forrest.exists
        (fun tree -> Tree.mem v1 tree && Tree.mem v2 tree)
        forrest
    then
      kruskal2 forrest tl
    else
      let combined_tree =
        Forrest.filter (fun tree -> Tree.mem v1 tree || Tree.mem v2 tree) forrest
        |> Forrest.elements
        |> List.fold_left Tree.union Tree.empty
      in
      let new_forrest =
        Forrest.add
          combined_tree
          (Forrest.filter (fun tree -> not (Tree.mem v1 tree || Tree.mem v2 tree)) forrest)
      in
      if (Forrest.cardinal new_forrest) = 1 then
        (v1, v2)
      else
        kruskal2 new_forrest tl
