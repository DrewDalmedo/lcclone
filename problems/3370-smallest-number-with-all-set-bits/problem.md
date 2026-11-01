id: 3370
slug: smallest-number-with-all-set-bits
title: Smallest Number With All Set Bits
difficulty: easy
tags: bit-manipulation, math
signature: int smallestNumber(int n)
---
Write a positive integer in base 2, starting at its highest `1` (no
leading zeros). Call the integer **all-ones** when that writing has no `0`
digit anywhere. The all-ones integers are `1`, `3`, `7`, `15`, `31`, ...,
written `1`, `11`, `111`, `1111`, `11111`, ...

Return the smallest all-ones integer that is greater than or equal to `n`.
If `n` is all-ones itself, the answer is `n`.

@examples

## Constraints

- `1 <= n <= 1000`
