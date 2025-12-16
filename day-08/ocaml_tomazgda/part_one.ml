let make_vertex row =
  match String.split_on_char ',' row with
  | x :: y :: z :: [] ->
    (int_of_string x, int_of_string y, int_of_string z)
  | _ -> failwith "could not make vertex from row"

let hash_vertex (x,y,z) =
  string_of_int x ^ string_of_int y ^ string_of_int z
  |> int_of_string

let read_vertices filename =
  In_channel.open_text filename
  |> In_channel.input_lines
  |> List.map make_vertex

let rec product l1 l2 =
  match l1 with
  | [] -> []
  | x :: xs -> List.map (fun y -> (x, y)) l2 @ product xs l2

let distance (x1,y1,z1) (x2,y2,z2) =
  [x2-x1;y2-y1;z2-z1]
  |> List.map Int.to_float
  |> List.map (fun n -> Float.pow n 2.)
  |> List.fold_left ( +. ) 0.
  |> Float.sqrt

let sorted_edges vertices =
  product vertices vertices
    |> List.filter (fun (v1,v2) -> compare v1 v2 < 0)
    |> List.sort_uniq (fun (v1,v2) (v3,v4) -> Float.compare (distance v1 v2) (distance v3 v4))
    |> List.map (fun (v1,v2) -> (hash_vertex v1, hash_vertex v2))

module Tree = Set.Make (Int)
module Forrest = Set.Make (Tree)

let make_initial_forrest vertices =
  List.map hash_vertex vertices
  |> List.map Tree.singleton
  |> Forrest.of_list

let rec kruskal forrest remaining_edges n =
    match n, remaining_edges with
      | 0, _ | _, [] -> forrest
      | n, (v1, v2) :: tl ->
        if Forrest.exists
            (fun tree -> Tree.mem v1 tree && Tree.mem v2 tree)
            forrest
        then
          kruskal forrest tl (n - 1)
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
          kruskal new_forrest tl (n - 1)

let solve filename n =
  let vertices = read_vertices filename in

  let edges = sorted_edges vertices
  and initial_forrest = make_initial_forrest vertices in

  kruskal initial_forrest edges n
  |> Forrest.to_list
  |> List.map Tree.cardinal
  |> List.sort (fun v1 v2 -> - Int.compare v1 v2)
  |> List.take 3
  |> List.fold_left ( * ) 1

