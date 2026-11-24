id: 897
slug: increasing-order-search-tree
title: Increasing Order Search Tree
difficulty: easy
tags: stack, tree, depth-first-search, binary-search-tree, binary-tree
signature: TreeNode increasingBST(TreeNode root)
---
Turn the binary search tree `root`, whose values are all different, into a
**ladder** of the same values. A ladder is a single line of nodes joined only
by right-child links:

- its top node holds the smallest value;
- one step down, through a right child, always reaches the next larger value;
- the left child of every rung is empty;
- the bottom rung holds the largest value and has no children.

Return the top node. Rungs may be the original nodes, rewired, or freshly
allocated copies.

A ladder prints as its values with a `null` between neighbours, for the empty
left slot of each rung: `[3,null,8,null,14]` is 3, with 8 as its right child
and 14 as the right child of 8. Input trees use the same level-order form.

@examples

## Constraints

- The tree has between `1` and `100` nodes.
- `0 <= Node.val <= 1000`
- `root` is a valid binary search tree, so all its values are distinct.
