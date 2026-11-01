id: 944
slug: delete-columns-to-make-sorted
title: Delete Columns to Make Sorted
difficulty: easy
tags: array, string
signature: int minDeletionSize(string[] strs)
---
Every string in `strs` has the same length `m`. For a position `j` from `0`
to `m - 1`, **column** `j` is the sequence of letters found at position `j`
of `strs[0]`, `strs[1]`, `strs[2]` and so on, taking the strings in the
order they are given.

A column is **ordered** when, from each string to the next, its letter
stays the same or moves later in the alphabet. It is **out of order** as
soon as some letter is followed by an earlier one (a `'d'` in one string
and a `'b'` at the same position of the next, say), wherever that happens
and however many times.

Return the number of columns that are out of order. This is also the
fewest columns you would have to strike out to leave only ordered ones,
since striking out a column changes nothing about the others. With a
single string, every column is ordered.

@examples

## Constraints

- `n == strs.length`
- `1 <= n <= 100`
- `1 <= strs[i].length <= 1000`
- All the strings of `strs` have the same length.
- `strs[i]` consists of lowercase English letters.
