id: 28
slug: find-the-index-of-the-first-occurrence-in-a-string
title: Find the Index of the First Occurrence in a String
difficulty: easy
tags: two-pointers, string, string-matching
signature: int strStr(string haystack, string needle)
---
Search for the word `needle` inside the text `haystack`. `needle` is found
at index `i` (counting from 0) when its letters can be read off `haystack`
starting there: `haystack[i]` is `needle[0]`, `haystack[i + 1]` is
`needle[1]`, and so on up to the last letter of `needle`, all of them
inside `haystack`.

Return the smallest index where `needle` is found, or `-1` if it is found
nowhere. The letters must be next to each other in `haystack`, with nothing
between them: `"ace"` is not found in `"abcde"`.

@examples

## Constraints

- `1 <= haystack.length, needle.length <= 10^4`
- `haystack` and `needle` consist of lowercase English letters.

## Follow-up

Comparing `needle` with the text at every index can take
`O(haystack.length * needle.length)` time. Can you do it in
`O(haystack.length + needle.length)`?
