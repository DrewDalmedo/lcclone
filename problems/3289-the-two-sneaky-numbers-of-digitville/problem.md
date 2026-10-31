id: 3289
slug: the-two-sneaky-numbers-of-digitville
title: The Two Sneaky Numbers of Digitville
difficulty: easy
tags: array, hash-table, math
signature: int[] getSneakyNumbers(int[] nums)
compare: unordered 0
---
Someone set out to write down every whole number from `0` to `n - 1` once,
but slipped and wrote two **different** numbers of that range down twice.
What they ended up with is `nums`: `n + 2` entries in no particular order
(so `n` is `nums.length - 2`), in which every number from `0` to `n - 1`
appears, two of them twice and all the others once.

Return the two numbers that appear twice, as an array of two, *in either
order*.

@examples

## Constraints

- `2 <= n <= 100`
- `nums.length == n + 2`
- `0 <= nums[i] < n`
- Every number from `0` to `n - 1` appears in `nums`. Exactly two of them
  appear twice; the rest appear once.

## Follow-up

Can you find the two numbers using only `O(1)` extra memory, without
changing `nums`?
