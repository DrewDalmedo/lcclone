id: 1450
slug: number-of-students-doing-homework-at-a-given-time
title: Number of Students Doing Homework at a Given Time
difficulty: easy
tags: array
signature: int busyStudent(int[] startTime, int[] endTime, int queryTime)
---
A teacher kept a log of when each student of a class sat over homework.
Student `i` began at minute `startTime[i]` and put it away at minute
`endTime[i]`, and was working during each minute from the first to the
last, **both included**. A student whose two minutes are the same worked
during that one minute only.

Return how many students were working during minute `queryTime`: the count
of indices `i` for which `startTime[i] <= queryTime <= endTime[i]`. The
students' times may overlap in any way, and they are listed in no
particular order.

@examples

## Constraints

- `1 <= startTime.length <= 100`
- `endTime.length == startTime.length`
- `1 <= startTime[i] <= endTime[i] <= 1000`
- `1 <= queryTime <= 1000`
