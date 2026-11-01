id: 405
slug: convert-a-number-to-hexadecimal
title: Convert a Number to Hexadecimal
difficulty: easy
tags: bit-manipulation, math, string
signature: string toHex(int num)
---
Return `num` written in base 16, as a string of hexadecimal digits. The
digit values zero to nine are written `0` to `9`, and ten to fifteen are
written with the lowercase letters `a` to `f`. The most significant digit
comes first. Leave out leading zeros, so that `num = 0` is the only number
whose answer starts with `0`: its answer is `"0"`. Add nothing else: no
sign, and no `0x` in front.

A negative `num` is written through its 32 bits in two's complement: those
same bits, read as an unsigned number, have the value `num + 2^32`, and
that value is what gets written in base 16. So every negative `num` comes
out as exactly eight digits, of which the first is `8` or more.

Do the conversion with your own arithmetic or bit operations. A library
routine that writes hexadecimal for you, such as `printf` with `%x` or
C++'s `std::hex`, defeats the purpose, even though lc only checks the
string you return.

@examples

## Constraints

- `-2^31 <= num <= 2^31 - 1`
