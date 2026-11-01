id: 2037
slug: minimum-number-of-moves-to-seat-everyone
title: Minimum Number of Moves to Seat Everyone
difficulty: easy
tags: greedy, array, counting-sort, sorting
signature: int minMovesToSeat(int[] seats, int[] students)
---
A room has `n` seats and `n` students, each at a whole-number position
along one straight line. Seat `i` is at position `seats[i]`, and
student `j` starts at position `students[j]`.

Every student must end up in a seat, and no seat may hold two students.
Students walk one unit at a time: a **step** carries one student a
distance of `1`, to the left or to the right. Return the smallest total
number of steps, added up over all the students, that gets everyone
seated.

Several seats may stand at the same position; each of them is still a
seat of its own. Several students may also start at the same position, and
a student may already be at a seat's position. Students may pass each
other on the way: only the total number of steps counts.

@examples

## Constraints

- `n == seats.length == students.length`
- `1 <= n <= 100`
- `1 <= seats[i], students[j] <= 100`
