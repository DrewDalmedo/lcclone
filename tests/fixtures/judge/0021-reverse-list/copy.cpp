// Builds a new list, freeing the old nodes as it goes.
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* out = nullptr;
        while (head) {
            out = new ListNode(head->val, out);
            ListNode* next = head->next;
            delete head;
            head = next;
        }
        return out;
    }
};
