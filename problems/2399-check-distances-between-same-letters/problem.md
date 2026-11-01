id: 2399
slug: check-distances-between-same-letters
title: Check Distances Between Same Letters
difficulty: easy
tags: array, hash-table, string
signature: bool checkDistances(string s, int[] distance)
---
The string `s` is made of lowercase letters, and every letter in it occurs
exactly twice. The array `distance` has 26 entries, one per letter of the
alphabet in order: `distance[0]` goes with `'a'`, `distance[1]` with `'b'`,
and so on up to `distance[25]` with `'z'`.

For a letter of `s`, count the characters standing between its two copies,
not counting the copies themselves: if they sit at indices `i < j`, that is
`j - i - 1`. Every character in between counts, copies of other letters
included. In `"xyyx"`, for example, two characters separate the copies of
`x`, and none separate the copies of `y`, which are neighbours.

Return `true` if, for each letter that occurs in `s`, this count equals that
letter's entry in `distance`, and `false` as soon as one letter's count
differs. The entries for letters that don't occur in `s` play no part,
whatever they hold.

@examples

## Constraints

- `2 <= s.length <= 52`
- `s` consists of lowercase English letters only.
- Each letter that occurs in `s` occurs exactly twice.
- `distance.length == 26`
- `0 <= distance[i] <= 50`
