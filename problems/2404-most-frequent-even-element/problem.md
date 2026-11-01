id: 2404
slug: most-frequent-even-element
title: Most Frequent Even Element
difficulty: easy
tags: array, hash-table, counting
signature: int mostFrequentEven(int[] nums)
---
Think of `nums` as a pile of ballots: each entry is one vote for the number
written on it. Only votes for even numbers are valid, and `0` counts as
even; every vote for an odd number is thrown out, however many such votes
there are.

Return the number that receives the most valid votes (the number itself,
not its number of votes). If several numbers share the highest number of
valid votes, the smallest of them wins, wherever its votes sit in `nums`.
If there is no valid vote at all, because every entry is odd, return `-1`.

@examples

## Constraints

- `1 <= nums.length <= 2000`
- `0 <= nums[i] <= 10^5`
