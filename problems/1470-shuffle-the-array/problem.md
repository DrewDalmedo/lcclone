id: 1470
slug: shuffle-the-array
title: Shuffle the Array
difficulty: easy
tags: array
signature: int[] shuffle(int[] nums, int n)
---
`nums` holds `2n` numbers. Cut it into a front half, `nums[0]` through
`nums[n - 1]`, and a back half, `nums[n]` through `nums[2n - 1]`. Then put
the halves back together the way a perfect riffle merges two piles of
cards, alternating between them and opening with the front half. The
front half ends up in the 1st, 3rd, 5th, ... places of the result and the
back half in the 2nd, 4th, 6th, ... places, each half keeping its own
order.

Return that merged sequence as a new array of `2n` numbers. In index terms,
position `2k` of the answer holds `nums[k]` and position `2k + 1` holds
`nums[n + k]`, for each `k` from `0` to `n - 1`. The parameter `n` is
always half the length of `nums`.

@examples

## Constraints

- `1 <= n <= 500`
- `nums.length == 2 * n`
- `1 <= nums[i] <= 1000`
