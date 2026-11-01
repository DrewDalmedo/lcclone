id: 925
slug: long-pressed-name
title: Long Pressed Name
difficulty: easy
tags: two-pointers, string
signature: bool isLongPressedName(string name, string typed)
---
To **stretch** a string, replace each of its characters with a block of
one or more copies of that same character. Each block may have its own
length, and a block of one copy leaves its character as it was. For
example, `"abb"` can be stretched into `"abb"`, `"aaabbb"` or `"abbbbb"`,
but not into `"ab"`, `"abba"` or `"bab"`.

This is what a sticky keyboard does: each key prints its character at
least once, and sometimes more. Someone wanted to type `name` and got
`typed`.

Return `true` if `typed` is a stretch of `name`, and `false` otherwise.

Stretching never drops a character, never changes the order of the
characters and never adds a different one. Two equal neighbours in `name` are two separate
blocks, so `typed` needs at least two copies of that character there.

@examples

## Constraints

- `1 <= name.length, typed.length <= 1000`
- `name` and `typed` consist of lowercase English letters.
