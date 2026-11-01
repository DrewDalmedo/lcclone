id: 3019
slug: number-of-changing-keys
title: Number of Changing Keys
difficulty: easy
tags: string
signature: int countKeyChanges(string s)
---
Someone typed the string `s` on a keyboard, one letter at a time, from left
to right. Every letter of the alphabet has a single key, which types that
letter in lowercase or in uppercase: the case of a letter never changes
which key was pressed, and nothing but letter keys is counted. So `'r'` and
`'R'` are the same key, while `'r'` and `'t'` are two different keys.

A keystroke is a **key change** when its key differs from the key of the
keystroke just before it. The first keystroke is never a key change, since
no keystroke comes before it. Only neighbours are compared: returning to a
key that was used earlier, after some other key, is a key change again.

Return the number of key changes made while typing `s`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of uppercase and lowercase English letters only.
