#include <iostream>
#include "queue.h"

Queue* init(void) {
    Queue* q = new Queue;
    q->head = nullptr;
    q->tail = nullptr;
    return q;
}

void release(Queue* queue) {
    if (!queue)
        return;
    {
        std::lock_guard<std::mutex> lock(queue->mtx);
        Node* curr = queue->head;
        while (curr != nullptr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        queue->head = nullptr;
        queue->tail = nullptr;
    }
    delete queue;
}

Node* nalloc(Item item) {
    Node* node = new Node;
    if (node != nullptr) {
        node->item = item;  // 얕은 복사 (Item.value는 호출 측에서 관리)
        node->next = nullptr;
    }
    return node;
}

void nfree(Node* node) {
    delete node;
}

Node* nclone(Node* node) {
    if (!node)
        return nullptr;
    Node* newNode = new Node;
    newNode->item = node->item;
    newNode->next = nullptr;
    return newNode;
}

Reply enqueue(Queue* queue, Item item) {
	Reply reply = { false, NULL };
	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, NULL };
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}
