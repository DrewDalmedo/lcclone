id: 3014
slug: minimum-number-of-pushes-to-type-word-i
title: Minimum Number of Pushes to Type Word I
difficulty: easy
tags: greedy, math, string
signature: int minimumPushes(string word)
---
You want to type `word` on a phone whose keypad you may label yourself.
Only the eight keys `2` to `9` can carry letters. You hand each letter of
`word` to exactly one of them, and line up the letters of each key in an
order of your choice. A key may carry any number of letters, or none;
letters that don't occur in `word` need no key.

A letter that stands `k`-th in its key's line is typed by pressing that
key `k` times: once for the first letter, twice for the second, and so on.

No letter occurs twice in `word`. Return the smallest number of key presses
that typing `word` can take, over every way of labelling the keys.

@examples

## Constraints

- `1 <= word.length <= 26`
- `word` consists of lowercase English letters.
- All letters of `word` are different.
