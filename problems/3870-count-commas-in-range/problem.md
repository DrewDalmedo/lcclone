id: 3870
slug: count-commas-in-range
title: Count Commas in Range
difficulty: easy
tags: math
signature: int countCommas(int n)
---
A page counter prints the whole numbers `1, 2, 3, ...` up to and including
`n`, each in decimal with no sign and no leading zeros. To make long
numbers easier to read, it puts in commas the usual English way: the
digits are split into blocks of three, starting from the units digit and
working left, and a comma goes between every two neighbouring blocks. The
block at the front may be shorter than three digits and never has a comma
before it, so `640` gets none, `4,096` one and `12,345,678` two.

Return how many commas the counter prints in total, over all the numbers
from `1` to `n`.

@examples

## Constraints

- `1 <= n <= 10^5`
