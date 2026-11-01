id: 561
slug: array-partition
title: Array Partition
difficulty: easy
tags: greedy, array, counting-sort, sorting
signature: int arrayPairSum(int[] nums)
---
`nums` has `2 * n` elements. Match them up two by two: every element goes
into exactly one pair, together with one other element, and you decide
which elements are paired. Two equal values at different positions are
two separate elements.

Each pair contributes the **smaller** of its two values, or that value
when the two are equal. Over all the ways of pairing up the elements,
return the largest sum of the `n` pairs' contributions.

@examples

## Constraints

- `1 <= n <= 10^4`
- `nums.length == 2 * n`
- `-10^4 <= nums[i] <= 10^4`

## Follow-up

A sort settles this in `O(n log n)` time. But every value of `nums` is
one of only `2 * 10^4 + 1` integers: can you make use of that to get the
answer in linear time?
