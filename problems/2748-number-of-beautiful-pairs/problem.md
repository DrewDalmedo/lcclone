id: 2748
slug: number-of-beautiful-pairs
title: Number of Beautiful Pairs
difficulty: easy
tags: array, hash-table, math, counting, number-theory
signature: int countBeautifulPairs(int[] nums)
---
Pick two positions `i < j` of `nums`. Take the **leading digit** of
`nums[i]`, the digit it starts with when written in decimal, and the
**units digit** of `nums[j]`, the digit it ends with. The pair of positions
is **beautiful** when those two digits are coprime, that is, when 1 is the
only positive whole number that goes evenly into both (their greatest
common divisor is 1). So 1 is coprime with every digit, while a digit
greater than 1 is never coprime with itself.

Return the number of beautiful pairs of positions in `nums`.

The digits are always taken this way round: the leading digit from the
earlier position and the units digit from the later one. Each pair of
positions is looked at once, two positions holding equal values still form
a pair of their own, and no position is paired with itself. A one-digit
number starts and ends with the same digit.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 9999`
- `nums[i] % 10 != 0`
