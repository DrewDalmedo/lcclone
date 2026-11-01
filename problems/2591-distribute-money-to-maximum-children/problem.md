id: 2591
slug: distribute-money-to-maximum-children
title: Distribute Money to Maximum Children
difficulty: easy
tags: greedy, math
signature: int distMoney(int money, int children)
---
You have `money` one-dollar bills to share among `children` children,
using one envelope per child. Fill the envelopes so that:

- every bill goes into an envelope, with none left over;
- every envelope holds at least one bill;
- no envelope holds exactly `4` bills, because four is unlucky.

Any other number of bills in an envelope is fine, more than `8` included.
Eight, on the other hand, is lucky: among all the ways of filling the
envelopes that keep these rules, take one with as many envelopes of exactly
`8` bills as possible, and return how many such envelopes it has.

If no way of filling the envelopes keeps all three rules, return `-1`.

@examples

## Constraints

- `1 <= money <= 200`
- `2 <= children <= 30`
