id: 4061
slug: minimum-queen-moves-to-reach-target
title: Minimum Queen Moves to Reach Target
difficulty: easy
tags: array, math
signature: int minQueenMoves(int[] source, int[] target)
---
An empty chessboard has 8 rows and 8 columns, each numbered from 1 to 8,
and a square is given as the pair `[row, column]`. The only piece on the
board is a queen, standing on the square `source`.

A move carries the queen in a straight line in one of eight directions:
along its row or its column, or along either of the two diagonals through
its square, each of them either way. It may cover any number of squares in
that direction, at least one, but it has to stop on the board. Two squares
share a diagonal exactly when they are as many rows apart as they are
columns apart. Nothing stands in the queen's way.

Return the smallest number of moves after which the queen stands on the
square `target`. When `target` is the square the queen already stands on,
the answer is `0`.

@examples

## Constraints

- `source.length == target.length == 2`
- `1 <= source[i], target[i] <= 8`
