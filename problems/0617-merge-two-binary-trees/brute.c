/* From the definition: a new node for each address either tree uses, with
 * a missing node counting as 0 and having no children. */
struct TreeNode* mergeTrees(struct TreeNode* root1, struct TreeNode* root2) {
    struct TreeNode *node;

    if (!root1 && !root2)
        return NULL;
    node = malloc(sizeof *node);
    node->val = (root1 ? root1->val : 0) + (root2 ? root2->val : 0);
    node->left = mergeTrees(root1 ? root1->left : NULL,
                            root2 ? root2->left : NULL);
    node->right = mergeTrees(root1 ? root1->right : NULL,
                             root2 ? root2->right : NULL);
    return node;
}
