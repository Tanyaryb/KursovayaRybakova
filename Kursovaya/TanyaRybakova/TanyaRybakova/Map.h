#include <iostream>
#include "List.h"


enum Color {
    Black, Red, DoubleBlack
};

template<typename TKey, typename TValue>
class Map {
private:
    class Node {
    public:
        TKey key;
        TValue value;
        Color color;
        Node *parent, *left, *right;

        Node() = default;

        // Default color is Red, node isn't linked
        explicit Node(TKey key, TValue val, Node *nil) : key(key), value(val), color(Color::Red),
                                                 parent(nullptr), left(nil), right(nil) {};

        // Constructor with parent and color specified
        Node(TKey key, TValue val, Node *nil, Color color) : key(key), value(val), color(color),
                                                     parent(nullptr), left(nil), right(nil) {};

        // --------- Auxiliary methods -----------------
        Node *get_grandpa() {
            auto parent = this->parent;
            if (!parent || !parent->parent)
                return nullptr;
            auto grandpa = parent->parent;
            return grandpa;
        }

        Node *get_uncle() {
            auto grandpa = this->get_grandpa();
            // If nodes parent is a right child, return left child
            if (grandpa->right == this->parent)
                return grandpa->left;
            else
                return grandpa->right;
        }
        // ---------------------------------------------
    };

    void clear(Node *node) {
        if (node == nullptr || node == nil)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
        node = nil;
        if (node->left != nil)
            node->left = nil;
        if (node->right != nil)
            node->right = nil;
    }

    Node *bst_find(const TKey &key, bool insertion = false) {
        auto node = this->root;
        auto prev = node;
        while (node && node != this->nil && node->key != key) {
            prev = node;
            if (key > node->key)
                node = node->right;
            else
                node = node->left;
        }
        // If node with this key exist, return it (deletion use-case)
        if (node != this->nil) {
            if (!insertion) {
                return node;
            } else {
                if (node && node->key == key)
                    throw std::out_of_range("Duplicate key");
                else
                    throw std::out_of_range("Invalid key");
            }
        } else {
            if (insertion)
                return prev;
            else
                throw std::out_of_range("Invalid key");
        }
    }

    Node *bst_successor(Node *node) {
        node = node->right;
        while (node->left != nil)
            node = node->left;
        return node;
    }

    Node *left_rotation(Node *node) {
        auto new_node = new Node(node->key, node->value, nil);

        if (node->left != nil && node->right->left != nil)
            new_node->right = node->right->left;
        new_node->left = node->left;
        new_node->color = node->color;

        node->key = node->right->key;
        node->value = node->right->value;
        node->left = new_node;

        if (new_node->left)
            new_node->left->parent = new_node;
        if (new_node->right)
            new_node->right->parent = new_node;
        new_node->parent = node;

        if (node->right && node->right->right)
            node->right = node->right->right;
        else
            node->right = nil;

        if (node->right)
            node->right->parent = node;
        return new_node;
    }

    Node *right_rotation(Node *node) {
        auto new_node = new Node(node->key, node->value, nil);

        if (node->left != nil && node->left->right != nil)
            new_node->left = node->left->right;
        new_node->right = node->right;
        new_node->color = node->color;

        node->key = node->left->key;
        node->value = node->left->value;

        node->right = new_node;
        if (new_node->left)
            new_node->left->parent = new_node;
        if (new_node->right)
            new_node->right->parent = new_node;
        new_node->parent = node;


        if (node->left && node->left->left)
            node->left = node->left->left;
        else
            node->left = nil;

        if (node->left)
            node->left->parent = node;
        return new_node;
    }

