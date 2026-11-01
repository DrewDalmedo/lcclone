id: 485
slug: max-consecutive-ones
title: Max Consecutive Ones
difficulty: easy
tags: array
signature: int findMaxConsecutiveOnes(int[] nums)
---
Every element of `nums` is either `0` or `1`. A **streak** is a stretch of
neighbouring elements that are all `1`, with a `0` or an end of the array
just before it and just after it. Return the number of elements in the
longest streak, or `0` if `nums` holds no `1` at all.

@examples

## Constraints

- `1 <= nums.length <= 10^5`
- Each `nums[i]` is `0` or `1`.

## Follow-up

Counting a streak afresh from every position takes `O(n^2)` time when
`nums` is all ones. Can you find the answer in a single pass?
