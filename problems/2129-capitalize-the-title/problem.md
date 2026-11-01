id: 2129
slug: capitalize-the-title
title: Capitalize the Title
difficulty: easy
tags: string
signature: string capitalizeTitle(string title)
---
`title` is a headline made of words, with exactly one space between each
word and the next. A word is a run of English letters, and its letters may
come in any mix of uppercase and lowercase.

Restyle every word according to how many letters it has:

- a word of one or two letters is written entirely in lowercase;
- a word of three or more letters starts with an uppercase letter, and all
  of its other letters are lowercase.

Return the restyled headline. Only the case of letters changes: the
letters keep their order and the spaces stay where they are. The rule looks
at length alone, not at where a word stands, so a word of one or two
letters at the start of the headline is lowercase as well.

@examples

## Constraints

- `1 <= title.length <= 100`
- `title` consists of English letters (`a`-`z`, `A`-`Z`) and spaces.
- `title` does not begin or end with a space, and never has two spaces in
  a row.
