id: 2739
slug: total-distance-traveled
title: Total Distance Traveled
difficulty: easy
tags: math, simulation
signature: int distanceTraveled(int mainTank, int additionalTank)
---
A delivery van holds `mainTank` liters of fuel in its main tank and
`additionalTank` liters in a reserve tank. The engine only ever draws from
the main tank, and every liter it burns takes the van 10 kilometers.

A small pump connects the two tanks. It counts the liters burned from the
main tank since the van set off, and each time that count reaches a
multiple of 5 (the 5th liter, the 10th, the 15th, and so on) it moves
exactly 1 liter from the reserve into the main tank, all at once, as long
as the reserve is not yet empty. Fuel never passes between the tanks in
any other way.

- A liter the pump has moved is burned like any other, so it counts toward
  the later multiples of 5.
- If the main tank has just run dry when the count reaches a multiple of
  5, the pump still acts, and the van drives on with the liter it gets.

The van stops once its main tank is empty and no transfer is due; whatever
is still in the reserve then is never used. Return how many kilometers the
van covers in total.

@examples

## Constraints

- `1 <= mainTank <= 100`
- `1 <= additionalTank <= 100`
