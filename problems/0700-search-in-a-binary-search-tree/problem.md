id: 700
slug: search-in-a-binary-search-tree
title: Search in a Binary Search Tree
difficulty: easy
tags: tree, binary-search-tree, binary-tree
signature: TreeNode searchBST(TreeNode root, int val)
---
Think of every node as the top of a **branch**: the node itself and everything
hanging below it. No value repeats in the binary search tree `root`, and from
any node, going left leads only to lower values and going right only to higher
ones.

Return the top of the branch that starts at `val`: the node holding that value,
with everything below it left in place. When the tree has no such value there
is no branch to give back, so return null, the empty tree, printed as `[]`.

Trees and branches are written in level order: `[8,3]` is a branch topped by 8
with 3 as its left child, and `[8,null,11]` has 11 on the right instead. In an
example, `val` comes on the line after the tree.

@examples

## Constraints

- The tree has between `1` and `5000` nodes.
- `1 <= Node.val <= 10^7`
- `1 <= val <= 10^7`
- `root` is a valid binary search tree, so all its values are distinct.
