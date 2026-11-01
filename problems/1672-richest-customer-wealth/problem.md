id: 1672
slug: richest-customer-wealth
title: Richest Customer Wealth
difficulty: easy
tags: array, matrix
signature: int maximumWealth(int[][] accounts)
---
A survey asked `m` customers how much money they hold at each of `n`
banks, the same banks for everyone. Their answers form the grid
`accounts`, with one row per customer and one column per bank: customer
`i` reported holding `accounts[i][j]` at bank `j`. Every customer
answered for every bank, so all rows have the same length.

Adding up the numbers in a customer's row gives their **wealth**. Return
the largest wealth found in the survey. Several customers may share it;
only the amount is asked for, not who has it.

@examples

## Constraints

- `m == accounts.length`
- `n == accounts[i].length`
- `1 <= m, n <= 50`
- `1 <= accounts[i][j] <= 100`
