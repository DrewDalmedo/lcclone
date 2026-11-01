id: 290
slug: word-pattern
title: Word Pattern
difficulty: easy
tags: hash-table, string
signature: bool wordPattern(string pattern, string s)
---
`s` is a sentence: words made of lowercase letters, with a single space
between one word and the next. `pattern` is a string of lowercase letters,
to be read as the outline of a sentence, one letter per word. Where the
outline repeats a letter, the sentence must repeat the word; where it uses
another letter, the sentence must use another word.

More exactly, line the words of `s` up with the letters of `pattern`: the
first word with the first letter, the second word with the second letter,
and so on. `s` **matches** `pattern` when

- `s` has exactly as many words as `pattern` has letters, and
- two positions that hold the same letter always hold the same word, and
  two positions that hold different letters always hold different words.

Return `true` if `s` matches `pattern`, and `false` if it doesn't.

Words are equal only when they are the same whole string: `"tap"`,
`"tape"` and `"tip"` are three different words. A letter of `pattern` is
never compared with a word of `s`, so the letter `b` may well line up with
the word `"a"`, or with `"b"`.

@examples

## Constraints

- `1 <= pattern.length <= 300`
- `pattern` consists of lowercase English letters.
- `1 <= s.length <= 3000`
- `s` consists of lowercase English letters and spaces.
- `s` doesn't begin or end with a space, and its words are separated by
  single spaces.
