id: 1207
slug: unique-number-of-occurrences
title: Unique Number of Occurrences
difficulty: easy
tags: array, hash-table
signature: bool uniqueOccurrences(int[] arr)
---
For each number that appears in `arr`, its **tally** is how many entries
of `arr` hold it. Decide whether the tallies tell the numbers apart:
return `true` if every number in `arr` has a tally that no other number
shares, and `false` if two different numbers turn up the same number of
times.

Only numbers that appear in `arr` have a tally. So an array whose entries
are all the same number is always `true`, and an array of two or more
entries that are all different is always `false`, since each number then
has a tally of 1.

@examples

## Constraints

- `1 <= arr.length <= 1000`
- `-1000 <= arr[i] <= 1000`
