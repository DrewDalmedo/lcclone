id: 1436
slug: destination-city
title: Destination City
difficulty: easy
tags: array, hash-table, string
signature: string destCity(string[][] paths)
---
A road trip went through a series of cities, driving straight from each
city to the next and never coming to the same city twice. Each leg of the
drive was noted down as a pair `[from, to]`, and `paths` holds all of
these notes, shuffled into no particular order.

Return the city where the trip **finished**: the one city that some leg
drives to but that no leg sets out from. (The city where the trip began
is also named only once, but as a `from`, so it is not the answer.)

City names are compared exactly, character by character. Upper and lower
case differ, so `"Rome"` and `"rome"` are different cities, as are
`"Rome"` and `"Romeo"`. A space is an ordinary character wherever it
appears, so `"Le Mans"`, `"Le  Mans"` (two spaces) and `"Le Mans "` are
three different names too. Return the name exactly as `paths` spells it.

@examples

## Constraints

- `1 <= paths.length <= 100`
- `paths[i].length == 2`
- `1 <= paths[i][0].length, paths[i][1].length <= 10`
- `paths[i][0] != paths[i][1]`
- City names consist of English letters (upper and lower case) and spaces.
- The legs join up into a single trip, as described: each city appears as
  a `from` at most once and as a `to` at most once, and no leg leads back
  to a city visited earlier.
