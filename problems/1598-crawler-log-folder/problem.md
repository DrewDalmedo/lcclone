id: 1598
slug: crawler-log-folder
title: Crawler Log Folder
difficulty: easy
tags: stack, array, string
signature: int minOperations(string[] logs)
---
A program walks around a tree of folders. It starts in the top folder of the
tree, and `logs` records the folder changes it then made, in order. Each
entry has one of three forms:

- `"../"`: go up to the folder that holds the current one. The top folder
  has nothing above it, so there this changes nothing.
- `"./"`: stay in the current folder.
- a folder name followed by `"/"`, such as `"src/"` or `"7z/"`: go down
  into the folder of that name inside the current one (the log only ever
  names folders that are there). Names are made of lowercase letters and
  digits, and the same name may come up more than once.

After the last entry, the program wants to get back to the top folder with
more changes of these same three kinds. Return the fewest changes that
takes (`0` if it is already there).

@examples

## Constraints

- `1 <= logs.length <= 1000`
- `2 <= logs[i].length <= 10`
- `logs[i]` consists of lowercase English letters, digits, `'.'` and `'/'`.
- Each `logs[i]` has one of the three forms above; a folder name is never
  empty.
