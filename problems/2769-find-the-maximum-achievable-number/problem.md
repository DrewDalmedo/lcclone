id: 2769
slug: find-the-maximum-achievable-number
title: Find the Maximum Achievable Number
difficulty: easy
tags: math
signature: int theMaximumAchievableX(int num, int t)
---
Two markers sit on a number line: one on the point `num`, the other on a
whole number `x` that you choose. The game goes in rounds. In every round
**both** markers move exactly one unit, and for each marker separately you
decide whether it steps down by `1` or up by `1`.

You win if, after at most `t` rounds, the two markers stand on the same
point; the game may end as soon as they meet. The marker that started on
`num` moves as well, so they don't have to meet at `num`.

Return the largest starting point `x` from which you can win.

@examples

## Constraints

- `1 <= num <= 50`
- `1 <= t <= 50`
