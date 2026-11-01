id: 2309
slug: greatest-english-letter-in-upper-and-lower-case
title: Greatest English Letter in Upper and Lower Case
difficulty: easy
tags: hash-table, string, enumeration
signature: string greatestLetter(string s)
---
`s` is made of English letters, some of them capitals and some small. A
letter of the alphabet is **doubled** in `s` when `s` has it in both
forms: the capital, such as `K`, somewhere, and the small one, `k`,
somewhere. It does not matter where the two forms stand, or how many
copies of each there are.

Of the doubled letters, find the one that comes last in the alphabet, and
return it as a one-character string holding its **capital** form: the
answer for `k` is `"K"`, whichever form appears first or more often.

If no letter is doubled in `s`, return the empty string `""`.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists of English letters, `a` to `z` and `A` to `Z`.
