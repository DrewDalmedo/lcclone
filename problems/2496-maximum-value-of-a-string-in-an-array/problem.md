id: 2496
slug: maximum-value-of-a-string-in-an-array
title: Maximum Value of a String in an Array
difficulty: easy
tags: array, string
signature: int maximumValue(string[] strs)
---
Every entry of `strs` is a short code made of lowercase letters and
digits. Each code is worth some points, by one of two rules:

- A code written **only in digits** is read as a whole number in decimal,
  and is worth that number. Leading zeros change nothing: `"0042"` is
  worth `42`, and `"000"` is worth `0`.
- A code with **at least one letter**, anywhere in it, is worth the
  number of characters it has, digits included: `"q8"` is worth `2`, and
  `"31zz07"` is worth `6`.

The rules never mix. A digit-only code is never scored by its length, and
the digits of a code that holds a letter are never read as a number.

Return the largest worth among all the codes in `strs`.

@examples

## Constraints

- `1 <= strs.length <= 100`
- `1 <= strs[i].length <= 9`
- `strs[i]` consists of lowercase English letters and digits only.
