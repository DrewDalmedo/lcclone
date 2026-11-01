id: 2176
slug: count-equal-and-divisible-pairs-in-an-array
title: Count Equal and Divisible Pairs in an Array
difficulty: easy
tags: array
signature: int countPairs(int[] nums, int k)
---
A sensor took one reading every second, starting at second `0`, so
`nums[t]` is the reading of second `t`. Two different seconds `s` and `t`
make a **matching pair** when both of these hold:

- their readings agree: `nums[s] == nums[t]`;
- the product of the two seconds themselves, `s * t`, is a multiple of
  `k`. The readings play no part in this second test.

Return how many matching pairs there are, counting each pair once:
seconds `2` and `5`, say, make one pair, not two.

A pair that includes second `0` has the product `0`, which is a multiple
of every `k`, so it matches whenever its two readings agree.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= k <= 100`
