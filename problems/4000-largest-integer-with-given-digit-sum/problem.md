id: 4000
slug: largest-integer-with-given-digit-sum
title: Largest Integer With Given Digit Sum
difficulty: easy
tags: greedy, math
signature: int largestInteger(int n, int s)
---
You are looking for a non-negative integer that passes two tests:

- written in decimal the usual way, with no leading zeros, it is at most
  `n` digits long (zero is written `0`, a single digit);
- its digits add up to exactly `s`.

Return the largest integer that passes both tests, or `-1` if none does.

@examples

## Constraints

- `1 <= n <= 5`
- `0 <= s <= 100`
