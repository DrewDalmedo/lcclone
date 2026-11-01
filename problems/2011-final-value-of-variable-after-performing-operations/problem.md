id: 2011
slug: final-value-of-variable-after-performing-operations
title: Final Value of Variable After Performing Operations
difficulty: easy
tags: array, string, simulation
signature: int finalValueAfterOperations(string[] operations)
---
The entries of `operations` make up a short program, one statement per
entry, run in order from the first to the last. The program has a single
integer variable, `X`, which holds `0` before the first statement runs. Every statement is three
characters long and is one of these four:

- `"++X"` and `"X++"` both make `X` one larger;
- `"--X"` and `"X--"` both make `X` one smaller.

Where the two signs stand, before `X` or after it, makes no difference:
there is no other value to track, only the step of 1 up or down.

Return the value of `X` after the last statement. It may be negative.

@examples

## Constraints

- `1 <= operations.length <= 100`
- Each `operations[i]` is `"++X"`, `"X++"`, `"--X"` or `"X--"`.
