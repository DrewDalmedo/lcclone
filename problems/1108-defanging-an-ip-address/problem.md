id: 1108
slug: defanging-an-ip-address
title: Defanging an IP Address
difficulty: easy
tags: string
signature: string defangIPaddr(string address)
---
`address` holds an IPv4 address in its usual written form: four fields,
each a whole number from `0` to `255` written in decimal without leading
zeros, joined by single dots, as in `10.0.4.192`.

Security reports often quote addresses in a form that no program will
take for a real address or turn into a link: each dot is put between
square brackets. This is called **defanging** the address.

Return a new string that is `address` with each of its three dots written
as the three characters `[.]`. The digits stay as they are and in the
same order, and nothing else (no spaces) is added.

@examples

## Constraints

- `address` is a valid IPv4 address, as described above.
