id: 3931
slug: check-adjacent-digit-differences
title: Check Adjacent Digit Differences
difficulty: easy
tags: string
signature: bool isAdjacentDiffAtMostTwo(string s)
---
Picture `s` as the log of a gauge that shows a single digit, read once a
second: character `i` is the reading at second `i`, taken as its value from
`0` to `9`. Going from one reading to the next is **smooth** when the value
rises or falls by at most 2, that is, when the two digits differ by 0, 1
or 2.

Return `true` if each of the `s.length - 1` moves, from `s[0]` to `s[1]`,
from `s[1]` to `s[2]` and so on up to the last character, is smooth.
Return `false` if at least one of them jumps up or down by 3 or more.

Only readings that stand side by side are compared. The last one is not
compared with the first, and readings further apart may differ by any
amount.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of the digits `0` to `9` only.
