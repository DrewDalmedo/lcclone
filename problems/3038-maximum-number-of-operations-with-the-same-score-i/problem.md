id: 3038
slug: maximum-number-of-operations-with-the-same-score-i
title: Maximum Number of Operations With the Same Score I
difficulty: easy
tags: array, simulation
signature: int maxOperations(int[] nums)
---
Picture the elements of `nums` standing in a queue, `nums[0]` at the
front. An **operation** takes the two elements at the front out of the
queue, and its **score** is the sum of those two values; everything behind
them moves up two places. An operation can only be done while at least two
elements are left.

You do operations one after another, and you may stop whenever you like,
but all the operations you do must have one and the same score. Return the
largest number of operations you can do.

There is nothing to choose in an operation: the first always takes
`nums[0]` and `nums[1]`, the second `nums[2]` and `nums[3]`, and so on. So
the first operation fixes the score, and you have to stop at the first pair
whose sum is different, even if pairs further back would add up to that
score again. When the length of `nums` is odd, the last element is never
taken.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 1000`
