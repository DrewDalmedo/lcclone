id: 1518
slug: water-bottles
title: Water Bottles
difficulty: easy
tags: math, simulation
signature: int numWaterBottles(int numBottles, int numExchange)
---
You begin with `numBottles` full bottles of water and no empty ones. From
then on you may do either of these, as often as you like and in any order:

- **drink** a full bottle, which counts toward your total and leaves you
  with an empty bottle;
- **recycle** exactly `numExchange` empty bottles at a machine that hands
  back one full bottle for them.

The machine never accepts fewer than `numExchange` empties at once, and
nothing can be borrowed or paid back later. Empties that are left over
after recycling stay with you and can go in later, together with bottles
you empty afterwards.

Return the largest number of bottles you can drink in total.

@examples

## Constraints

- `1 <= numBottles <= 100`
- `2 <= numExchange <= 100`
