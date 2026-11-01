id: 1592
slug: rearrange-spaces-between-words
title: Rearrange Spaces Between Words
difficulty: easy
tags: string
signature: string reorderSpaces(string text)
---
The string `text` holds some words of lowercase letters with spaces around
them. A **word** is a run of letters that can't be made longer: it has a
space or an end of `text` on each side. Neighbouring words have at least one
space between them, and there may be spaces before the first word or after
the last one too.

Lay the line out again, keeping every word as it is and in the same order,
and keeping every space: none is added or removed, so the result has the
same length as `text`. Say `text` has `w` words and `s` spaces.

- The result starts with the first word, with no space before it.
- When `w >= 2`, each of the `w - 1` gaps between neighbouring words gets
  the same number of spaces, as large as it can be: `s / (w - 1)`, rounded
  down. The `s mod (w - 1)` spaces left over all go after the last word.
- When `w = 1`, all `s` spaces go after the only word.

Return the result.

@examples

## Constraints

- `1 <= text.length <= 100`
- `text` consists of lowercase English letters and spaces `' '`.
- `text` contains at least one word.
