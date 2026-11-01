id: 2363
slug: merge-similar-items
title: Merge Similar Items
difficulty: easy
tags: array, hash-table, ordered-set, sorting
signature: int[][] mergeSimilarItems(int[][] items1, int[][] items2)
---
`items1` and `items2` are two lists of rows. A row `[v, w]` stands for
items with the **label** `v` and a weight of `w` in all. A label occurs at
most once within `items1`, and at most once within `items2`, but the same
label may occur in both lists.

Merge them by label. For a label `v` that appears in `items1`, in
`items2` or in both, the result holds the row `[v, s]`, where `s` sums the
weights that `v` carries across the two lists: just its one weight if a
single list has it, or both weights added if both lists do.

Neither input list is in any particular order. In the result each label
appears once, and the rows run from the smallest label to the largest.

@examples

## Constraints

- `1 <= items1.length, items2.length <= 1000`
- `items1[i].length == items2[j].length == 2`
- `1 <= v, w <= 1000` for every row `[v, w]`
- The labels within `items1` are all different, and so are the labels
  within `items2`.
