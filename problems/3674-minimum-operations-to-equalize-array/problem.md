id: 3674
slug: minimum-operations-to-equalize-array
title: Minimum Operations to Equalize Array
difficulty: easy
tags: bit-manipulation, brainteaser, array
signature: int minOperations(int[] nums)
---
`nums` is a row of `n` positive integers, and you may change it with
**squashes**. A squash picks a block of one or more neighbouring entries,
`nums[l]` to `nums[r]` with `0 <= l <= r < n`, works out the bitwise AND
of the numbers in the block, and writes that result into every entry of
the block. (In the bitwise AND of several numbers, a bit is `1` exactly
when it is `1` in every one of them.) The block may be the whole row.

Return the smallest number of squashes after which every entry of `nums`
holds the same number. Which number that is doesn't matter, and if the
entries are all equal from the start, no squash is needed.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 10^5`
