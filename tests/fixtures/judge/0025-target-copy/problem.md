id: 25
slug: target-copy
title: Target Copy
difficulty: easy
signature: TreeNode getTargetCopy(TreeNode original, TreeNode cloned, TreeNode target)
build: copy cloned original
ref: target in original, return in cloned
---
A fixture for `build: copy` and `ref:`: `cloned` is a copy of `original`
(a case gives only `original`), `target` is the node of `original` with
the value given, and the function must return the node of `cloned` that
matches it. The result shows the tree from that node.
