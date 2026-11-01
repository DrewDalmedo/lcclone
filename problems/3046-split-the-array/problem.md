id: 3046
slug: split-the-array
title: Split the Array
difficulty: easy
tags: array, hash-table, counting
signature: bool isPossibleToSplit(int[] nums)
---
A pile of `n` cards lies on the table, where `n` is even, and card `i` has
the value `nums[i]` written on it. You must deal out the whole pile to two
players so that:

- each player gets exactly `n / 2` cards, and
- neither player ends up holding two cards with the same value.

Any card may go to either player, whatever its place in `nums`.

Return `true` if some deal meets both rules, and `false` if every deal
breaks at least one of them.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `nums.length` is even.
- `1 <= nums[i] <= 100`
