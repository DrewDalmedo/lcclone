id: 2578
slug: split-with-minimum-sum
title: Split With Minimum Sum
difficulty: easy
tags: greedy, math, sorting
signature: int splitNum(int num)
---
Make one card for every decimal digit of `num`, repeats included: `3003`
would give the four cards `3`, `0`, `0` and `3`. Deal all the cards into two
piles, with at least one card in each. Then lay each pile's cards out in a
row, in any order you like, and read the row as a decimal number. The order
the digits had in `num` plays no part.

Zeros at the front of a row are allowed and simply count for nothing, so
the row `0` `0` `5` is worth `5`.

Return the smallest sum the two numbers can have.

@examples

## Constraints

- `10 <= num <= 10^9`
