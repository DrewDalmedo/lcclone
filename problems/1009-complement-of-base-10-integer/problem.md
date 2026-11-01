id: 1009
slug: complement-of-base-10-integer
title: Complement of Base 10 Integer
difficulty: easy
tags: bit-manipulation
signature: int bitwiseComplement(int n)
---
The number `n` is given in decimal. Convert it to the shortest string of
binary digits that writes it: the string starts with a `1`, except for
`n = 0`, whose binary form is the single digit `0`.

Swap every digit of that string for the other binary digit, and convert
the new string back to decimal. That number is the **complement** of `n`;
return it. Zeros that the swap leaves at the front of the string count for
nothing, so `0011` is read as `3`. In particular, the complement of `0` is
`1`.

Only the digits of the string change. A 32-bit `int` also stores zero bits
in front of them; `~n` would set those as well and come out negative, which
is not the answer asked for here.

@examples

## Constraints

- `0 <= n < 10^9`
