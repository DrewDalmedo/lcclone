id: 1022
slug: sum-of-root-to-leaf-binary-numbers
title: Sum of Root To Leaf Binary Numbers
difficulty: easy
tags: tree, depth-first-search, binary-tree
signature: int sumRootToLeaf(TreeNode root)
---
Think of the binary tree `root` as a bank of lockers. Every childless node, or
**leaf**, is one locker, and the nodes on the way down to it spell out that
locker's number. Every node holds a single bit. Read the bits from the root to the leaf as a base-2
numeral: the root's bit is the most significant, the leaf's the least, and
zeros at the front change nothing. A leaf reached through the bits `0`, `1`,
`0`, `0` is locker `4`.

Add up all the locker numbers and return the total. Each leaf counts once,
even when another leaf has the same number. Every input keeps that total
within a signed 32-bit integer, although a route may be far longer than 32
bits when it starts with zeros.

Example inputs list the bits from the top of the tree downward, one depth at a
time, with `null` marking an absent child.

@examples

## Constraints

- The tree has between `1` and `1000` nodes.
- `Node.val` is `0` or `1`.
- The answer is at most `2^31 - 1`.
