id: 2094
slug: finding-3-digit-even-numbers
title: Finding 3-Digit Even Numbers
difficulty: easy
tags: recursion, array, hash-table, enumeration, sorting
signature: int[] findEvenNumbers(int[] digits)
---
Each element of `digits` is a card showing one decimal digit, and several
cards may show the same digit. A number can be **spelled** with the cards
when you can set aside one card for each of its decimal digits, a card
showing that digit, without using any card twice. For example, `404` can
be spelled only when at least two cards show a `4` and at least one shows
a `0`.

Return all the **even** numbers from `100` to `998` that can be spelled
with the cards of `digits`, in increasing order. List each number once,
however many ways there are to pick its cards. If there is none, return
an empty array.

So every number on the list takes exactly three cards, and a `0` card
never comes first: the cards `0`, `4` and `8` laid down in that order read
`048`, which is 48, not a number from 100 to 998.

@examples

## Constraints

- `3 <= digits.length <= 100`
- `0 <= digits[i] <= 9`
