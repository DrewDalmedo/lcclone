id: 1556
slug: thousand-separator
title: Thousand Separator
difficulty: easy
tags: string
signature: string thousandSeparator(int n)
---
Many countries mark off the thousands in a long number with a full stop:
two and a half million is written `2.500.000`. Return the non-negative
integer `n` written that way, as a string.

Precisely: take the decimal digits of `n`, with no leading zeros (`0` is
the single digit `0`). Counting digits from the right-hand end, put a full
stop between the 3rd and the 4th digit, between the 6th and the 7th, and so
on, for as long as there are digits on both sides. Add nothing else: no other
separator, no spaces, no sign.

So a number of up to three digits gets no dot at all, every group after the
first has exactly three digits, and the first group has one, two or three.
Every digit of `n` stays, so a group can be `005` or `000`.

@examples

## Constraints

- `0 <= n <= 2^31 - 1`
