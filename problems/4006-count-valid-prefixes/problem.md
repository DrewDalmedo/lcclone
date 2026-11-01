id: 4006
slug: count-valid-prefixes
title: Count Valid Prefixes
difficulty: easy
tags: string, counting
signature: int countValidPrefixes(string s)
---
`s` is a string of the characters `0` and `1`. Its prefixes are the
strings made of its first `k` characters, for each `k` from `1` to
`s.length`: the whole of `s` is one of them, and the empty string is not.

A prefix is **valid** if its characters can be laid out in a row in which
every character differs from the one right after it. Any order of the
characters may be used, including the order they already have. For
example, `"0011"` is valid, since it can be laid out as `"0101"`, but
`"0111"` is not. A prefix of length 1 is always valid.

Return how many prefixes of `s` are valid.

@examples

## Constraints

- `1 <= s.length <= 100`
- Each `s[i]` is `'0'` or `'1'`.
