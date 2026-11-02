id: 3798
slug: largest-even-number
title: Largest Even Number
difficulty: easy
tags: string
signature: string largestEven(string s)
---
The string `s` uses no digits other than `1` and `2`; read from left to
right, it is a positive integer in decimal notation. Choose which of its
digits to keep. Any choice is allowed, keeping all of them included, as
long as at least one digit is kept. Reading the kept digits from left to
right, in the order they have in `s`, gives a new integer.

An integer written with these digits is **even** exactly when its last
digit is `2`. Return the greatest even integer that a choice can give, as
its string of digits. Choices are compared by the value of the integer, not
alphabetically as strings: `2112` is greater than `222`, although `"222"`
would sort after `"2112"`.

If `s` contains no `2`, no choice gives an even integer, and the answer is
the empty string `""`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of the characters `'1'` and `'2'` only.
