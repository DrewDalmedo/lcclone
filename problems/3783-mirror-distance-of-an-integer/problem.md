id: 3783
slug: mirror-distance-of-an-integer
title: Mirror Distance of an Integer
difficulty: easy
tags: math
signature: int mirrorDistance(int n)
---
Hold the decimal digits of `n` up to a mirror: the **mirror image** of `n`
is the number spelled by those digits in the opposite order. When `n` ends
in zeros, the reversed digits begin with them, and they are not part of
the value. So the mirror image of `40100` is `104` (the zero between the
`1` and the `4` stays), and a one-digit number is its own mirror image.

Return the distance between `n` and its mirror image on the number line:
the larger of the two minus the smaller. That distance is `0` exactly when
the digits of `n` read the same in both directions.

@examples

## Constraints

- `1 <= n <= 10^9`
