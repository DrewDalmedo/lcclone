id: 2455
slug: average-value-of-even-numbers-that-are-divisible-by-three
title: Average Value of Even Numbers That Are Divisible by Three
difficulty: easy
tags: array, math
signature: int averageValue(int[] nums)
---
Keep only the entries of `nums` that are even **and** divisible by `3`,
that is, the multiples of `6`. An entry that meets just one of the two
conditions, such as `8` or `15`, is dropped like any other.

Return the mean of the entries you kept: their total divided by how many
there are, rounded down to a whole number. A value that occurs several
times is kept once for every occurrence, so each copy adds to both the
total and the count.

If no entry of `nums` is a multiple of `6`, there is nothing to average:
return `0`.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 1000`
