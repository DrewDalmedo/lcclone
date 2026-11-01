id: 66
slug: plus-one
title: Plus One
difficulty: easy
tags: array, math
signature: int[] plusOne(int[] digits)
---
The array `digits` holds a non-negative whole number written in decimal, one
digit per element, with the most significant digit first: `[4,0,7]` stands
for four hundred and seven. The number never has extra zeros in front, so
`digits[0]` is `0` only when the number is zero itself and `digits` is
`[0]`.

Return the digits of that number plus one, in the same form: most
significant digit first, with no zeros in front. The answer is one digit
longer than `digits` when every digit is a `9`.

`digits` can be up to 100 digits long, far more than any built-in integer
type can hold.

@examples

## Constraints

- `1 <= digits.length <= 100`
- `0 <= digits[i] <= 9`
- `digits` has no leading zeros, except for the number zero, `[0]`.
