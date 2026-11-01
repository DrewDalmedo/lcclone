id: 2710
slug: remove-trailing-zeros-from-a-string
title: Remove Trailing Zeros From a String
difficulty: easy
tags: string
signature: string removeTrailingZeros(string num)
---
The string `num` spells out a positive whole number in base ten. Strip the
zeros off its right-hand end: as long as the last character is `0`,
delete it. Return what is left.

Only the zeros after the last non-zero digit go. A `0` with a non-zero
digit anywhere to its right stays where it is, and if `num` doesn't end
in `0`, it comes back unchanged. Since `num` never starts with `0`, the
result always keeps at least its first digit.

`num` can be up to 1000 digits long, far more than a `long long` holds,
so work on the text itself.

@examples

## Constraints

- `1 <= num.length <= 1000`
- `num` consists of the digits `0` to `9` only.
- `num` has no leading zeros.
