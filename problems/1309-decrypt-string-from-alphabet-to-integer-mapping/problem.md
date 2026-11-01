id: 1309
slug: decrypt-string-from-alphabet-to-integer-mapping
title: Decrypt String from Alphabet to Integer Mapping
difficulty: easy
tags: string
signature: string freqAlphabets(string s)
---
Give every lowercase letter a number: `a` is 1, `b` is 2, and so on up to
`z`, which is 26. A word is encoded by replacing each of its letters, in
order, with that letter's number, and writing the pieces one after another
with nothing between them:

- a letter numbered 1 to 9 (`a` to `i`) becomes its single digit;
- a letter numbered 10 to 26 (`j` to `z`) becomes its two digits followed
  by a `#`.

So `"bag"` is encoded as `"217"`, and `"quiz"` as `"17#21#926#"`.

You are given such an encoding, `s`. Return the word it came from.

Reading `s` back is never ambiguous: each `#` closes a number made of
exactly the two digits just before it, and every other digit is a letter
on its own. So `"25"` stands for `"be"`, while `"25#"` stands for `"y"`.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists of digits and `#` only.
- `s` is the encoding of some word of lowercase English letters.
