id: 125
slug: valid-palindrome
title: Valid Palindrome
difficulty: easy
tags: two-pointers, string
signature: bool isPalindrome(string s)
---
Clean up the string `s` in two steps:

1. Keep only its letters (`A`-`Z` and `a`-`z`) and digits (`0`-`9`), in
   the order they come, and drop every other character: spaces,
   punctuation, `_` and all the rest.
2. Change each uppercase letter that is left into the same letter in
   lowercase.

Return `true` if the cleaned-up text is the same backwards as forwards,
and `false` if it is not.

Digits are kept just as they are, so a digit only ever matches the same
digit, never a letter. If step 1 drops every character, the cleaned-up
text is empty. Reversing an empty text changes nothing, so the answer is
then `true`.

@examples

## Constraints

- `1 <= s.length <= 2 * 10^5`
- `s` consists of printable ASCII characters (codes 32 to 126, the space
  included).

## Follow-up

Can you decide it without building the cleaned-up text, using `O(1)`
extra memory?
