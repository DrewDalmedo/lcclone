id: 1399
slug: count-largest-group
title: Count Largest Group
difficulty: easy
tags: hash-table, math, counting
signature: int countLargestGroup(int n)
---
The **digit sum** of a positive integer is the total of its decimal digits:
the digit sum of `507` is `5 + 0 + 7 = 12`.

File each whole number from `1` up to `n`, `n` included, under its digit
sum. All the numbers filed under the same digit sum make up one group, and
the size of a group is how many numbers it holds.

Find the size of the biggest group, then return how many groups are that
big. The answer counts groups, not numbers: if three groups tie for the
most numbers, return `3`, whatever their size.

@examples

## Constraints

- `1 <= n <= 10^4`
