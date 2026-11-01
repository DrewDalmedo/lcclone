id: 2490
slug: circular-sentence
title: Circular Sentence
difficulty: easy
tags: string
signature: bool isCircularSentence(string sentence)
---
The string `sentence` records one round of a word-chain game: the words in
the order they were said, each word but the last followed by a single
space, and no other spaces. Each word is one or more English letters,
capital or small.

The round is a **ring** when both of these hold:

- every word after the first begins with the letter that the word before
  it ends with;
- the last word ends with the letter that the first word begins with, so
  the chain comes back round to its start.

Return `true` if the round is a ring, and `false` otherwise. Letters match
only when they are exactly the same, case included: `'M'` and `'m'` don't
match. With a single word only the second rule applies, so that word must
begin and end with the same letter.

@examples

## Constraints

- `1 <= sentence.length <= 500`
- `sentence` holds only English letters, capital or small, and spaces.
- `sentence` begins and ends with a letter, and every space has a letter
  on each side of it.
