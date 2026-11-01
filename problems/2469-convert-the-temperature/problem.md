id: 2469
slug: convert-the-temperature
title: Convert the Temperature
difficulty: easy
tags: math
signature: double[] convertTemperature(double celsius)
---
`celsius` is a temperature in degrees Celsius. Return the same temperature
on two other scales, as an array of two numbers in this order:

1. in kelvin: `kelvin = celsius + 273.15`;
2. in degrees Fahrenheit: `fahrenheit = celsius * 1.80 + 32.00`.

The order matters: kelvin comes first. Each number within `10^-5` of its
exact value is accepted.

@examples

## Constraints

- `0 <= celsius <= 1000`
- `celsius` has at most two digits after the decimal point.
