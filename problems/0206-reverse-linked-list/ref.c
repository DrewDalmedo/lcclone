/* Move each node in turn onto the front of a new list. */
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode *out = NULL;

    while (head) {
        struct ListNode *next = head->next;

        head->next = out;
        out = head;
        head = next;
    }
    return out;
}
