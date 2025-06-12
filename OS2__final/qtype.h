#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>

typedef int Key;
typedef void* Value;

typedef struct {
    Key key;
    Value value;
} Item;

typedef struct {
    bool success;
    Item item;
} Reply;

struct Node {
    Item item;
    Node* next;
};

struct Queue {
    Node* head;
    Node* tail;
    std::mutex mtx;
};

#endif 