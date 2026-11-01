id: 1544
slug: make-the-string-great
title: Make The String Great
difficulty: easy
tags: stack, string
signature: string makeGood(string s)
---
The string `s` is made of English letters, small and capital. Two
characters side by side **clash** when they are one letter written in its
two cases: `q` followed by `Q`, or `Q` followed by `q`. Nothing else
clashes: not two copies of a letter in the same case (`qq`, `QQ`), and not
two different letters, whatever their cases (`qR`, `Rq`).

As long as `s` contains a clashing pair, erase one such pair, both of its
characters. Erasing it brings the characters around it together, so they may
clash in turn. When no clashing pair is left, return what is left of `s`.

However you choose the pairs to erase, you end with the same string. The
characters that remain keep their cases and their order from `s`. If every
character gets erased, return the empty string `""`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase and uppercase English letters only.
