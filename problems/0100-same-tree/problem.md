id: 100
slug: same-tree
title: Same Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: bool isSameTree(TreeNode p, TreeNode q)
---
A backup is only useful if nothing was lost or altered on the way. The binary
tree `p` is the original and `q` is its backup. Return `true` if `q` is a
**perfect replica** of `p`, and `false` if it isn't.

Two trees are perfect replicas when both are empty, or when both have a root,
the roots hold equal values, their left subtrees are perfect replicas, and so
are their right subtrees. The arrangement counts as much as the values: a lone
left child doesn't replicate a lone right child, and the same values in other
places don't either. Either tree, or both, may be empty, and values can repeat.

An example lists `p`, then `q`, each written one row of the tree after
another, with `null` in a child spot that has no node.

@examples

## Constraints

- Each tree has between `0` and `100` nodes; the two sizes may differ.
- `-10^4 <= Node.val <= 10^4`
