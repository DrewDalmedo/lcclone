id: 3842
slug: toggle-light-bulbs
title: Toggle Light Bulbs
difficulty: easy
tags: array, hash-table, sorting, simulation
signature: int[] toggleLightBulbs(int[] bulbs)
---
Along a hallway hang a hundred bulbs labelled `1` through `100`, all dark
at the start. Each bulb has its own push button, and a press changes that
bulb's state: a dark bulb lights up, and a lit bulb goes dark.

`bulbs` records button presses in the order they happen. Each entry is the
label of the bulb whose button was pressed, and the same bulb may be
pressed any number of times.

After the last press, return the labels of the bulbs that are lit, in
increasing order and without repeats. A bulb ends up lit exactly when its
label occurs an odd number of times in `bulbs`. When every bulb ends up
dark, the answer is the empty array.

@examples

## Constraints

- `1 <= bulbs.length <= 100`
- `1 <= bulbs[i] <= 100`
