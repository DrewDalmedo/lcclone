id: 997
slug: find-the-town-judge
title: Find the Town Judge
difficulty: easy
tags: graph, array, hash-table
signature: int findJudge(int n, int[][] trust)
---
Each of a town's `n` residents has a number from `1` to `n`, and every pair
`[a, b]` in the list `trust` says that resident `a` trusts resident `b`.
Only the listed pairs count: trust goes one way (`[a, b]` tells you nothing
about whether `b` trusts `a`), and it doesn't pass along (if 1 trusts 2 and
2 trusts 3, that doesn't make 1 trust 3).

A resident is the **judge** when both of these hold:

- they trust no one: they are never the first number of a pair;
- every one of the other `n - 1` residents trusts them.

Return the number of the judge, or `-1` if nobody meets both conditions.
Two residents can never both qualify, as each would have to trust the
other. When `n` is `1`, that one resident trusts no one and nobody else
exists, so they are the judge.

@examples

## Constraints

- `1 <= n <= 1000`
- `0 <= trust.length <= 10^4`
- `trust[i].length == 2`
- `trust[i] = [a, b]` with `1 <= a, b <= n` and `a != b`
- No pair appears twice in `trust`.
