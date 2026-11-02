id: 680
slug: valid-palindrome-ii
title: Valid Palindrome II
difficulty: easy
tags: greedy, two-pointers, string
signature: bool validPalindrome(string s)
---
A **palindrome** is a string equal to its own reverse: in a palindrome `t`,
`t[i] == t[t.length - 1 - i]` for every index `i`.

Return `true` if `s` is a palindrome already, or if taking a single,
well-chosen character out of it leaves one, and `false` if neither holds.
Taking a character out closes the gap: the characters after it move one
place to the left, and none of them is changed or swapped with another.
Only one character may be taken out, never two or more.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- `s` consists of lowercase English letters only.

## Follow-up

Taking out each character in turn and checking what is left takes
`O(n^2)` time. Can you decide it in `O(n)`?
