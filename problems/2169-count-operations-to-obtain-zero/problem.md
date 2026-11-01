id: 2169
slug: count-operations-to-obtain-zero
title: Count Operations to Obtain Zero
difficulty: easy
tags: math, simulation
signature: int countOperations(int num1, int num2)
---
Two jars hold marbles: the first holds `num1` of them and the second
`num2`. A **move** takes marbles out of one jar, as many as the other jar
holds at that moment:

- if the first jar holds at least as many marbles as the second, they come
  out of the first jar;
- otherwise, they come out of the second jar.

So when the jars hold the same number of marbles, the first jar is the one
that loses them, and it ends up empty.

Moves are made one after another for as long as both jars still hold at
least one marble, and stop as soon as either jar is empty. Return the
number of moves made. If a jar is empty from the start, no move is made
and the answer is `0`.

@examples

## Constraints

- `0 <= num1, num2 <= 10^5`
