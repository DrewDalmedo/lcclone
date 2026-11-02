id: 844
slug: backspace-string-compare
title: Backspace String Compare
difficulty: easy
tags: stack, two-pointers, string, simulation
signature: bool backspaceCompare(string s, string t)
---
`s` and `t` are two keystroke logs. Each lists, in order, the keys someone
pressed while filling in a one-line input box that started out empty:

- a lowercase letter key adds that letter at the right end of the box;
- the `#` key is backspace: it removes the letter at the right end of the
  box. In an empty box it does nothing, and it is not saved up to remove a
  letter typed later.

The `#` key never puts a character in the box. Replay each log in its own
empty box, and return `true` if the two boxes end up holding exactly the
same text (the same letters in the same order), or `false` if they don't.
Two boxes that both end up empty hold the same text.

@examples

## Constraints

- `1 <= s.length <= 200`
- `1 <= t.length <= 200`
- `s` and `t` contain only lowercase English letters and `'#'`.

## Follow-up

Replaying the logs into two buffers takes `O(n)` extra memory. Can you
compare the final texts in `O(n)` time with only `O(1)` extra memory?
