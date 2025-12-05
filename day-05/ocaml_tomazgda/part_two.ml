open Utils

(* two ranges are disjoint if they do not overlap *)
let disjoint (l1, u1) (l2, u2) =
  l2 > u1 || u2 < l1

(* we can compose ranges that are not disjoint *)
let can_compose r1 r2 =
  not (disjoint r1 r2)

let contains (l1, u1) (l2, u2) =
  l2 >= l1 && u2 <= u1

let min (x1, x2) =
  if x1 < x2 then x1 else x2

let max (x1, x2) =
  if x1 > x2 then x1 else x2

let join (l1, u1) (l2, u2) =
  (min (l1, l2), max (u1, u2))

(* If two ranges are disjoint, one range can swallow the other
   or we can join the ranges together. *)
let compose r1 r2 =
  if contains r1 r2 then r1
  else if contains r2 r1 then r2
  else join r1 r2

(* produce a set of disjoint ranges from a set of ranges *)
let disjoint_ranges ranges =
  RangeSet.fold
    (fun range (acc : RangeSet.t) ->
       let composable_ranges =
         RangeSet.filter (can_compose range) acc
       in
       (* try to find a range in the composable ranges *)
       match RangeSet.choose_opt composable_ranges with
       (* if we find one, we need to replace that range with the composed range *)
       | Some r ->
         RangeSet.remove r acc |> RangeSet.add (compose r range)
       (* otherwise their are no composable ranges, and so we add the range to the set of disjoint rnages *)
       | None -> RangeSet.add range acc
    )
    ranges
    RangeSet.empty

(* count fresh ids from a set of disjoint ranges *)
let count_fresh ranges =
  RangeSet.fold
    (fun (low, high) (acc : int) ->
       acc + (high - low) + 1)
    ranges
    0






