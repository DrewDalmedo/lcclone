id: 2481
slug: minimum-cuts-to-divide-a-circle
title: Minimum Cuts to Divide a Circle
difficulty: easy
tags: geometry, math
signature: int numberOfCuts(int n)
---
A round pizza is to be shared out as `n` slices that are exactly alike:
the same size and the same shape. The cutter makes only straight cuts
through the centre of the pizza, of two kinds:

- a **long cut** runs all the way across, from a point on the rim through
  the centre to the opposite point on the rim;
- a **short cut** runs from the centre straight out to one point on the
  rim.

No other cut is allowed: every cut passes through the centre and reaches
the rim (at both ends, for a long cut). A short cut on its own only slits
the pizza, without dividing it.

Return the smallest number of cuts that leaves the pizza in exactly `n`
slices, all alike. For `n = 1` the uncut pizza is already the one slice,
so the answer is `0`.

@examples

## Constraints

- `1 <= n <= 100`
