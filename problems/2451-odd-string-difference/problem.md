id: 2451
slug: odd-string-difference
title: Odd String Difference
difficulty: easy
tags: array, hash-table, string
signature: string oddString(string[] words)
---
Number the lowercase letters by their place in the alphabet, starting from
zero: `a` is 0, `b` is 1, and so on up to `z`, which is 25. The **steps** of
a word `w` of length `n` are the `n - 1` numbers

`w[1] - w[0], w[2] - w[1], ..., w[n - 1] - w[n - 2]`

in that order, each one the number of a letter minus the number of the
letter before it. A step may be negative, and the alphabet does not wrap
around: going from `y` to `b` is a step of `-23`, and from `b` to `y` one of
`23`. For instance, `"cge"` and `"ptr"` both have the steps `4, -2`.

Every word in `words` has the same length `n`. One word is the odd one out:
its steps differ from those of the others in at least one place, while the
remaining words all have identical steps. Return the odd word, exactly as
it appears in `words`.

@examples

## Constraints

- `3 <= words.length <= 100`
- `2 <= words[i].length <= 20`, and every word has the same length.
- `words[i]` consists of lowercase English letters.
- Exactly one word has steps different from the others, and all the other
  words have the same steps as each other.
