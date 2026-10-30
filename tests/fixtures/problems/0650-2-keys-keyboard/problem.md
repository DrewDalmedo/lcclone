id: 650
slug: 2-keys-keyboard
title: 2 Keys Keyboard
difficulty: medium
tags: math, dynamic-programming
signature: int minSteps(int n)
time_limit_ms: 2000
memory_limit_mb: 64
# This file has a byte order mark and CRLF line endings on purpose.
---
A text box starts out holding a single letter `A`. Your keyboard has only two
keys:

1. **Copy All** puts the whole contents of the box on the clipboard.
2. **Paste** appends the clipboard to the end of the box.

Return the smallest number of key presses that leaves exactly `n` letters in
the box.

```text
n = 4:  Copy All, Paste, Paste, Paste  ->  AAAA  (4 presses)
```

Constraint: `1 <= n <= 2000`.
