id: 2236
slug: root-equals-sum-of-children
title: Root Equals Sum of Children
difficulty: easy
tags: tree, binary-tree
signature: bool checkTree(TreeNode root)
---
A very short invoice is stored as a binary tree of exactly three nodes. The
root holds the total the invoice claims, and its two children, which have no
children themselves, hold the two line items. A negative item is a refund.

The invoice **balances** when the claimed total is exactly the sum of the two
items. Return `true` if `root` balances, and `false` if it doesn't.

An example lists the three values in the order total, left item, right item.

@examples

## Constraints

- The tree has exactly `3` nodes: a root and two childless children.
- `-100 <= Node.val <= 100`
