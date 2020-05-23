#ifndef LIST_H
#define LIST_H
#include <iostream>

template<typename T>
class List {
private:
    class Node {
    public:
        T data;
        Node *prev, *next;
        explicit Node(T value) : data(value), prev(nullptr), next(nullptr) {}
        Node(T value, Node* prev, Node* next) : data(value), prev(prev), next(next) {}
    };
    size_t size;
    Node *head, *tail;
public:
    List() : head(nullptr), tail(nullptr), size(0) {}
    ~List() {
        this->clear();
    }
    void push_back(T value) {
        Node *temp = new Node(value);
        if (!isEmpty()) {
            temp->prev = tail;
            tail->next = temp;
            tail = temp;
            size++;
        } else {
            head = temp;
            tail = temp;
            size++;
        }
    }
    void push_front(T value) {
        Node *temp = new Node(value);
        if (!isEmpty()){
        head->prev = temp;
        temp->next = head;
        head = temp;
            size++;
		} else {
	       head = temp;
	       tail = temp;
	       size++;
		}
    }
   void pop_back() {
        Node *temp = tail;
        if (head != tail) {
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
            size--;
        } else {
            if (!isEmpty())
                size--;
            head = nullptr;
            tail = nullptr;
            delete temp;
        }
    }

    void pop_front() {
        Node *temp = head;
        if (head != tail) {
            head = head->next;
            head->prev = nullptr;
            delete temp;
            size--;
        } else {
            if (!isEmpty())
                size--;
            head = nullptr;
            tail = nullptr;
            delete temp;
        }
    }
    
    void insert(T value, size_t pos) {
        if (pos >= size || pos < 0)
            throw std::out_of_range("Out of range exception");
        size_t i = 0;
        Node *insertion_node = head;
        if (pos == 0)
            push_front(value);
        else if (pos == size-1)
            push_back(value);
        else {
            // Iterate through list to the node before one at pos
            while (i < pos - 1) {
                insertion_node = insertion_node->next;
                i++;
            }
            // Insert new node at pos
            Node *temp = new Node(value, insertion_node, insertion_node->next);
            insertion_node->next = temp;
            size++;
        }
    }

    T at(size_t pos) {
        if (pos >= size || pos < 0)
            throw std::out_of_range("Out of range exception");
        size_t i = 0;
        Node *temp = head;
        // Iterate through list to the node at pos
        while (i < pos) {
            temp = temp->next;
            i++;
        }
        return temp->data;
    }

    void remove(size_t pos) {
        if (pos >= size || pos < 0)
            throw std::out_of_range("Out of range exception");
        size_t i = 0;
        Node *temp = head;
        // Iterate through list to the node at pos
        while (i < pos) {
            temp = temp->next;
            i++;
        }
        if (temp == head) {
            if (!isEmpty())
                size--;
            head = head->next;
            head->prev = nullptr;
            delete temp;
        } else if (temp == tail) {
            if (!isEmpty())
                size--;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
        } else {
                size--;
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
        }
    }

    size_t get_size() {
        return size;
    }

    void print_to_console() {
        Node *temp = head;
        while (temp) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    void clear() {
        while (head)
            pop_front();
        size = 0;
    }

    void set(size_t pos, T value) {
        if (pos >= size || pos < 0)
            throw std::out_of_range("Out of range exception");
        size_t i = 0;
        Node *temp = head;
        // Iterate through list to the node at pos
        while (i < pos) {
            temp = temp->next;
            i++;
        }
        // If trying to access list at not existing index, throw an exception
        temp->data = value;
    }

    size_t find_first(List sublist) {
        const size_t list_s = this->get_size();
        const size_t sublist_s = sublist.get_size();
        // If sublist is longer than list to be searched, return error code
        if (sublist_s > list_s || sublist.isEmpty() || this->isEmpty())
            return -1;
        size_t i = 0;  // Index of first appearance of sublist
        Node *node = head;
        // Iterate through list excluding last sublist_s - 1 elements
        while (i <= list_s - sublist_s) {
            Node *temp = node;  // Creating temp node of main list
            size_t sublist_i = 0;  // Creating counter for sublist
            while (sublist_i < sublist_s && temp->data == sublist.at(sublist_i)) {
                temp = temp->next;
                sublist_i++;
            }
            if (sublist_i == sublist_s)
                return i;
            i++;
            node = node->next;
        }
        return -1;
    }

    bool isEmpty() {
        return this->head == nullptr;
    }
};

template<typename T>
class Queue {
private:
    // Defining nested class describing node
    class Node {
    public:
        T data;
        Node *prev, *next;

        // A constructor for Node class
        explicit Node(T value) : data(value), prev(nullptr), next(nullptr) {}
        Node(T value, Node* prev, Node* next) : data(value), prev(prev), next(next) {}
    };

    size_t size;
    Node *head, *tail;
public:
    // A default constructor for List class
    Queue() : head(nullptr), tail(nullptr), size(0) {}

    ~Queue() {
        this->clear();
    }

    void enqueue(T value) {
        Node *temp = new Node(value);
        if (!isEmpty()) {
            temp->prev = tail;
            tail->next = temp;
            tail = temp;
            size++;
        } else {
            head = temp;
            tail = temp;
            size++;
        }
    }

    void dequeue() {
        Node *temp = head;
        if (head != tail) {
            head = head->next;
            head->prev = nullptr;
            delete temp;
            size--;
        } else {
            if (!isEmpty())
                size--;
            head = nullptr;
            tail = nullptr;
            delete temp;
        }
    }

    T front(){
        if (this->head == nullptr)
            throw std::out_of_range("Queue is empty");
        return this->head->data;
    }

    size_t get_size() {
        return size;
    }

    bool isEmpty() {
        return this->head == nullptr;
    }

    void clear() {
        while (head)
            dequeue();
        size = 0;
    }
};
#endif
