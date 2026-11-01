id: 1078
slug: occurrences-after-bigram
title: Occurrences After Bigram
difficulty: easy
tags: string
signature: string[] findOcurrences(string text, string first, string second)
---
A phone keyboard learns what to suggest after a two-word phrase by reading
what you have typed before. `text` is that typing: lowercase words, each
pair of neighbouring words separated by one space.

Find every place in `text` where the word `first` is immediately followed
by the word `second`. When another word comes right after such a pair, it
is a **follower**. Return every follower in the order they appear in
`text`, listing a word once for each time it is a follower. If there are
none, return an empty array.

Words match only as whole words: `"art"` is not the word `"arts"`, nor the
end of `"cart"`. The places may overlap, so a follower can itself be the
`first` or `second` word of a later place, and `first` and `second` may be
the same word.

@examples

## Constraints

- `1 <= text.length <= 1000`
- `text` consists of lowercase English letters and spaces.
- `text` has no space at its start or end, and never two spaces in a row.
- `1 <= first.length, second.length <= 10`
- `first` and `second` consist of lowercase English letters.
