/* Stay on a node while its successor repeats its value. */
struct ListNode* deleteDuplicates(struct ListNode* head) {
    struct ListNode *p = head;

    while (p && p->next) {
        if (p->next->val == p->val)
            p->next = p->next->next;
        else
            p = p->next;
    }
    return head;
}
