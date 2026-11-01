id: 1496
slug: path-crossing
title: Path Crossing
difficulty: easy
tags: hash-table, string
signature: bool isPathCrossing(string path)
---
A courier walks through a city whose streets form a square grid, and every
street corner has whole-number coordinates `(x, y)`. She sets out from the
corner `(0, 0)`, and `path` records her route one block at a time, each
letter a compass direction: `'N'` adds 1 to `y` and `'S'` takes 1 from
it, while `'E'` adds 1 to `x` and `'W'` takes 1 from it.

So a route of `n` letters takes her to `n + 1` corners, counting both the
corner she starts from and the one she ends at. Return `true` if some
corner appears in that list at least twice, and `false` if all `n + 1`
corners are different.

Coming back to `(0, 0)` counts: she was there before her first step.

@examples

## Constraints

- `1 <= path.length <= 10^4`
- Each `path[i]` is `'N'`, `'S'`, `'E'` or `'W'`.
