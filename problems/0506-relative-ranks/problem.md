id: 506
slug: relative-ranks
title: Relative Ranks
difficulty: easy
tags: array, sorting, heap-priority-queue
signature: string[] findRelativeRanks(int[] score)
---
After a contest, `score[i]` is the number of points that entrant `i` got,
and no two entrants got the same number. Rank the entrants from the most
points down: the **place** of an entrant is `1` plus the number of entrants
with more points than theirs. So the top total is in place `1`, the next
one in place `2`, and the bottom total in place `n`, where `n` is the
length of `score`.

Every entrant gets a label for their place. Places `1`, `2` and `3` get
the medal labels `"Gold Medal"`, `"Silver Medal"` and `"Bronze Medal"`, in
that order. Any place `p` of `4` or more gets the decimal digits of `p` and
nothing else: place `4` is labelled `"4"`, and place `27` is `"27"`.

Return the `n` labels with entrant `i`'s label at index `i`, so that they
line up with `score`; they are not sorted by place. Case and spacing count:
`"gold medal"` or `"Gold  Medal"` would be wrong.

@examples

## Constraints

- `n == score.length`
- `1 <= n <= 10^4`
- `0 <= score[i] <= 10^6`
- All the values in `score` are different.
