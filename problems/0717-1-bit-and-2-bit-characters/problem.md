id: 717
slug: 1-bit-and-2-bit-characters
title: 1-bit and 2-bit Characters
difficulty: easy
tags: array
signature: bool isOneBitCharacter(int[] bits)
---
A tiny machine reads its program, the list `bits` of `0`s and `1`s, from
left to right, as a series of instructions of two sizes:

- a **1-bit** instruction is a single `0`;
- a **2-bit** instruction is a `1` together with the bit right after it,
  so it is either `10` or `11`.

Each instruction starts where the one before it ended, and its first bit
leaves the machine no choice: a `0` there is a whole instruction, and a `1`
takes the next bit with it, whatever that bit is. The program always ends
in a `0`, so every `1` has a bit after it, and the instructions use up the
program exactly.

Return `true` if the last instruction is a 1-bit one, the final `0` read on
its own, and `false` if that `0` is the second half of a `10`.

@examples

## Constraints

- `1 <= bits.length <= 1000`
- `bits[i]` is `0` or `1`.
- `bits[bits.length - 1] == 0`
