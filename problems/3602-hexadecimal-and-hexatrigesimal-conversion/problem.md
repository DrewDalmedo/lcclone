id: 3602
slug: hexadecimal-and-hexatrigesimal-conversion
title: Hexadecimal and Hexatrigesimal Conversion
difficulty: easy
tags: math, string
signature: string concatHex36(int n)
---
Take the positive integer `n` and form two numerals from it:

- its square, `n * n`, written in base 16;
- its cube, `n * n * n`, written in base 36.

Return the base-16 numeral with the base-36 numeral appended right after
it, with no separator.

A numeral in base `b` is a string of digits `d_k ... d_1 d_0` standing for
the number `d_k * b^k + ... + d_1 * b + d_0`. Every digit value lies
between `0` and `b - 1`, and the leading digit `d_k` is never `0`, so a
positive number has exactly one numeral in each base. A digit value `d` is
written as the character at position `d`, counting from 0, of
`0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ`: base 16 uses the characters `0` to
`F`, base 36 all of them, and letters are always capitals. The answer is
made of digit characters alone: a prefix such as `0x` is not part of a
numeral here.

@examples

## Constraints

- `1 <= n <= 1000`
