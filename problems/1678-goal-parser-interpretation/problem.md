id: 1678
slug: goal-parser-interpretation
title: Goal Parser Interpretation
difficulty: easy
tags: string
signature: string interpret(string command)
---
`command` is written in a tiny shorthand made of three tokens, `G`, `()`
and `(al)`, which may appear in any order and any number of times, packed
together without separators. Each token stands for some letters:

- `G` stands for the capital letter `G`;
- `()` stands for the small letter `o`;
- `(al)` stands for the two small letters `al`.

Expand `command`: replace every token by the letters it stands for, keep
the tokens' order, and return the resulting string.

There is only one way to split `command` into tokens: each `G` is a token
on its own, and the character after a `(` says which token it opens: a
`)` means `()`, and an `a` means `(al)`.

@examples

## Constraints

- `1 <= command.length <= 100`
- `command` is made of the tokens `G`, `()` and `(al)` and nothing else.
