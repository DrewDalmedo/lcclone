id: 2717
slug: semi-ordered-permutation
title: Semi-Ordered Permutation
difficulty: easy
tags: array, simulation
signature: int semiOrderedPermutation(int[] nums)
---
A shelf holds `n` books labelled with the numbers `1` to `n`, each label
used once. `nums` lists the labels from the left end of the shelf to the
right end, so `n` is `nums.length`.

The shelf is **semi-ordered** when book `1` stands at the left end and book
`n` at the right end. The books between those two may be in any order.

In one move you swap two books that stand side by side. Return the fewest
moves that make the shelf semi-ordered, which is `0` when it already is.

@examples

## Constraints

- `2 <= nums.length == n <= 50`
- `1 <= nums[i] <= 50`
- `nums` is a permutation of `1` to `n`: each of those numbers appears
  exactly once.
