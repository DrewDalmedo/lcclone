id: 2913
slug: subarrays-distinct-element-sum-of-squares-i
title: Subarrays Distinct Element Sum of Squares I
difficulty: easy
tags: binary-indexed-tree, segment-tree, array, hash-table
signature: int sumCounts(int[] nums)
---
Pick two indices with `0 <= i <= j < nums.length`. The elements from
`nums[i]` through `nums[j]`, kept in order, make up the **slice**
`nums[i..j]`. An array of length `n` has `n * (n + 1) / 2` slices, one for
each choice of `(i, j)`, and every one of them is used below, even when
another slice has exactly the same contents.

The **variety** of a slice is how many different numbers it holds. A
number that shows up several times in the slice still adds only 1, so a
slice equal to `[9,3,9,9]` has variety 2.

Square the variety of every slice, then add up all those squares, and
return the total.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
