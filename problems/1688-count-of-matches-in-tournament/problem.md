id: 1688
slug: count-of-matches-in-tournament
title: Count of Matches in Tournament
difficulty: easy
tags: math, simulation
signature: int numberOfMatches(int n)
---
`n` teams enter a knockout tournament played in rounds. At the start of a
round, the teams still in the tournament are paired up, and each pair
plays one match. The loser of a match is out; the winner stays in.

When a round starts with an odd number of teams, one team is left without
an opponent. That team gets a **bye**: it plays no match in this round but
still goes through to the next one. Which team gets the bye does not
change the answer.

Rounds are played until a single team is left, the champion. Return the
total number of matches played in the whole tournament. A tournament
entered by only one team has no rounds at all.

@examples

## Constraints

- `1 <= n <= 200`
