id: 13
slug: roman-to-integer
title: Roman to Integer
difficulty: easy
tags: hash-table, math, string
signature: int romanToInt(string s)
---
`s` spells a whole number from 1 to 3999 in Roman numerals. Return that
number.

Seven letters are used, each with a fixed value:

- `I` = 1, `V` = 5, `X` = 10, `L` = 50
- `C` = 100, `D` = 500, `M` = 1000

The numeral is written in the standard modern way: one decimal digit at a
time, from the thousands down, with the pieces placed side by side. The
thousands digit is that many `M`s. The hundreds, tens and ones each have
three letters, worth one, five and ten units of that place: `C`, `D`, `M`
for the hundreds, `X`, `L`, `C` for the tens and `I`, `V`, `X` for the ones.
Calling them *one*, *five* and *ten*, a digit `d` of one of these places is
written as:

- `0`: nothing;
- `1` to `3`: *one*, repeated `d` times;
- `4`: *one*, then *five*;
- `5`: *five*;
- `6` to `8`: *five*, then *one* repeated `d - 5` times;
- `9`: *one*, then *ten*.

So the value of a numeral is the sum of its letters, except that a letter
written just before a letter of greater value is subtracted instead of
added. That happens only in the pairs `IV`, `IX`, `XL`, `XC`, `CD` and `CM`.

@examples

## Constraints

- `1 <= s.length <= 15`
- `s` consists of the letters `I`, `V`, `X`, `L`, `C`, `D` and `M`.
- `s` is the standard numeral, written as described above, of a number from
  `1` to `3999`.
