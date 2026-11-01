id: 4024
slug: nearest-available-drone
title: Nearest Available Drone
difficulty: easy
tags: array, enumeration
signature: int nearestDrone(int[][] drones, int[] target)
---
A parcel is waiting at the point `target = [tx, ty]` of a city whose
streets form a grid, and a fleet of drones is parked around it. Each
`drones[i] = [x, y, r]` says that drone `i` is parked at `(x, y)` and that
its battery lasts for at most `r` units of flight.

The drones fly along the streets, so getting from `(x, y)` to the parcel
takes a flight of `|x - tx| + |y - ty|` units. Drone `i` is **available**
when that flight fits its battery: a flight of exactly `r` units can still
be made, one of `r + 1` units cannot. A drone that is not available is
left out of the choice altogether, even if it is parked very close.

Send the available drone with the shortest flight to the parcel, and
return its index, counting from `0`. When two or more available drones
tie for the shortest flight, send the one listed first in `drones`. Spare
battery is worth nothing here: only the length of the flight matters.
Return `-1` if no drone is available.

@examples

## Constraints

- `1 <= drones.length <= 100`
- `drones[i].length == 3`
- `target.length == 2`
- `-25 <= drones[i][0], drones[i][1], target[0], target[1] <= 25`
- `1 <= drones[i][2] <= 100`
