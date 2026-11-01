id: 541
slug: reverse-string-ii
title: Reverse String II
difficulty: easy
tags: two-pointers, string
signature: string reverseStr(string s, int k)
---
Starting at the left end of `s`, split it into groups of `k` consecutive
characters. The last group takes whatever is left over, so it may hold
fewer than `k`.

Number the groups 1, 2, 3, ... from the left. Reverse the characters within
each odd-numbered group (the 1st, the 3rd, the 5th, ...), leave every
even-numbered group exactly as it is, and return the string the groups make
when they are joined back together in their original order.

So when `k` is at least the length of `s`, the whole string is one group
and comes back reversed; when `k` is 1, nothing changes.

@examples

## Constraints

- `1 <= s.length <= 10^4`
- `s` consists of lowercase English letters only.
- `1 <= k <= 10^4`
