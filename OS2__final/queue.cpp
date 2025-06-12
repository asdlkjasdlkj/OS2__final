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
        node->item = item;
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
    Reply rep;
    rep.success = false;

    Node* newNode = nalloc(item);
    if (!newNode)
        return rep;
    {
        std::lock_guard<std::mutex> lock(queue->mtx);

        if (queue->head == nullptr) {
            queue->head = newNode;
            queue->tail = newNode;
        }
        else {
            Node* curr = queue->head;
            Node* prev = nullptr;

            while (curr != nullptr) {
                if (curr->item.key == item.key) {
                    curr->item.value = item.value;
                    rep.success = true;
                    return rep;
                }
                prev = curr;
                curr = curr->next;
            }
            if (item.key > queue->head->item.key) {
                newNode->next = queue->head;
                queue->head = newNode;
            }
            else {
                prev->next = newNode;
                newNode->next = curr;
                if (newNode->next == nullptr) {
                    queue->tail = newNode;
                }
            }
        }
    }
    rep.success = true;
    return rep;
}

Reply dequeue(Queue* queue) {
    Reply rep;
    rep.success = false;
    {
        std::lock_guard<std::mutex> lock(queue->mtx);
        if (queue->head == nullptr) {
            return rep;
        }
        Node* temp = queue->head;
        rep.item = temp->item;
        queue->head = queue->head->next;
        if (queue->head == nullptr)
            queue->tail = nullptr;
        delete temp;
        rep.success = true;
    }
    return rep;
}

Queue* range(Queue* queue, Key start, Key end) {
    Queue* newQueue = init();
    if (!newQueue)
        return nullptr;
    {
        std::lock_guard<std::mutex> lock(queue->mtx);
        Node* curr = queue->head;
        while (curr != nullptr) {
            if (curr->item.key >= start && curr->item.key <= end) {
                enqueue(newQueue, curr->item);
            }
            curr = curr->next;
        }
    }
    return newQueue;
}

