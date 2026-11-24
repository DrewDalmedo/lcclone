/* A queue of counterpart pairs, starting from the root's two children. Every
 * node is in one pair at most, so at most 1 + 2 * 499 pairs are ever added. */
bool isSymmetric(struct TreeNode* root)
{
    struct TreeNode *queue[2 * 1000];
    int head = 0, tail = 0;

    queue[tail++] = root->left;
    queue[tail++] = root->right;
    while (head < tail) {
        struct TreeNode *a = queue[head++];
        struct TreeNode *b = queue[head++];

        if (!a && !b)
            continue;
        if (!a || !b || a->val != b->val)
            return false;
        queue[tail++] = a->left;
        queue[tail++] = b->right;
        queue[tail++] = a->right;
        queue[tail++] = b->left;
    }
    return true;
}
