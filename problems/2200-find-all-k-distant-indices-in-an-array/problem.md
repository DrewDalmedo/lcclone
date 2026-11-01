id: 2200
slug: find-all-k-distant-indices-in-an-array
title: Find All K-Distant Indices in an Array
difficulty: easy
tags: array, two-pointers
signature: int[] findKDistantIndices(int[] nums, int key, int k)
---
A road is divided into plots numbered `0` to `n - 1` from one end, where `n`
is the length of `nums`, and `nums[i]` is a code for what stands on plot
`i`. Each plot whose code is `key` has a street lamp. A lamp lights its own
plot and every plot at most `k` plots away from it, on either side.

Put as a formula: plot `i` is lit exactly when some plot `j` has
`nums[j] == key` and `|i - j| <= k`. The title's **k-distant indices** are
the lit plot numbers. A plot that holds a lamp is always lit, by its own
lamp.

Return the numbers of all lit plots in increasing order, each once. (These
are plot numbers, not codes from `nums`.)

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 1000`
- `key` occurs in `nums`.
- `1 <= k <= nums.length`
