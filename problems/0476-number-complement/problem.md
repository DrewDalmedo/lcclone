id: 476
slug: number-complement
title: Number Complement
difficulty: easy
tags: bit-manipulation
signature: int findComplement(int num)
---
Picture `num` as the row of its binary digits, from its highest `1` down
to its last digit. Put the opposite digit in every place of that row and
read the new row as a binary number: its value is the **complement** of
`num`. Return it.

The row starts at the highest `1`: the zeros above it, such as the leading
zeros of a 32-bit `int`, are not part of it and are never flipped. So the
complement is not the bitwise NOT `~num`, and it is never negative. The
first digit of the row is a `1` that becomes a `0`, which makes the
complement always smaller than `num`.

@examples

## Constraints

- `1 <= num <= 2^31 - 1`
