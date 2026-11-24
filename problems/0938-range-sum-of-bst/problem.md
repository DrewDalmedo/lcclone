id: 938
slug: range-sum-of-bst
title: Range Sum of BST
difficulty: easy
tags: tree, depth-first-search, binary-search-tree, binary-tree
signature: int rangeSumBST(TreeNode root, int low, int high)
---
Together, `low` and `high` mark out a **band** of integers, both ends
included: `v` is in the band when `low <= v <= high`. Neither end has to occur
in the tree.

`root` is a binary search tree of distinct positive integers, so reading its
nodes in order (left subtree, node, right subtree) gives them from lowest to
highest. Return the total of the values in the tree that are in the band, or
`0` if none of them is.

Example inputs, in order: `root` as a level-order list (a `null` is a vacant
child position), `low`, `high`.

@examples

## Constraints

- The tree has between `1` and `2 * 10^4` nodes.
- `1 <= Node.val <= 10^5`
- `1 <= low <= high <= 10^5`
- `root` is a valid binary search tree, and all its values are distinct.
