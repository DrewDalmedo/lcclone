id: 744
slug: find-smallest-letter-greater-than-target
title: Find Smallest Letter Greater Than Target
difficulty: easy
tags: array, binary-search
signature: char nextGreatestLetter(char[] letters, char target)
---
`letters` is a list of lowercase letters sorted in alphabetical order, and
it holds at least two different letters. `target` is a lowercase letter.

Return the smallest letter in `letters` that comes strictly after `target`
in the alphabet. If no letter in `letters` comes after `target`, return the
first letter of `letters` instead.

@examples

## Constraints

- `2 <= letters.length <= 10^4`
- `letters[i]` is a lowercase English letter.
- `letters` is sorted in non-decreasing order.
- `letters` contains at least two different letters.
- `target` is a lowercase English letter.

## Follow-up

The list is sorted. Can you find the answer in `O(log n)` time?
