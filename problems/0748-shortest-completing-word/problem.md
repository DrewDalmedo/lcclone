id: 748
slug: shortest-completing-word
title: Shortest Completing Word
difficulty: easy
tags: array, hash-table, string
signature: string shortestCompletingWord(string licensePlate, string[] words)
---
A game for long drives: read the letters off a passing car's license
plate, then name a word that uses all of them. Here the plate is
`licensePlate`, and the words you may name are listed in `words`.

Only the plate's letters matter, and a capital counts as its small
letter: an `A` on the plate asks for the same thing as an `a`, while a
digit or a space asks for nothing. A word **completes** the plate when
no letter turns up fewer times in the word than on the plate, counted
that way. The order of the letters makes no difference, and the word
may hold letters the plate never asked for. For instance, the plate
`"7Ab A"` asks for `a` twice and `b` once: `"cabal"` completes it, while
`"crab"`, which has only one `a`, does not.

The answer is a word of `words` that completes the plate, chosen by
length first and position second: the fewest letters wins, and among the
completing words of that length, the one nearest the start of `words`.
A plate with no letters at all asks for nothing, so then every word
completes it.

@examples

## Constraints

- `1 <= licensePlate.length <= 7`
- `licensePlate` consists of digits, lowercase and uppercase English
  letters, and spaces `' '`.
- `1 <= words.length <= 1000`
- `1 <= words[i].length <= 15`
- `words[i]` consists of lowercase English letters.
- At least one word of `words` completes the plate.
