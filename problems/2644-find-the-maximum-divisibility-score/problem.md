id: 2644
slug: find-the-maximum-divisibility-score
title: Find the Maximum Divisibility Score
difficulty: easy
tags: array
signature: int maxDivScore(int[] nums, int[] divisors)
---
Each value `d` in `divisors` earns one point for every entry of `nums` that
is a multiple of `d`, meaning that dividing the entry by `d` leaves no
remainder. Entries of `nums` are counted by position, so a number that
appears four times in `nums` gives four points to each `d` it is a multiple
of. The points of `d` are its **score**.

Find the best score and return the divisor that earns it. When several
divisors share the best score, the answer is the least value among them,
wherever it sits in `divisors`. That holds even when nothing scores: if no
entry of `nums` is a multiple of any divisor, every score is `0` and the
answer is the smallest entry of `divisors`.

@examples

## Constraints

- `1 <= nums.length, divisors.length <= 1000`
- `1 <= nums[i], divisors[i] <= 10^9`
