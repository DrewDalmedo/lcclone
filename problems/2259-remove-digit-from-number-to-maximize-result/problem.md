id: 2259
slug: remove-digit-from-number-to-maximize-result
title: Remove Digit From Number to Maximize Result
difficulty: easy
tags: greedy, string, enumeration
signature: string removeDigit(string number, char digit)
---
The string `number` spells a positive integer with the digits `1` to `9`
(it has no zeros). You must erase **exactly one** character of `number`,
and it has to be a copy of `digit`. The characters on either side then
close the gap, keeping their order, so what is left spells a number with
one digit fewer.

When `digit` appears several times, you choose which copy to erase.
Choose so that the number left over is as large as possible, and return
it as a string. (Erasing two different copies may leave the same number;
either way, that number is the answer.)

The numbers can be up to 100 digits long, far more than any integer type
holds.

@examples

## Constraints

- `2 <= number.length <= 100`
- `number` consists of the digits `'1'` to `'9'`.
- `digit` is one of the digits `'1'` to `'9'`.
- `digit` occurs in `number` at least once.
