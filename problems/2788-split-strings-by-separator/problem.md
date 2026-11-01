id: 2788
slug: split-strings-by-separator
title: Split Strings by Separator
difficulty: easy
tags: array, string
signature: string[] splitWordsBySeparator(string[] words, char separator)
---
`separator` is a punctuation mark that marks boundaries in text. In
each string of `words`, call a **chunk** any stretch of one or more
consecutive characters, none of them `separator`, that is as long as it
can be: on each side it reaches either an end of the string or a
`separator`.

Return the chunks of `words[0]` from left to right, followed by the
chunks of `words[1]` from left to right, and so on through the last
string, all in one array. A chunk that repeats one listed earlier is
listed again.

- The `separator` characters themselves belong to no chunk. A string
  made only of separators has no chunks, and if no string has any,
  return an empty array.
- A chunk lies within one string: the last chunk of one string and the
  first chunk of the next stay apart, even with no separator between
  them.
- Only `separator` marks boundaries. Every other punctuation mark is an
  ordinary character that stays inside its chunk, and a chunk may
  consist of such marks alone.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 20`
- Each character of `words[i]` is a lowercase English letter or one of
  the marks `.,|$#@`
- `separator` is one of the marks `.,|$#@`
