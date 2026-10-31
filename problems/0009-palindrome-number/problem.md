id: 9
slug: palindrome-number
title: Palindrome Number
difficulty: easy
tags: math
signature: bool isPalindrome(int x)
---
Write the integer `x` in decimal, with a leading `-` if it is negative.
Return `true` if those characters read the same from right to left as from
left to right, and `false` otherwise.

A negative number never qualifies, because its `-` sign would end up at the
wrong end. Zero and every single-digit number do.

@examples

## Constraints

- `-2^31 <= x <= 2^31 - 1`

## Follow-up

Turning `x` into a string makes this easy. Can you decide it with arithmetic
on the number alone, without running into overflow?
