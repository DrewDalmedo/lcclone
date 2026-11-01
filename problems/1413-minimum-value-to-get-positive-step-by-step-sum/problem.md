id: 1413
slug: minimum-value-to-get-positive-step-by-step-sum
title: Minimum Value to Get Positive Step by Step Sum
difficulty: easy
tags: array, prefix-sum
signature: int minStartValue(int[] nums)
---
You open a bank account by paying in `v` dollars, where `v` is a whole
number and at least `1`. After that, the transactions in `nums` reach the
account one at a time, first to last, and each adds its value to the
balance (so a negative transaction is a withdrawal).

The account must never run dry: right after each transaction, the balance
must be at least `1`. Every one of these balances counts, not only the
last.

Return the smallest `v` that keeps the balance at `1` or more after every
transaction. `v` itself is never less than `1`, even when a start of `0`
or less would already keep every balance at `1` or more.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
