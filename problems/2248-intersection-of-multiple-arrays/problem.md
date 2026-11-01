id: 2248
slug: intersection-of-multiple-arrays
title: Intersection of Multiple Arrays
difficulty: easy
tags: array, hash-table, counting, sorting
signature: int[] intersection(int[][] nums)
---
A group of people is choosing a day for an outing. Person `i` hands in
`nums[i]`, the numbers of the days on which they are free. Everyone names
at least one day and no one names a day twice, but people may name
different numbers of days, and in any order.

Return the days on which **everyone** is free, that is, the numbers found
in every one of the lists, in increasing order. If there is no such day,
return an empty array.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i].length`
- `sum(nums[i].length) <= 1000`
- `1 <= nums[i][j] <= 1000`
- The numbers of each `nums[i]` are distinct.
