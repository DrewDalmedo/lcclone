id: 3005
slug: count-elements-with-maximum-frequency
title: Count Elements With Maximum Frequency
difficulty: easy
tags: array, hash-table, counting
signature: int maxFrequencyElements(int[] nums)
---
`nums` is one day's log of a radio station: each entry is the catalogue
number of a song, written down every time that song went on air. A song's
**play count** is the number of entries of `nums` that hold its number.

The day's **hits** are the songs whose play count is the highest of the
day; when several songs share that count, they are all hits. Return the
total number of plays that went to hits, that is, how many entries of
`nums` hold the number of a hit.

The answer counts entries, not songs: two hits played three times each
make `6`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
