id: 2357
slug: make-array-zero-by-subtracting-equal-amounts
title: Make Array Zero by Subtracting Equal Amounts
difficulty: easy
tags: greedy, array, hash-table, sorting, simulation, heap-priority-queue
signature: int minimumOperations(int[] nums)
---
Every entry of `nums` is a whole number, `0` or more. The only way to change
`nums` is to play **rounds**, one after another. A round goes like this:

1. among the entries that are still above `0`, let `m` be the smallest, as
   `nums` stands at the start of this round;
2. choose any whole number `x` from `1` to `m`;
3. take `x` away from every entry that is above `0`. Entries that are
   already `0` are left as they are, so nothing ever drops below `0`.

A round can only be played while some entry is above `0`, since otherwise
there is no `m`.

How few rounds are enough to bring all of `nums` down to `0`? Return that
number. It is `0` when no entry is above `0` to begin with.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 100`
