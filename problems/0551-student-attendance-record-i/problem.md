id: 551
slug: student-attendance-record-i
title: Student Attendance Record I
difficulty: easy
tags: string
signature: bool checkRecord(string s)
---
`s` tells, day by day and in order, how a student turned up for school.
Each character stands for one school day: `'P'` for a day the student was
there on time, `'L'` for a day they arrived late, and `'A'` for a day they
missed.

Call `s` **acceptable** when it keeps both of these rules:

1. At most one day of the whole of `s` is an absence. Absences count
   wherever they are, next to each other or far apart.
2. There is no late streak of three: nowhere in `s` do three `'L'`
   characters come one right after another. A `'P'` or an `'A'` day
   between late days ends the streak.

Return `true` if `s` is acceptable and `false` if it breaks a rule. Late
days that are not three in a row don't count against the student, no
matter how many there are, and days on time never do.

@examples

## Constraints

- `1 <= s.length <= 1000`
- Each `s[i]` is `'A'`, `'L'` or `'P'`.
