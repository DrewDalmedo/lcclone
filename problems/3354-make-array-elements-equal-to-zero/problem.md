id: 3354
slug: make-array-elements-equal-to-zero
title: Make Array Elements Equal to Zero
difficulty: easy
tags: array, prefix-sum, simulation
signature: int countValidSelections(int[] nums)
---
Think of `nums` as a row of `n` cells, cell `0` at the left end and cell
`n - 1` at the right, where cell `i` holds `nums[i]`. A cell holding `0`
is empty; a cell holding a positive number is a **wall** that can take
that many more hits.

A ball is set down on an empty cell and pushed to the left or to the right.
From then on it moves in steps, and each step does one of these:

- If the ball is outside the row (left of cell `0` or right of cell
  `n - 1`), it stops for good.
- If its cell is empty, it rolls on to the next cell in the direction it is
  moving.
- If its cell is a wall, the wall takes a hit (its number goes down by
  `1`), the ball turns around, and it rolls on to the next cell in its new
  direction. It does not look at the cell it bounced off again before
  moving on.

A **launch** is a choice of starting cell, which must be empty, together
with a starting direction, so each empty cell gives two different launches.
A launch **clears** the row when, once the ball has stopped, every cell
holds `0`. Each launch is tried on its own copy of the original `nums`.

Return the number of launches that clear the row.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 100`
- At least one element of `nums` is `0`.
