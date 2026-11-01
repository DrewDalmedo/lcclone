id: 3330
slug: find-the-original-typed-string-i
title: Find the Original Typed String I
difficulty: easy
tags: string
signature: int possibleStringCount(string word)
---
A string `s` was sent down a faulty line, and `word` is what came out. The
line has a single kind of fault, an **echo**: one character of `s` comes
out followed by one or more extra copies of itself. An echo happened at
most once during the whole transmission, perhaps not at all. Apart from
that, each character of `s` came out once, in its place.

For example, `"tap"` can come out as `"tap"` (no echo), `"ttap"`,
`"taaaap"` or `"tapp"`, but not as `"ttapp"`, which needs two echoes.

Return the number of different strings `s` that could have been sent to
produce `word`.

@examples

## Constraints

- `1 <= word.length <= 100`
- `word` consists of lowercase English letters.
