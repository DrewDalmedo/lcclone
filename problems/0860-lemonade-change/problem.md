id: 860
slug: lemonade-change
title: Lemonade Change
difficulty: easy
tags: greedy, array
signature: bool lemonadeChange(int[] bills)
---
You sell cups of lemonade at `5` each, and your cash box starts out empty.
Customers come up one at a time, in the order of `bills`: customer `i` buys
one cup and pays with a single note worth `bills[i]`, which is `5`, `10` or
`20`, so they are owed `bills[i] - 5` back.

Change can only be made from notes in the box, and the box holds nothing
but the notes customers have paid with, less the ones already handed back.
Customers can't be skipped or served out of order.

Return `true` if you can give every customer exactly what they are owed,
and `false` if you get stuck at some customer.

@examples

## Constraints

- `1 <= bills.length <= 10^5`
- Each `bills[i]` is `5`, `10` or `20`.
