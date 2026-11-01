id: 67
slug: add-binary
title: Add Binary
difficulty: easy
tags: bit-manipulation, math, string, simulation
signature: string addBinary(string a, string b)
---
The strings `a` and `b` each hold a non-negative whole number written in
base 2, as characters `'0'` and `'1'` with the most significant bit first:
`"110"` stands for six.

Return their sum, written the same way. The answer has no zeros in front of
its first `'1'` and nothing else around the bits (no `"0b"`, no spaces); a
sum of zero is written `"0"`.

The two numbers may have different lengths, and each can be up to `10^4`
bits long, far more than any built-in integer type can hold.

@examples

## Constraints

- `1 <= a.length, b.length <= 10^4`
- Every character of `a` and `b` is `'0'` or `'1'`.
- `a` and `b` each start with `'1'`, unless the string is exactly `"0"`.
