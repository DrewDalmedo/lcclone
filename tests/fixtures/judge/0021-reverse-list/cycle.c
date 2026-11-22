/* Forgets to cut the old head's link: it points back into the list. */
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode* prev = NULL;
    struct ListNode* first = head;

    while (head) {
        struct ListNode* next = head->next;

        head->next = prev;
        prev = head;
        head = next;
    }
    if (first && first != prev)
        first->next = prev->next;
    return prev;
}
