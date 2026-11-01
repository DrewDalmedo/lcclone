id: 1614
slug: maximum-nesting-depth-of-the-parentheses
title: Maximum Nesting Depth of the Parentheses
difficulty: easy
tags: stack, string
signature: int maxDepth(string s)
---
The string `s` is written with digits, the signs `+`, `-`, `*` and `/`, and
round brackets. Its brackets pair up properly: each `)` belongs to the
nearest `(` on its left that has no partner yet, and no bracket of either
kind is left without a partner. Nothing else is promised about `s`: the
digits and signs may come in any order, so `s` need not be a formula that
makes sense, and a pair may hold nothing at all, as in `()`.

A pair sits **within** another pair when both of its brackets lie between
that pair's two brackets. Return the length of the longest chain of pairs in
which every pair sits within the one before it. In other words, find how
many layers of brackets the most deeply buried spot of `s` is under. An
empty pair `()` is a layer like any other, and a string with no brackets
has the answer `0`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of the digits `0` to `9` and the characters `+`, `-`, `*`,
  `/`, `(` and `)`.
- The brackets of `s` pair up as described above.
