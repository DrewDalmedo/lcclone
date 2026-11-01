id: 2293
slug: min-max-game
title: Min Max Game
difficulty: easy
tags: array, simulation
signature: int minMaxGame(int[] nums)
---
The length of `nums` is a power of two. A **squeeze** turns an array of
length `m` (with `m >= 2`) into one of length `m / 2`:

- cut the array into consecutive pairs: positions `0` and `1` make pair
  `0`, positions `2` and `3` make pair `1`, and so on up to pair
  `m / 2 - 1`;
- pair number `k` is replaced by its **smaller** value when `k` is even,
  and by its **larger** value when `k` is odd;
- the values that are left keep the order of their pairs.

Squeeze `nums`, then squeeze the result, and go on until a single value is
left. Return that value.

The pairs are numbered from `0` again in every squeeze, so pair `0` always
gives its smaller value. If `nums` already holds a single value, no squeeze
is needed and that value is the answer.

@examples

## Constraints

- `1 <= nums.length <= 1024`
- `nums.length` is a power of two.
- `1 <= nums[i] <= 10^9`
