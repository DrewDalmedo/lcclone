id: 1502
slug: can-make-arithmetic-progression-from-sequence
title: Can Make Arithmetic Progression From Sequence
difficulty: easy
tags: array, sorting
signature: bool canMakeArithmeticProgression(int[] arr)
---
You may reorder the integers of `arr` however you like, but you must keep
every one of them: a value that occurs twice in `arr` occurs twice in the
new order too. Can you choose an order in which each number, after the
first, is the one before it plus the same fixed integer `d`?

Return `true` if some order does this, and `false` if none does. The step
`d` may be positive, negative or zero; `d = 0` works exactly when every
element of `arr` has the same value.

@examples

## Constraints

- `2 <= arr.length <= 1000`
- `-10^6 <= arr[i] <= 10^6`
