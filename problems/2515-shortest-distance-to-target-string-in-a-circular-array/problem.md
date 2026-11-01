id: 2515
slug: shortest-distance-to-target-string-in-a-circular-array
title: Shortest Distance to Target String in a Circular Array
difficulty: easy
tags: array, string
signature: int closestTarget(string[] words, string target, int startIndex)
---
A round table has one seat for each entry of `words`. With `n` entries,
the seats are numbered `0` to `n - 1` going clockwise, so that seat
`n - 1` is right next to seat `0` again. A place card stands on every
seat: the card on seat `i` reads `words[i]`. Several cards may read the
same word. You sit down in seat `startIndex`.

A **move** takes you to a neighbouring seat: clockwise from seat `i` to
seat `i + 1`, or counterclockwise to seat `i - 1`, with the numbers taken
round the table (clockwise from seat `n - 1` is seat `0`, and
counterclockwise from seat `0` is seat `n - 1`). You may change direction
whenever you like.

Return the smallest number of moves after which you sit in a seat whose
card reads exactly `target`: the same letters in the same order, nothing
more and nothing less. That is `0` when the card on seat `startIndex`
already reads `target`. When no card reads `target`, return `-1`.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 100`
- `1 <= target.length <= 100`
- `words[i]` and `target` consist of lowercase English letters.
- `0 <= startIndex < words.length`
