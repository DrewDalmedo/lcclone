id: 3582
slug: generate-tag-for-video-caption
title: Generate Tag for Video Caption
difficulty: easy
tags: string, simulation
signature: string generateTag(string caption)
---
A video site files each upload under a short **tag** that it makes from the
upload's `caption`. The caption holds English letters and spaces. Its
**words** are its maximal runs of letters, read from left to right. Spaces
do nothing but separate words: spaces at the start or end of `caption`, or
several spaces in a row, never make an empty word. So the first word is the
leftmost run of letters, however many spaces come before it.

The tag is the character `#` followed by every word of the caption, in
order, with nothing in between. Each word is restyled on the way:

- every letter of the first word becomes a small letter, even a capital at
  its start;
- in each later word, the first letter becomes a capital and all the
  others become small letters.

The tag may be at most 100 characters long, the `#` included. A longer one
loses everything after its 100th character, even if that cuts a word in two,
so at most 99 letters remain.

Return the tag. A caption made only of spaces has no words, and its tag is
just `"#"`.

@examples

## Constraints

- `1 <= caption.length <= 150`
- `caption` consists of English letters (`a`-`z`, `A`-`Z`) and spaces
  `' '`.
