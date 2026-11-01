id: 3903
slug: smallest-stable-index-i
title: Smallest Stable Index I
difficulty: easy
tags: array, prefix-sum
signature: int firstStableIndex(int[] nums, int k)
---
`nums` holds `n` numbers at positions `0` to `n - 1`. Cutting it at
position `i` gives two pieces that **overlap** in `nums[i]`: the left piece
`nums[0..i]`, from the start up to and including position `i`, and the
right piece `nums[i..n-1]`, from position `i` to the end.

The **gap** of the cut at `i` is the largest number of the left piece
minus the smallest number of the right piece. Since `nums[i]` lies in both
pieces, a gap is never negative. The cut is **stable** when its gap is at
most `k`; a gap of exactly `k` still counts as stable.

Return the smallest position `i` whose cut is stable, or `-1` if no cut
is. When `n` is `1`, both pieces are that single number, so position `0`
is stable.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 10^9`
- `0 <= k <= 10^9`
