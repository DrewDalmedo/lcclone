id: 929
slug: unique-email-addresses
title: Unique Email Addresses
difficulty: easy
tags: array, hash-table, string
signature: int numUniqueEmails(string[] emails)
---
Each entry of `emails` holds exactly one `@`, which splits it into a
**user part** (before the `@`) and a **domain** (after it).

A mail server ignores some characters of the user part. It files each
message in a **mailbox**, which it reads off the address in three steps:

1. If the user part contains a `+`, it is cut at the first `+`: that `+`
   and whatever follows it in the user part are dropped.
2. Every `.` still in the user part is deleted.
3. The domain is kept exactly as written, with all its dots, and with any
   `+` it may contain.

The mailbox is the user part left after steps 1 and 2, then `@`, then the
domain. Two mailboxes are the same only when they are equal character
for character. The user part may become empty (as in `"..@x.com"`); the
mailbox is then `@x.com`, which counts like any other.

Every entry of the list is sent one message (an entry listed twice is
sent two). Return how many distinct mailboxes end up holding at least one
message.

@examples

## Constraints

- `1 <= emails.length <= 100`
- `1 <= emails[i].length <= 100`
- `emails[i]` consists of lowercase English letters, `.`, `+` and `@`.
- Each `emails[i]` contains exactly one `@`, and both its user part and
  its domain are non-empty.
- No user part starts with `+`.
- Every domain ends with `.com`, and has at least one character before
  that `.com`.
