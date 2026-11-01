id: 2843
slug: count-symmetric-integers
title: Count Symmetric Integers
difficulty: easy
tags: math, enumeration
signature: int countSymmetricIntegers(int low, int high)
---
The tickets of a raffle are numbered consecutively: the first carries
`low`, the last carries `high`, and no number in between is skipped or used
twice. The numbers are printed in plain decimal, with no leading zeros.

A ticket is **symmetric** when the digits of its number split into a
front group and a back group of the same size, and the two groups have the
same digit total. For instance, `1854` is symmetric, since
`1 + 8 = 5 + 4`, and so is `77`, but `52` and `3601` are not.

Such a split needs an even count of digits. So a number whose length is
odd, like `7` or `393`, can't qualify, and no leading `0` is added to even
out its length.

Return how many of the tickets are symmetric.

@examples

## Constraints

- `1 <= low <= high <= 10^4`
