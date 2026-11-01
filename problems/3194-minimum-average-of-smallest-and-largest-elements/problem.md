id: 3194
slug: minimum-average-of-smallest-and-largest-elements
title: Minimum Average of Smallest and Largest Elements
difficulty: easy
tags: array, two-pointers, sorting
signature: double minimumAverage(int[] nums)
---
`nums` holds an even number of integers. Empty it in rounds: each round,
remove **one** occurrence of the smallest value left and **one** occurrence
of the largest value left, and write down their average, `(smallest +
largest) / 2`, as a real number (`3` and `8` average `5.5`, not `5`). When
every value left is the same, the two removed elements are two different
occurrences of it.

After the last round, return the smallest average written down.

An answer within `10^-5` of the exact value is accepted.

@examples

## Constraints

- `2 <= nums.length <= 50`
- `nums.length` is even
- `1 <= nums[i] <= 50`

## Follow-up

Once `nums` is sorted, which elements are removed together in each round?
