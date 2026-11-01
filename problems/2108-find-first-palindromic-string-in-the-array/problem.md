id: 2108
slug: find-first-palindromic-string-in-the-array
title: Find First Palindromic String in the Array
difficulty: easy
tags: array, two-pointers, string
signature: string firstPalindrome(string[] words)
---
A word is a **palindrome** when it mirrors itself: its first letter equals
its last letter, its second letter equals its second-to-last, and so on,
pair by pair, until the pairs meet in the middle. In a word of odd length
the middle letter is paired with itself, so it can be anything; in
particular every one-letter word is a palindrome.

Go through `words` in order, starting at `words[0]`, and return the first
word you meet that is a palindrome, exactly as it is written. The words
after it don't matter, even ones that are longer palindromes. If no word
of `words` is a palindrome, return the empty string `""`.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 100`
- `words[i]` consists of lowercase English letters only.
