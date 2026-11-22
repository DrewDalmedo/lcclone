id: 1379
slug: find-a-corresponding-node-of-a-binary-tree-in-a-clone-of-that-tree
title: Find a Corresponding Node of a Binary Tree in a Clone of That Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: TreeNode getTargetCopy(TreeNode original, TreeNode cloned, TreeNode target)
build: copy cloned original
ref: target in original, return in cloned
---
`cloned` was made from the binary tree `original` by copying every node, so
the two have the same shape and values but no node in common. `target`
points at one node of `original`. Find its **twin**, the node of `cloned`
reached from the root by the same moves left and right, and return it.

The answer has to be a node of `cloned`: returning `target` itself is wrong,
even though it looks the same. Change neither tree.

An example gives `original` in level order, row by row with `null` for a
missing child (a missing node's children aren't listed, and the `null`s at
the end are left out), then `target` as the value of its node. `cloned` isn't
written out, since it is identical. The output shows the subtree of the node
you return, in the same form.

@examples

## Constraints

- The tree has between `1` and `10^4` nodes.
- `1 <= Node.val <= 10^5`
- All values in the tree are distinct.
- `target` is a node of `original`.

## Follow-up

Does your method rely on the values being distinct? One that moves through
both trees in step doesn't.
