id: 1700
slug: number-of-students-unable-to-eat-lunch
title: Number of Students Unable to Eat Lunch
difficulty: easy
tags: stack, queue, array, simulation
signature: int countStudents(int[] students, int[] sandwiches)
---
At a sandwich counter, `n` students wait in a single line and `n`
sandwiches lie in a pile. Each sandwich is one of two kinds, numbered `0`
and `1`, and each student will eat only one of the two kinds.

`students[j]` is the kind wanted by the student at position `j` of the
line, where position `0` is the front. `sandwiches[i]` is the kind of the
sandwich at depth `i` in the pile, where depth `0` is the top. Only the top
sandwich can be taken.

The counter repeats one step. The student at the front of the line looks
at the top sandwich. If it is the kind they want, they take it and step
out of the line, and the sandwich under it becomes the top. If not, they
pass, and walk to the back of the line with the pile untouched.

This goes on until the line is empty, or until no student left in the line
wants the kind on top of the pile, since from then on they would only go
round and round. Return the number of students still in the line when it
stops.

@examples

## Constraints

- `n == students.length == sandwiches.length`
- `1 <= n <= 100`
- Each `students[j]` and each `sandwiches[i]` is `0` or `1`.
