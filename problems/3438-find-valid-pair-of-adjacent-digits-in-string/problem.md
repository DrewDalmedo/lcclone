id: 3438
slug: find-valid-pair-of-adjacent-digits-in-string
title: Find Valid Pair of Adjacent Digits in String
difficulty: easy
tags: hash-table, string, counting
signature: string findValidPair(string s)
---
`s` is a string of the digits `1` to `9`. Call a digit **complete** when
the number of times it occurs in `s` equals its own value: a `1` that
occurs once, a `2` that occurs twice, and so on, up to a `9` that occurs
nine times. Occurrences are counted over the whole of `s`, wherever they
stand, and the count has to hit the value exactly, so a digit that occurs
one time too many or one time too few is not complete.

Now walk along `s` from the left and look at each pair of neighbouring
characters, `s[i]` and `s[i + 1]`. The pair is **good** when its two
digits are different and both of them are complete.

Return the first good pair, the one with the smallest `i`, as a string of
its two characters in the order they stand in `s`. If `s` has no good
pair, return the empty string `""`.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of the digits `1` to `9` only.
