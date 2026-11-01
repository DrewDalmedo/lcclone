id: 1629
slug: slowest-key
title: Slowest Key
difficulty: easy
tags: array, string
signature: char slowestKey(int[] releaseTimes, string keysPressed)
---
A typing session is logged in two sequences of the same length `n`, both
indexed from `0`. Keystroke `i` was made on the key `keysPressed[i]`, and
`releaseTimes[i]` is the moment that key was let go. The clock reads `0`
when keystroke `0` begins, and the keystrokes follow one another with no
gap and no overlap: keystroke `i`, for `i >= 1`, begins at
`releaseTimes[i - 1]`. The **hold time** of a keystroke, from its start to
its release, is therefore `releaseTimes[0]` for keystroke `0`, and
`releaseTimes[i] - releaseTimes[i - 1]` for keystroke `i >= 1`.

Find the greatest hold time among the `n` keystrokes and answer with the
key that was held down for it. Hold times are never pooled per key: a key
struck several times has a separate hold time for each strike, and only
single strikes compete. When several keystrokes tie for the greatest hold
time, answer with the key among them that comes last in the alphabet, no
matter where in the log they appear.

@examples

## Constraints

- `releaseTimes.length == keysPressed.length == n`
- `2 <= n <= 1000`
- `1 <= releaseTimes[i] <= 10^9`
- `releaseTimes[i] < releaseTimes[i + 1]`
- `keysPressed` consists of lowercase English letters only.
