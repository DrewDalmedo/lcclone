id: 3168
slug: minimum-number-of-chairs-in-a-waiting-room
title: Minimum Number of Chairs in a Waiting Room
difficulty: easy
tags: string, simulation
signature: int minimumChairs(string s)
---
The string `s` logs what happens at the door of a waiting room, one event
after another:

- `'E'`: a person walks in and needs a chair that nobody is sitting on;
- `'L'`: one of the people inside walks out, and the chair they used is
  free again for whoever walks in later.

Nobody is inside before the first event, and people may still be inside
after the last one. Return the smallest number of chairs the room can have
without anyone ever walking in to find them all taken. Because chairs are
reused, this is the largest number of people inside at the same time,
which can be less than the number of `'E'` events.

@examples

## Constraints

- `1 <= s.length <= 50`
- `s` consists of the letters `'E'` and `'L'` only.
- Nobody walks out of an empty room: every prefix of `s` has at least as
  many `'E'`s as `'L'`s.
