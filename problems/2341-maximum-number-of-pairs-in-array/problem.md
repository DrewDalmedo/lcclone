id: 2341
slug: maximum-number-of-pairs-in-array
title: Maximum Number of Pairs in Array
difficulty: easy
tags: array, hash-table, counting
signature: int[] numberOfPairs(int[] nums)
---
A jar holds marbles, and marble `i` has the number `nums[i]` painted on it.
You take marbles out of the jar two at a time, and the two you take out
together must show the same number; each such draw is one **pair**. Keep
drawing pairs for as long as the jar still holds two marbles with the same
number.

Return an array of two integers: first the number of pairs you drew, then
the number of marbles still in the jar at the end.

Which marbles you draw, and in what order, doesn't change the two counts.
A number painted on three marbles, for instance, always gives one pair and
leaves one of those marbles in the jar.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 100`