    void resolve_insert_violations(Node *node) {
        while (node->parent->color == Color::Red && node->color == Color::Red) {
            auto grandpa = node->get_grandpa();
            auto uncle = node->get_uncle();
            // If parent is a left child of grandparent
            if (grandpa->left == node->parent) {
                if (uncle->color == Color::Red) {
                    node->parent->color = Color::Black;
                    uncle->color = Color::Black;
                    grandpa->color = Color::Red;
                    if (grandpa != root)
                        node = grandpa;
                    else
                        break;
                } else if (node == grandpa->left->right) {
                    // If uncle's color is black or it's null and path is LEFT-RIGHT
                    node = left_rotation(node->parent);
                } else {
                    grandpa->color = Color::Red;
                    node = right_rotation(grandpa);
                    node->parent->color = Color::Black;
                    if (grandpa != root)
                        node = grandpa;
                    else
                        break;
                }
            } else {
                // If parent is a right child of grandparent
                if (uncle->color == Color::Red) {
                    node->parent->color = Color::Black;
                    uncle->color = Color::Black;
                    grandpa->color = Color::Red;
                    if (grandpa != root)
                        node = grandpa;
                    else
                        break;
                } else if (node == grandpa->right->left)
                    node = right_rotation(node->parent);
                else {
                    grandpa->color = Color::Red;
                    node = left_rotation(grandpa);
                    node->parent->color = Color::Black;
                    if (grandpa != root)
                        node = grandpa;
                    else
                        break;
                }
            }
        }
        root->color = Color::Black;
    }

    void remove_with_fix(Node *node) {
        if (node == root) {
            delete root;
            root = nullptr;
            return;
        }

        // Simple case
        if (node->color == Color::Red || node->left->color == Color::Red || node->right->color == Color::Red) {
            // Choose left child if it exists, else choose right child
            auto child = node->left != nil ? node->left : node->right;

            // Remove node
            if (node == node->parent->left) {
                node->parent->left = child;
                if (child != nil)
                    child->parent = node->parent;
                child->color = Color::Black;
                delete node;
            } else {
                node->parent->right = child;
                if (child != nil)
                    child->parent = node->parent;
                child->color = Color::Black;
                delete node;
            }
        }
            // Cases with black node
        else {
            Node *sibling = nullptr;
            Node *parent = nullptr;
            Node *ptr = node;
            ptr->color = Color::DoubleBlack;

            while (ptr != root && ptr->color == Color::DoubleBlack) {
                parent = ptr->parent;
                // If double-black node is a left child
                if (ptr == parent->left) {
                    sibling = parent->right;
                    // If sibling's color is red
                    if (sibling->color == Color::Red) {
                        sibling->color = Color::Black;
                        parent->color = Color::Red;
                        left_rotation(parent);
                    }
                        // If sibling's color is black
                    else {
                        if (sibling->left->color == Color::Black && sibling->right->color == Color::Black) {
                            sibling->color = Color::Red;
                            if (parent->color == Color::Red)
                                parent->color = Color::Black;
                            else
                                parent->color = Color::DoubleBlack;
                            ptr = parent;
                        } else {
                            if (sibling->right->color == Color::Black) {
                                sibling->left->color = Color::Black;
                                sibling->color = Color::Red;
                                right_rotation(sibling);
                                sibling = parent->right;
                            }
                            sibling->color = parent->color;
                            parent->color = Color::Black;
                            sibling->right->color = Color::Black;
                            left_rotation(parent);
                            break;
                        }
                    }
                }
                    // If double-black node is a right child
                else {
                    sibling = parent->left;
                    // If sibling's color is red
                    if (sibling->color == Color::Red) {
                        sibling->color = Color::Black;
                        parent->color = Color::Red;
                        right_rotation(parent);
                    }
                        // If sibling's color is black
                    else {
                        // and both its children are black, recolor
                        if (sibling->left->color == Color::Black && sibling->right->color == Color::Black) {
                            sibling->color = Color::Red;
                            if (parent->color == Color::Red)
                                parent->color = Color::Black;
                            else
                                parent->color = Color::DoubleBlack;
                            ptr = parent;
                        }
                            // else, when at least one of its children is red, rotate
                        else {
                            if (sibling->left->color == Color::Black) {
                                sibling->right->color = Color::Black;
                                sibling->color = Color::Red;
                                left_rotation(sibling);
                                sibling = parent->left;
                            }
                            sibling->color = parent->color;
                            parent->color = Color::Black;
                            sibling->left->color = Color::Black;
                            right_rotation(parent);
                            break;
                        }
                    }
                }
            }
            if (node == node->parent->left)
                node->parent->left = nil;
            else
                node->parent->right = nil;
            delete node;
            root->color = Color::Black;
        }
    }

// Map private fields
    Node *root;
    Node *nil;

public:

