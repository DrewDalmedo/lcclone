/* Only nodes equal to the root can have a better candidate below them: a
 * larger node is a candidate, and everything under it is at least as large.
 * A flag says whether a candidate exists, since 2^31 - 1 can be the answer. */
int findSecondMinimumValue(struct TreeNode* root) {
    struct TreeNode *stack[26];
    int top = 0, found = 0, best = 0;

    stack[top++] = root;
    while (top > 0) {
        struct TreeNode *t = stack[--top];

        if (t->val > root->val) {
            if (!found || t->val < best) {
                best = t->val;
                found = 1;
            }
        } else if (t->left) {
            stack[top++] = t->left;
            stack[top++] = t->right;
        }
    }
    return found ? best : -1;
}
