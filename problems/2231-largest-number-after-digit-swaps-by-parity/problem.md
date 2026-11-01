id: 2231
slug: largest-number-after-digit-swaps-by-parity
title: Largest Number After Digit Swaps by Parity
difficulty: easy
tags: sorting, heap-priority-queue
signature: int largestInteger(int num)
---
Split the ten decimal digits into two families: the odd ones `1, 3, 5, 7, 9`
and the even ones `0, 2, 4, 6, 8`.

Write `num` in decimal. You may rearrange its digits, but only by trading the
places of two digits from the same family: an odd digit with another odd
digit, or an even digit with another even digit. You can make as many trades
as you like, in any order, or none at all.

What counts is the family of the digit's **value**, not whether its position
is odd or even. A place that holds an odd digit always holds some odd digit,
and a place that holds an even digit always holds some even digit, but within
each family the digits can end up in any order.

Return the largest number you can reach.

@examples

## Constraints

- `1 <= num <= 10^9`