    Map() : root(nullptr), nil(new Node()) {
        nil->left = nil;
        nil->right = nil;
    };


    void insert(const TKey &key, TValue value) {
        auto new_node = new Node(key, value, nil);
        // If tree is empty
        if (root == nullptr) {
            root = new_node;
            root->color = Color::Black;
        } else {
            // Find a place for a node violating RB-tree properties
            auto parent = this->bst_find(key, true);
            if (new_node->key > parent->key)
                parent->right = new_node;
            else
                parent->left = new_node;
            new_node->parent = parent;
            resolve_insert_violations(new_node);
        }
    }

    void remove(const TKey &key) {
        // Find node to remove
        auto node = this->bst_find(key);
        if (node == nullptr || node == nil)
            return;

        if (node->left != nil && node->right == nil) {
            // Case when node has only left child
            node->key = node->left->key;
            node->value = node->left->value;
            // Select child for removal
            node = node->left;
        } else if (node->right != nil && node->left == nil) {
            // Case when node has only right child
            node->key = node->right->key;
            node->value = node->right->value;
            // Select child for removal
            node = node->right;
        } else if (node->left != nil && node->right != nil) {
            // Case when node has both children
            auto successor = this->bst_successor(node);
            node->key = successor->key;
            node->value = successor->value;
            node = successor;
        }
        remove_with_fix(node);
    }

    void set(const TKey &key, TValue value) {
        auto node = bst_find(key);
        node->value = value;
    }

    TValue find(const TKey &key) {
        auto node = bst_find(key);
        if (node == nullptr)
            throw std::out_of_range("Key doesn't exist");
        if (node->key == key)
            return node->value;
        else {
            if (node->left == node) {
                if (node->left->key == key)
                    return node->left->value;
            } else {
                if (node->right->key == key)
                    return node->right->value;
            }
        }
    }

    TValue operator[](const TKey &key) {
        return find(key);
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    List<TKey> get_keys() {
        List<TKey> keys_list;
        if (root) {
            auto node = root;
            List<Node *> stack;
            stack.push_front(node);
            while (!stack.isEmpty()) {
                node = stack.at(0);
                stack.pop_front();
                keys_list.push_back(node->key);
                if (node->right != nil)
                    stack.push_front(node->right);
                if (node->left != nil)
                    stack.push_front(node->left);
            }
        }
        return keys_list;
    }

    List<TValue> get_values() {
        List<TValue> values_list;
        if (root) {
            auto node = root;
            List<Node *> stack;
            stack.push_front(node);
            while (!stack.isEmpty()) {
                node = stack.at(0);
                stack.pop_front();
                values_list.push_back(node->value);
                if (node->right != nil)
                    stack.push_front(node->right);
                if (node->left != nil)
                    stack.push_front(node->left);
            }
        }
        return values_list;
    }

    void print() {
        std::cout << '{';
        if (root) {
            auto node = root;
            List<Node *> stack;
            stack.push_front(node);
            while (!stack.isEmpty()) {
                node = stack.at(0);
                if (node != root)
                    std::cout << ", ";
                stack.pop_front();
                std::cout << node->key << ": " << node->value;
                if (node->right != nil)
                    stack.push_front(node->right);
                if (node->left != nil)
                    stack.push_front(node->left);
            }
        }
        std::cout << '}' << std::endl;
    }
};
