id: 3921
slug: score-validator
title: Score Validator
difficulty: easy
tags:
signature: int[] scoreValidator(string[] events)
---
A scorer keeps the tally of one innings of a cricket match as a list of
short codes, `events`, written in the order things happened. The tally
starts at `0` runs and `0` wickets. These are the only codes, and the only
rules that apply:

- `"0"`, `"1"`, `"2"`, `"3"`, `"4"` or `"6"`: that many runs are added
  (`"0"` adds none, but is still a code like the others);
- `"W"`: a wicket falls, so the wickets go up by one; no runs are added;
- `"WD"` or `"NB"`: one run is added; the wickets stay as they are.

Codes are compared whole: `"WD"` begins with a W, but it is not a wicket.

Go through the codes from the first one on. The innings ends as soon as the
**tenth** wicket falls: the `"W"` that makes it ten still counts, but every
code after it is ignored, runs included. If fewer than ten wickets fall,
every code counts.

Return the tally at the end of the innings as the array `[runs, wickets]`.

@examples

## Constraints

- `1 <= events.length <= 1000`
- Each `events[i]` is one of `"0"`, `"1"`, `"2"`, `"3"`, `"4"`, `"6"`,
  `"W"`, `"WD"` and `"NB"`.
