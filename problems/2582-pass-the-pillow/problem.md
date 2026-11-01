id: 2582
slug: pass-the-pillow
title: Pass the Pillow
difficulty: easy
tags: math, simulation
signature: int passThePillow(int n, int time)
---
`n` children sit in a row of chairs numbered `1` to `n`, and the child in
chair `1` starts with a pillow at second `0`. The pillow moves one chair per
second, always to a child sitting right next to the one who had it.

The pillow sets off towards chair `n` and keeps going the same way until
there is no chair beyond it. There it turns back at once: the child in chair
`n` passes it to chair `n - 1` the very next second, and when it later gets
back to chair `1`, it heads for chair `n` again, and so on for as long as
the game lasts. Turning costs no extra second. With `5` chairs, for
instance, the pillow is in chairs `1, 2, 3, 4, 5, 4, 3, 2, 1, 2` at seconds
`0` to `9`, and the pattern repeats from there.

Return the number of the chair the pillow is in after `time` seconds.

@examples

## Constraints

- `2 <= n <= 1000`
- `1 <= time <= 1000`
