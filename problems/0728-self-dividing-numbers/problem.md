id: 728
slug: self-dividing-numbers
title: Self Dividing Numbers
difficulty: easy
tags: math
signature: int[] selfDividingNumbers(int left, int right)
---
Take a positive integer and divide it by each of its own decimal digits in
turn. If none of these divisions leaves a remainder, the number is
**self-dividing**. For instance, `36` is self-dividing, as `36 / 3 = 12`
and `36 / 6 = 6`, but `26` is not: `26 / 2` comes out even, while `26 / 6`
leaves `2` over.

Nothing can be divided by `0`, so a number with the digit `0` anywhere in
it, as in `20` or `405`, is never self-dividing. Every number from `1` to
`9` is, because it divides itself.

Return all the self-dividing numbers from `left` up to `right`, both ends
included, in increasing order, or an empty array if there are none.

@examples

## Constraints

- `1 <= left <= right <= 10^4`
