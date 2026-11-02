id: 3894
slug: traffic-signal-color
title: Traffic Signal Color
difficulty: easy
tags: math, string, simulation
signature: string trafficSignal(int timer)
---
The controller of a crossing light keeps a countdown, `timer`, and shows a
word on its status panel that depends only on the number the countdown
holds. Return that word.

Only a few readings stand for a color:

- a reading of exactly `0` shows `"Green"`;
- a reading of exactly `30` shows `"Orange"`;
- any reading from `31` up to `90`, `90` included, shows `"Red"`.

Every other reading, that is `1` to `29` and anything above `90`, is not a
state the light can be in, and the panel shows `"Invalid"`. Note that `30`
itself is Orange and not Red.

The panel's four words are spelled and capitalized exactly as quoted
above, with no spaces around them.

@examples

## Constraints

- `0 <= timer <= 1000`
