id: 3432
slug: count-partitions-with-even-sum-difference
title: Count Partitions with Even Sum Difference
difficulty: easy
tags: array, math, prefix-sum
signature: int countPartitions(int[] nums)
---
A train has `n` carriages in a row, and carriage `i` carries `nums[i]`
passengers. It can be split at any one of the `n - 1` couplings between two
neighbouring carriages, which leaves a front train (the carriages before
the coupling) and a rear train (the carriages after it), each with at least
one carriage.

A coupling is **fair** when the number of passengers in the front train and
the number in the rear train differ by an even amount. It doesn't matter
which train has more passengers, and a difference of `0` is even.

Return how many of the `n - 1` couplings are fair.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
