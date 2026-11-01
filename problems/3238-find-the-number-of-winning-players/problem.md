id: 3238
slug: find-the-number-of-winning-players
title: Find the Number of Winning Players
difficulty: easy
tags: array, hash-table, counting
signature: int winningPlayerCount(int n, int[][] pick)
---
`n` players, numbered `0` to `n - 1`, draw balls from a bag. The balls come
in colours numbered `0` to `10`. Each entry `[x, y]` of `pick` records one
draw: player `x` drew a ball of colour `y`. The entries may come in any
order, and two equal entries are two separate balls.

The players carry handicaps: to win, player `i` must hold **more than `i`
balls of one single colour**. A single ball of any colour is therefore
enough for player `0`, while player `3`, say, needs four that match, and
player `9` needs ten. Balls of different colours never add up: two red
balls and two blue ones make two of a colour, not four.

Return how many players win: anywhere from none of them to all `n`. A
player who has enough balls in several colours still counts once, and a
player who drew nothing doesn't win.

@examples

## Constraints

- `2 <= n <= 10`
- `1 <= pick.length <= 100`
- `pick[i].length == 2`
- `pick[i] = [x, y]` with `0 <= x <= n - 1` and `0 <= y <= 10`
