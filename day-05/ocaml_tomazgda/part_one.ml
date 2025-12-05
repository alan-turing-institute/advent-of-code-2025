open Utils

let valid_id id (low, high) =
  id >= low && id <= high

let n_fresh_ingredients ids id_ranges =
  ids
  |> IntSet.filter (fun id ->
      RangeSet.exists (fun id_range ->
          valid_id id id_range) id_ranges)
  |> IntSet.cardinal
