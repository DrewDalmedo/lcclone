id: 1437
slug: check-if-all-1s-are-at-least-length-k-places-away
title: Check If All 1's Are at Least Length K Places Away
difficulty: easy
tags: array
signature: bool kLengthApart(int[] nums, int k)
---
`nums` is a recording of a signal, one entry per tick: `nums[i]` is `1` if
a pulse came at tick `i` and `0` if the line was quiet. Two pulses are
**consecutive** when no other pulse comes between them.

The recording is **well spaced** when every two consecutive pulses have at
least `k` quiet ticks between them. For pulses at ticks `i < j` that is
`j - i - 1 >= k`, or `j - i > k`: with `k = 2`, pulses at ticks `3` and `6`
are fine, but pulses at ticks `3` and `5` are too close.

Return `true` if `nums` is well spaced, and `false` otherwise. Quiet ticks
before the first pulse or after the last one play no part, and a recording
with fewer than two pulses is always well spaced.

@examples

## Constraints

- `1 <= nums.length <= 10^5`
- `0 <= k <= nums.length`
- Each `nums[i]` is `0` or `1`.
