/* Compare each node with every node visited before it. */
bool hasCycle(struct ListNode *head) {
    struct ListNode **seen = malloc(10001 * sizeof *seen);
    int n = 0, i;

    for (; head; head = head->next) {
        for (i = 0; i < n; i++) {
            if (seen[i] == head) {
                free(seen);
                return true;
            }
        }
        seen[n++] = head;
    }
    free(seen);
    return false;
}
