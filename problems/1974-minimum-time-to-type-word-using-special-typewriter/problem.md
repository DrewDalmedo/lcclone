id: 1974
slug: minimum-time-to-type-word-using-special-typewriter
title: Minimum Time to Type Word Using Special Typewriter
difficulty: easy
tags: greedy, string
signature: int minTimeToType(string word)
---
A label maker has a round dial with the 26 lowercase letters spaced evenly
around its rim in alphabetical order, so the rim closes up with `'z'`
sitting right between `'y'` and `'a'`. A marker points at one letter of
the dial, and when you begin it points at `'a'`.

Each second you do exactly one of these two things:

- turn the dial by one notch, in either direction, so that the marker
  now points at one of the two letters next to the one it pointed at;
- press the print key, which prints the letter the marker points at and
  leaves the dial where it is.

Return the smallest number of seconds in which you can print the letters of
`word`, one after another from the first to the last. The dial may be left
pointing anywhere at the end.

@examples

## Constraints

- `1 <= word.length <= 100`
- `word` consists of lowercase English letters.
