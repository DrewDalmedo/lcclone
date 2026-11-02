id: 4070
slug: minimum-rotations-to-dial-a-number-i
title: Minimum Rotations to Dial a Number I
difficulty: easy
tags:
signature: int minRotations(string s)
---
A safe is opened with a round knob. The ten digits `0` to `9` are printed
around its edge in order, and the circle goes on from `9` straight back to
`0`: every digit has exactly two neighbours, and `9` and `0` are
neighbours of each other. A fixed arrow above the knob shows one digit,
and at the start it shows `0`.

Each **rotation** turns the knob a single step, clockwise or
anticlockwise as you like, and you choose again for every rotation. After
it, the arrow shows the digit one place further round the circle, up or
down from where it was.

The code `s` is entered one digit at a time, from left to right: turn the
knob until the arrow shows the next digit of `s`, and that digit counts as
entered. The knob then stays where it is, and the next digit is reached
from there. A digit equal to the one before it needs no rotation at all.

Return the smallest total number of rotations that enters the whole code.

@examples

## Constraints

- `s.length == 10`
- `s` consists of digits from `'0'` to `'9'`.
