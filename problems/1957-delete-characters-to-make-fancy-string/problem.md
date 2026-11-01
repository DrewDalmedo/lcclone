id: 1957
slug: delete-characters-to-make-fancy-string
title: Delete Characters to Make Fancy String
difficulty: easy
tags: string
signature: string makeFancyString(string s)
---
A string is **fancy** if no letter appears in it three times in a row: any
three neighbouring characters include at least two different letters. So
`"tool"` is fancy, but `"toool"` is not.

Strike out as few characters of `s` as you can, so that the characters left,
kept in their original order, spell a fancy string. Return that string.

There may be several ways to strike that few characters, but they all
leave the same string, so the answer is well defined. If `s` is fancy
already, nothing is struck and the answer is `s` itself.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- `s` consists of lowercase English letters.
