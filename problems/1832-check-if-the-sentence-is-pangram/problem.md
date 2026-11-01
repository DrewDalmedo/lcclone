id: 1832
slug: check-if-the-sentence-is-pangram
title: Check if the Sentence Is Pangram
difficulty: easy
tags: hash-table, string
signature: bool checkIfPangram(string sentence)
---
Write the alphabet out, `a` to `z`. Then go through `sentence` character
by character, and put a tick beside the alphabet's copy of each letter
that turns up.

`sentence` is a **pangram** when, at the end, all 26 letters of the
alphabet carry a tick. How often a letter occurs, and where, doesn't
matter. Return `true` if `sentence` is a pangram, and `false` if at least
one letter is left without a tick.

In spite of its name, `sentence` is a single run of lowercase letters,
with no spaces or punctuation.

@examples

## Constraints

- `1 <= sentence.length <= 1000`
- `sentence` consists of lowercase English letters only.
