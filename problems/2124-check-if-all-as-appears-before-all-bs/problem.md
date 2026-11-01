id: 2124
slug: check-if-all-as-appears-before-all-bs
title: Check if All A's Appears Before All B's
difficulty: easy
tags: string
signature: bool checkString(string s)
---
`s` is a string of the letters `a` and `b`. Can it be cut at a single
place into a left part with no `b` in it and a right part with no `a` in
it? The cut may also fall before the first letter or after the last one,
so either part is allowed to be empty.

Return `true` if such a cut exists, and `false` if it doesn't. Put the
other way round, the answer is `false` exactly when some `b` has an `a`
somewhere to its right, whether next to it or further along.

@examples

## Constraints

- `1 <= s.length <= 100`
- Every character of `s` is `a` or `b`.
