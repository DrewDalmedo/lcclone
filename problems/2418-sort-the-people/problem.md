id: 2418
slug: sort-the-people
title: Sort the People
difficulty: easy
tags: array, hash-table, string, sorting
signature: string[] sortPeople(string[] names, int[] heights)
---
A choir of `n` singers is to stand in a single row ordered by height, the
tallest at the left end. Singer `i` goes by `names[i]` and is `heights[i]`
tall: the two arrays describe the same singer at the same index. No two
singers are equally tall, so the row is fully decided.

Return the names as they read along the row from left to right: the
tallest singer's name first, then the next tallest, down to the shortest.
Each name is written exactly as it was given, letter case included.

Names are not unique. Two singers who share a name are still two singers,
each placed by their own height, so the name appears once for each of them.

@examples

## Constraints

- `n == names.length == heights.length`
- `1 <= n <= 10^3`
- `1 <= names[i].length <= 20`
- `names[i]` consists of lowercase and uppercase English letters.
- `1 <= heights[i] <= 10^5`
- All the values of `heights` are different.
