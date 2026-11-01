id: 2383
slug: minimum-hours-of-training-to-win-a-competition
title: Minimum Hours of Training to Win a Competition
difficulty: easy
tags: greedy, array
signature: int minNumberOfHours(int initialEnergy, int initialExperience, int[] energy, int[] experience)
---
A tournament pits you against `n` challengers in a fixed order: challenger
`0` first, then `1`, and so on up to `n - 1`. Challenger `i` brings
`energy[i]` energy and `experience[i]` experience. You enter with
`initialEnergy` energy and `initialExperience` experience.

You beat challenger `i` only if, when you meet, your energy is higher than
`energy[i]` **and** your experience is higher than `experience[i]`. Being
level with the challenger in either one is a loss. Beating challenger `i`
uses up `energy[i]` of your energy and adds `experience[i]` to your
experience, and then you meet the next challenger. You must beat all `n`.

Training is allowed only up front, before challenger `0`, never between
meetings. One hour of it buys one point: `1` more energy or `1` more
experience, your pick hour by hour. You may train as long as you like, or
not at all.

Return the fewest hours of training after which you can beat every
challenger. If you can beat them all without training, return `0`.

@examples

## Constraints

- `n == energy.length == experience.length`
- `1 <= n <= 100`
- `1 <= initialEnergy, initialExperience, energy[i], experience[i] <= 100`
