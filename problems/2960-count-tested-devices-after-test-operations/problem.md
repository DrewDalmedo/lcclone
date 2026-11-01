id: 2960
slug: count-tested-devices-after-test-operations
title: Count Tested Devices After Test Operations
difficulty: easy
tags: array, counting, simulation
signature: int countTestedDevices(int[] batteryPercentages)
---
A row of gadgets is lined up on a workbench, and `batteryPercentages[i]`
is the charge of gadget `i`, counting from `0` at the left. A technician
walks along the row from left to right and deals with each gadget once,
looking at the charge it has *at that moment*:

- if the charge is above `0`, she tests the gadget. Running a test drains
  `1` from the charge of every gadget to its right, the ones she has not
  reached yet; a gadget whose charge is already `0` stays at `0`. Gadgets
  to its left, and the tested gadget itself, keep their charge;
- if the charge is `0`, she skips the gadget, and no charge changes.

Return how many gadgets she tests.

@examples

## Constraints

- `1 <= batteryPercentages.length <= 100`
- `0 <= batteryPercentages[i] <= 100`
