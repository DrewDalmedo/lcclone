id: 2119
slug: a-number-after-a-double-reversal
title: A Number After a Double Reversal
difficulty: easy
tags: math
signature: bool isSameAfterReversals(int num)
---
To **flip** a non-negative integer, read its decimal digits from the last
one to the first and take the number they spell. Zeros that come out in
front are worth nothing and simply disappear: flipping `9040` spells
`0409`, so the result is `409`. Flipping `0` gives `0`.

Flip `num`, and then flip the number that came out. Return `true` if the
second flip gives back `num` itself, and `false` if it gives some other
number.

@examples

## Constraints

- `0 <= num <= 10^6`
