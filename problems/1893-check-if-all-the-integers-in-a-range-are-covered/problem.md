id: 1893
slug: check-if-all-the-integers-in-a-range-are-covered
title: Check if All the Integers in a Range Are Covered
difficulty: easy
tags: array, hash-table, prefix-sum
signature: bool isCovered(int[][] ranges, int left, int right)
---
A fence has posts numbered `1`, `2`, `3`, and so on. Several painters have
worked on it, and `ranges` records their work: `ranges[i] = [s, e]` means
that one painter painted every post from number `s` to number `e`,
including both of those posts. Painters may have painted overlapping
stretches, the same stretch twice, or a single post (`s == e`), and the
records are in no particular order.

Return `true` if every post whose number lies between `left` and `right`
(both included) was painted by at least one painter, and `false` if at
least one of those posts was missed. Posts outside that stretch don't
matter.

Only whole posts count: there is nothing between post `b` and post `b + 1`,
so stretches `[a, b]` and `[b + 1, c]` together paint every post from `a`
to `c`.

@examples

## Constraints

- `1 <= ranges.length <= 50`
- `ranges[i].length == 2`
- `1 <= s <= e <= 50` for every `ranges[i] = [s, e]`
- `1 <= left <= right <= 50`
