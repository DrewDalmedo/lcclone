id: 599
slug: minimum-index-sum-of-two-lists
title: Minimum Index Sum of Two Lists
difficulty: easy
tags: array, hash-table, string
signature: string[] findRestaurant(string[] list1, string[] list2)
compare: unordered 0
---
You are given two lists of strings, `list1` and `list2`. Within each list,
no string appears twice. A string is **common** when it appears in both
lists, and if it is `list1[i]` and `list2[j]` (counting from 0), its
**index sum** is `i + j`.

Return every common string whose index sum is the smallest of all the
common strings' index sums. When several common strings share that
smallest sum, return all of them, *in any order*.

Two strings match only when they are exactly equal: upper and lower case
letters differ, and every space counts, so `"ab"`, `"Ab"`, `"a b"` and
`"ab "` are four different strings. Every test has at least one common
string.

@examples

## Constraints

- `1 <= list1.length, list2.length <= 1000`
- `1 <= list1[i].length, list2[i].length <= 30`
- `list1[i]` and `list2[i]` consist of English letters and spaces.
- The strings of `list1` are all different, and so are those of `list2`.
- At least one string is in both lists.

## Follow-up

Comparing every string of `list1` with every string of `list2` takes
`O(n * m)` comparisons, for lists of `n` and `m` strings. Can you find the
answer while looking at each string only a constant number of times?
