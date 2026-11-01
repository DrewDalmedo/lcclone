id: 2190
slug: most-frequent-number-following-key-in-an-array
title: Most Frequent Number Following Key In an Array
difficulty: easy
tags: array, hash-table, counting
signature: int mostFrequent(int[] nums, int key)
---
A music player logs the songs it plays as song numbers, in playing order:
`nums[0]` played first, then `nums[1]`, and so on. Song `key` is in the log
at least once.

Go through the log, and every time an entry equals `key` and is not the
last entry, write down the song in the entry right after it. A song can be
written down many times, once for each such entry. Return the song that is
written down most often (its song number, not how many times it was written
down). The input is chosen so that exactly one song is written down more
often than every other song.

Each entry is looked at on its own, so `key` itself can be written down: in
a stretch `key, key, key` of the log, the second and the third entry each
come right after an entry equal to `key`, which writes `key` down twice. A
`key` in the very last entry has nothing after it and writes nothing down.

@examples

## Constraints

- `2 <= nums.length <= 1000`
- `1 <= nums[i] <= 1000`
- `key` occurs in `nums`.
- Exactly one song is written down more often than each of the others.
