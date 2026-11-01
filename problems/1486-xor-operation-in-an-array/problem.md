id: 1486
slug: xor-operation-in-an-array
title: XOR Operation in an Array
difficulty: easy
tags: bit-manipulation, math
signature: int xorOperation(int n, int start)
---
Count upward from `start` in steps of `2` and write down `n` numbers:
`start`, `start + 2`, `start + 4`, and so on, the last one being
`start + 2 * (n - 1)`. Counting them from `0`, number `i` on the list is
`start + 2 * i`.

Return the bitwise XOR of all `n` of them: the number that has a bit set
exactly where an odd number of them have that bit set. When `n` is `1`,
that is `start` itself.

@examples

## Constraints

- `1 <= n <= 1000`
- `0 <= start <= 1000`
