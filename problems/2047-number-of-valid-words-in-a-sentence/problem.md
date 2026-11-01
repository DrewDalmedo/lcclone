id: 2047
slug: number-of-valid-words-in-a-sentence
title: Number of Valid Words in a Sentence
difficulty: easy
tags: string
signature: int countValidWords(string sentence)
---
Cut `sentence` at its spaces into **chunks**: a chunk is a longest stretch
of characters with no space in it. Spaces only keep chunks apart and never
belong to one. Two chunks may have one space or several between them, and
`sentence` may also start or end with spaces.

A chunk is a **valid word** when it reads as an optional *stem* followed by
an optional *closing mark*, and nothing else:

- a stem is one or more lowercase letters, which may be followed by a
  hyphen `'-'` and one or more further lowercase letters;
- a closing mark is a single `'!'`, `'.'` or `','`.

Return how many chunks of `sentence` are valid words.

Put another way, a valid word holds no digit; it has at most one hyphen,
with a letter immediately before it and immediately after it; and it has
at most one of `'!'`, `'.'` and `','`, which can only be its last
character. So a chunk that is just `","` is a valid word, but a chunk that
is just `"-"` is not, and neither is `"on-!"`, since its hyphen has no
letter after it.

@examples

## Constraints

- `1 <= sentence.length <= 1000`
- `sentence` consists of lowercase English letters, digits, spaces and the
  characters `'-'`, `'!'`, `'.'` and `','`.
- `sentence` holds at least one chunk.
