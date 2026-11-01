id: 1710
slug: maximum-units-on-a-truck
title: Maximum Units on a Truck
difficulty: easy
tags: greedy, array, sorting
signature: int maximumUnits(int[][] boxTypes, int truckSize)
---
A warehouse has boxes of several kinds waiting to be shipped. Each entry
`boxTypes[i] = [count_i, units_i]` describes one kind: there are `count_i`
boxes of it in stock, and every one of those boxes contains `units_i`
units.

A truck can take at most `truckSize` boxes, of any mix of kinds. From each
kind you may load any whole number of boxes, from none up to all `count_i`
of them. Return the most units the truck can carry this way, counting the
units inside every box it takes.

Boxes go on the truck whole, never split. The truck doesn't have to be
full: if there are no more than `truckSize` boxes in the warehouse
altogether, it simply takes all of them. Two entries may describe kinds with
the same `units_i`, or even be identical; they still count as separate
stock.

@examples

## Constraints

- `1 <= boxTypes.length <= 1000`
- `boxTypes[i].length == 2`
- `1 <= count_i, units_i <= 1000`
- `1 <= truckSize <= 10^6`
