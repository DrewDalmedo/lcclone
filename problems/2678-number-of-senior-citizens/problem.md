id: 2678
slug: number-of-senior-citizens
title: Number of Senior Citizens
difficulty: easy
tags: array, string
signature: int countSeniors(string[] details)
---
A coach company gives a discount to riders over sixty. Its booking system
keeps each rider on file as a record of exactly 15 characters, numbered
from `0`, with every field at a fixed place:

- characters `0` to `9`: the rider's phone number, ten digits;
- character `10`: a gender code, one of `M`, `F` and `O`;
- characters `11` and `12`: the rider's age in years, as a two-digit
  number, so an age below 10 starts with a `0` (`"07"` is 7 and `"00"` is
  0);
- characters `13` and `14`: the seat number, two digits.

`details` holds one such record per rider. Return how many riders are
**older than 60**, that is, aged 61 or more; a rider aged exactly 60 does
not count. Only the age field matters. The phone number, the gender code
and the seat play no part, even where their digits look like an age.

@examples

## Constraints

- `1 <= details.length <= 100`
- `details[i].length == 15`
- Character `10` of every record is `M`, `F` or `O`, and every other
  character is a digit from `0` to `9`.
- No two records share a phone number, and no two share a seat number.
