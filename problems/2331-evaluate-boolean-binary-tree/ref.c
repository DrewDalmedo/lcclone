/* Lists the nodes breadth first, so that every child comes after its parent,
 * then works out the signals from the end of the list back to the root. */
bool evaluateTree(struct TreeNode* root) {
    struct TreeNode** order = malloc(1001 * sizeof *order);
    int* first_child = malloc(1001 * sizeof *first_child);
    bool* signal = malloc(1001 * sizeof *signal);
    int count = 1, i;
    bool answer;

    order[0] = root;
    for (i = 0; i < count; i++) {
        struct TreeNode* p = order[i];

        first_child[i] = count;
        if (p->left) {
            order[count++] = p->left;
            order[count++] = p->right;
        }
    }
    for (i = count - 1; i >= 0; i--) {
        struct TreeNode* p = order[i];

        if (!p->left) {
            signal[i] = p->val == 1;
        } else {
            bool a = signal[first_child[i]], b = signal[first_child[i] + 1];

            signal[i] = p->val == 2 ? (a || b) : (a && b);
        }
    }
    answer = signal[0];
    free(order);
    free(first_child);
    free(signal);
    return answer;
}
