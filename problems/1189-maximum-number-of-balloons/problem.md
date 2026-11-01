id: 1189
slug: maximum-number-of-balloons
title: Maximum Number of Balloons
difficulty: easy
tags: hash-table, string, counting
signature: int maxNumberOfBalloons(string text)
---
Think of `text` as a heap of letter beads, one bead per character. You
want to thread strands that spell `balloon`, and each strand takes exactly
seven beads: one `b`, one `a`, two `l`, two `o` and one `n`. You can
thread the beads in any order, so where a bead sat in `text` makes no
difference, but a bead on one strand can't also go on another.

Return the largest number of complete `balloon` strands the heap allows.
Beads of other letters are never needed, and a strand that lacks even one
bead doesn't count, so the answer is `0` when the heap can't finish a
single one.

@examples

## Constraints

- `1 <= text.length <= 10^4`
- `text` consists of lowercase English letters only.
