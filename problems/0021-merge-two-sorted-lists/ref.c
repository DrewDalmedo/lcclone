/* Repeatedly move the smaller front node onto the tail of the result. */
struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    struct ListNode dummy, *tail = &dummy;

    while (list1 && list2) {
        if (list2->val < list1->val) {
            tail->next = list2;
            list2 = list2->next;
        } else {
            tail->next = list1;
            list1 = list1->next;
        }
        tail = tail->next;
    }
    tail->next = list1 ? list1 : list2;
    return dummy.next;
}
