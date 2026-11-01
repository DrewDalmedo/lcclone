id: 455
slug: assign-cookies
title: Assign Cookies
difficulty: easy
tags: greedy, array, two-pointers, sorting
signature: int findContentChildren(int[] g, int[] s)
---
A group of children is waiting for cookies, and you have a tray of
cookies to share among them. Child `i` is picky: it is **content** only
with a cookie whose size is `g[i]` or more. The size of cookie `j` is
`s[j]`.

You choose who gets which cookie, with two rules: a child gets at most one
cookie, and a cookie goes to at most one child. Some children may get no
cookie, and some cookies may be left on the tray. A child given a cookie
smaller than `g[i]` is not content.

Return the largest number of children you can make content.

Neither array is in any particular order, and either may hold repeated
values.

@examples

## Constraints

- `1 <= g.length <= 3 * 10^4`
- `0 <= s.length <= 3 * 10^4`
- `1 <= g[i], s[j] <= 2^31 - 1`
