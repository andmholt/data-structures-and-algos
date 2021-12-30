#ifndef RBBST_H
#define RBBST_H

#include "bst.h"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the height as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template<typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~ Helper Funcs ~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~ Methods ~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * An explicit constructor to initialize the elements by calling the base class constructor and setting
 * the color to red since every new node will be red when it is first inserted.
 */
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
        : Node<Key, Value>(key, value, parent), height_(1) {}

/**
 * A destructor which does nothing.
 */
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

/**
 * A getter for the height of a AVLNode.
 */
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const {
    return height_;
}

/**
 * A setter for the height of a AVLNode.
 */
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height) {
    height_ = height;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template<class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    virtual void insert(const std::pair<const Key, Value>& new_item);  // TODO
    virtual void remove(const Key& key);                               // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // Add helper functions here
    bool isLeftChildOf(const AVLNode<Key, Value>* child, const AVLNode<Key, Value>* parent) const;
    bool isRightChildOf(const AVLNode<Key, Value>* child, const AVLNode<Key, Value>* parent) const;
    bool hasRightChild(const AVLNode<Key, Value>* parent) const;
    bool hasLeftChild(const AVLNode<Key, Value>* parent) const;
    bool hasTwoChildren(const AVLNode<Key, Value>* parent) const;
    bool hasNoChildren(const AVLNode<Key, Value>* parent) const;
    void refreshHeight(AVLNode<Key, Value>* node) const;
    int getGreatestChildHeight(const AVLNode<Key, Value>* parent) const;

    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    bool isRoot(const AVLNode<Key, Value>* node) const;
    void bubbleUpHeight(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* getPredecessor(AVLNode<Key, Value>* node) const;

    void printValues() const;
};
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~ Helper Funcs ~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChildOf(const AVLNode<Key, Value>* child, const AVLNode<Key, Value>* parent) const {
    // if parent is null
    if (parent == nullptr)
        return false;

    return parent->getLeft() == child;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChildOf(const AVLNode<Key, Value>* child, const AVLNode<Key, Value>* parent) const {
    // if parent is null
    if (parent == nullptr)
        return false;

    return parent->getRight() == child;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::hasRightChild(const AVLNode<Key, Value>* parent) const {
    return parent->getRight() != nullptr;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::hasLeftChild(const AVLNode<Key, Value>* parent) const {
    return parent->getLeft() != nullptr;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::hasTwoChildren(const AVLNode<Key, Value>* parent) const {
    return (parent->getLeft() != nullptr && parent->getRight() != nullptr);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::hasNoChildren(const AVLNode<Key, Value>* parent) const {
    return (parent->getLeft() == nullptr && parent->getRight() == nullptr);
}

template<class Key, class Value>
void AVLTree<Key, Value>::refreshHeight(AVLNode<Key, Value>* node) const {

    // if no children
    if (node->getLeft() == nullptr && node->getRight() == nullptr) {
        static_cast<AVLNode<Key, Value>*>(node)->setHeight(1);
        return;
    }

    // if left child only
    else if (node->getRight() == nullptr) {
        static_cast<AVLNode<Key, Value>*>(node)->setHeight(
                static_cast<AVLNode<Key, Value>*>(node->getLeft())->getHeight() + 1);
        return;
    }

    // if right child only
    else if (node->getLeft() == nullptr) {
        static_cast<AVLNode<Key, Value>*>(node)->setHeight(
                static_cast<AVLNode<Key, Value>*>(node->getRight())->getHeight() + 1);
        return;
    }

    // if both children
    int hL = static_cast<AVLNode<Key, Value>*>(node->getLeft())->getHeight();
    int hR = static_cast<AVLNode<Key, Value>*>(node->getRight())->getHeight();

    if (hL > hR)
        static_cast<AVLNode<Key, Value>*>(node)->setHeight(hL + 1);
    else
        static_cast<AVLNode<Key, Value>*>(node)->setHeight(hR + 1);
}

template<class Key, class Value>
int AVLTree<Key, Value>::getGreatestChildHeight(const AVLNode<Key, Value>* node) const {

    // if no children
    if (node->getLeft() == nullptr && node->getRight() == nullptr)
        return 0;

    // if left child only
    else if (node->getRight() == nullptr)
        return node->getLeft()->getHeight();

    // if right child only
    else if (node->getLeft() == nullptr)
        return node->getRight()->getHeight();

    // if both children
    int hL = node->getLeft()->getHeight();
    int hR = node->getRight()->getHeight();

    if (hL > hR)
        return hL;
    else
        return hR;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {

    // find parent
    AVLNode<Key, Value>* parent = node->getParent();

    // if zigzag, rotate left
    if (parent->getRight() == node && node->getLeft() != nullptr && node->getRight() == nullptr) {
        rotateRight(node->getLeft());
        node = node->getParent();
    }

    // do some changes
    parent->setRight(node->getLeft());
    node->setParent(parent->getParent());
    node->setLeft(parent);
    if (parent->getRight() != nullptr)
        parent->getRight()->setParent(parent);

    // set old parent of parent to node, as long as parent of node is not root
    if (!isRoot(parent)) {
        if (isLeftChildOf(parent, parent->getParent()))
            parent->getParent()->setLeft(node);
        else if (isRightChildOf(parent, parent->getParent()))
            parent->getParent()->setRight(node);
    }

    // set old parent's parent
    parent->setParent(node);

    // if root, set root
    if (isRoot(parent)) {
        this->root_ = node;
    }

    // adjust heights
    refreshHeight(parent);
    refreshHeight(node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {

    // find parent
    AVLNode<Key, Value>* parent = node->getParent();

    // if zigzag, rotate left
    if (parent->getLeft() == node && node->getRight() != nullptr && node->getLeft() == nullptr) {
        rotateLeft(node->getRight());
        node = node->getParent();
    }

    // do some changes
    parent->setLeft(node->getRight());
    node->setParent(parent->getParent());
    node->setRight(parent);
    if (parent->getLeft() != nullptr)
        parent->getLeft()->setParent(parent);

    // set old parent of parent to node
    if (!isRoot(parent)) {
        if (isRightChildOf(parent, parent->getParent()))
            parent->getParent()->setRight(node);
        else if (isLeftChildOf(parent, parent->getParent()))
            parent->getParent()->setLeft(node);
    }

    // set old parent's parent
    parent->setParent(node);

    // if root, set root
    if (isRoot(parent)) {
        this->root_ = node;
    }

    // adjust heights
    refreshHeight(parent);
    refreshHeight(node);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRoot(const AVLNode<Key, Value>* node) const {
    return node == this->root_;
}

template<class Key, class Value>
void AVLTree<Key, Value>::bubbleUpHeight(AVLNode<Key, Value>* node) {

    // set height of starting node
    node->setHeight(1);

    // bubble up and set height
    int newHeight = 2;
    while (true) {
        if (node->getParent() == nullptr)
            break;
        node = node->getParent();
        if (node->getHeight() < newHeight)
            node->setHeight(newHeight++);
        else
            break;
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getPredecessor(AVLNode<Key, Value>* node) const {

    // if smallest node
    if (node->getLeft() == nullptr)
        return nullptr;

    // else look down tree
    else {
        AVLNode<Key, Value>* currNode = node->getLeft();
        while (currNode->getRight() != nullptr)
            currNode = currNode->getRight();

        return currNode;
    }
}

// Helper func for testing; prints all values of all nodes in tree
template<class Key, class Value>
void AVLTree<Key, Value>::printValues() const {

    std::cout << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "****PRINT VALUES****" << std::endl;

    AVLNode<Key, Value>* currNode;
    int key;
    int label = 1;
    for (typename BinarySearchTree<Key, Value>::iterator treeIter = this->begin(); treeIter != this->end();
         ++treeIter) {

        // get key from iterator
        key = treeIter->first;

        // find node given key
        currNode = static_cast<AVLNode<Key, Value>*>(this->root_);
        while (key != currNode->getKey()) {
            if (key < currNode->getKey()) {
                if (currNode->getLeft() != nullptr)
                    currNode = currNode->getLeft();
                else {
                    std::cout << "ERROR: iterator gave invalid key" << std::endl;
                    break;
                }
            } else if (key > currNode->getKey()) {
                if (currNode->getRight() != nullptr)
                    currNode = currNode->getRight();
                else {
                    std::cout << "ERROR: iterator gave invalid key" << std::endl;
                    break;
                }
            }
        }

        // print...
        std::cout << "~~~~~~~~~~~~~~~~~~~~" << std::endl;

        // label
        if (label < 10)
            std::cout << "[0" << label++ << ']' << std::endl;
        else
            std::cout << '[' << label++ << ']' << std::endl;

        // key
        std::cout << "KEY: " << currNode->getKey() << std::endl;

        // height
        std::cout << "HEIGHT: " << currNode->getHeight() << std::endl;

        // parent
        if (currNode->getParent() != nullptr)
            std::cout << "PARENT: " << currNode->getParent()->getKey() << std::endl;
        else
            std::cout << "PARENT: NULL" << std::endl;

        // left
        if (currNode->getLeft() != nullptr)
            std::cout << "LEFT: " << currNode->getLeft()->getKey() << std::endl;
        else
            std::cout << "LEFT: NULL" << std::endl;

        // right
        if (currNode->getRight() != nullptr)
            std::cout << "RIGHT: " << currNode->getRight()->getKey() << std::endl;
        else
            std::cout << "RIGHT: NULL" << std::endl;

        std::cout << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << std::endl;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~ Methods ~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    // TODO

    // create new node
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);

    // if first node
    if (this->root_ == nullptr) {
        this->root_ = newNode;
        newNode->setHeight(1);
        return;
    }

    // insert to tree
    AVLNode<Key, Value>* currNode = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (true) {
        // if key already exists
        if (newNode->getKey() == currNode->getKey()) {
            currNode->setValue(newNode->getValue());
            delete newNode;
            return;
        }

        // if less than
        else if (newNode->getKey() < currNode->getKey()) {
            // if left node free, place new node there
            if (currNode->getLeft() == nullptr) {
                currNode->setLeft(newNode);
                newNode->setParent(currNode);
                break;
            }
            // if not free, continue the search
            else {
                currNode = currNode->getLeft();
                continue;
            }
        }

        // if greater than
        else if (newNode->getKey() > currNode->getKey()) {
            // if right node free, place new node there
            if (currNode->getRight() == nullptr) {
                currNode->setRight(newNode);
                newNode->setParent(currNode);
                break;
            }
            // if not free, continue the search
            else {
                currNode = currNode->getRight();
                continue;
            }
        }
    }

    AVLNode<Key, Value>* unbalNode = newNode;
    // if tree is unbalanced, balance it
    if (!this->isBalanced()) {
        // find first parent that is unbalanced
        while (this->balancedHelper(unbalNode) != -1) {
            // if not unbalanced node, move up
            unbalNode = unbalNode->getParent();
        }
        // find which side is unbalanced
        if (unbalNode->getLeft() == nullptr)
            rotateLeft(unbalNode->getRight());
        else if (unbalNode->getRight() == nullptr)
            rotateRight(unbalNode->getLeft());
        else if (unbalNode->getLeft()->getHeight() < unbalNode->getRight()->getHeight())
            rotateLeft(unbalNode->getRight());
        else
            rotateRight(unbalNode->getLeft());
    }

    // adjust heights
    bubbleUpHeight(newNode);
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // TODO

    // if tree empty
    if (this->root_ == nullptr)
        return;

    // find node
    AVLNode<Key, Value>* currNode = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (key != currNode->getKey()) {
        // if key not in tree
        if (currNode->getLeft() == nullptr && currNode->getRight() == nullptr)
            return;
        // if key larger than current node
        else if (key > currNode->getKey()) {
            if (currNode->getRight() != nullptr)
                currNode = currNode->getRight();
            else
                return;
        }
        // if key smaller than current node
        else if (key < currNode->getKey()) {
            if (currNode->getLeft() != nullptr)
                currNode = currNode->getLeft();
            else
                return;
        }
    }

    // if only node
    if (currNode == this->root_ && hasNoChildren(currNode)) {
        delete currNode;
        this->root_ = nullptr;
        return;
    }

    // while node has both children, swap with predecessor
    while (hasTwoChildren(currNode))
        nodeSwap(currNode, getPredecessor(currNode));

    // if root node and one child
    if (currNode == this->root_ && !hasTwoChildren(currNode)) {
        if (hasRightChild(currNode))
            this->root_ = currNode->getRight();
        else if (hasLeftChild(currNode))
            this->root_ = currNode->getLeft();

        this->root_->setParent(nullptr);
        delete currNode;
        return;
    }

    // if no children
    else if (hasNoChildren(currNode)) {
        if (isLeftChildOf(currNode, currNode->getParent()))
            currNode->getParent()->setLeft(nullptr);
        else
            currNode->getParent()->setRight(nullptr);
    }

    // if left child only
    else if (hasLeftChild(currNode)) {
        if (isLeftChildOf(currNode, currNode->getParent()))
            currNode->getParent()->setLeft(currNode->getLeft());
        else if (isRightChildOf(currNode, currNode->getParent()))
            currNode->getParent()->setRight(currNode->getLeft());

        currNode->getLeft()->setParent(currNode->getParent());
    }

    // if right child only
    else if (hasRightChild(currNode)) {
        if (isLeftChildOf(currNode, currNode->getParent()))
            currNode->getParent()->setLeft(currNode->getRight());
        else if (isRightChildOf(currNode, currNode->getParent()))
            currNode->getParent()->setRight(currNode->getRight());

        currNode->getRight()->setParent(currNode->getParent());
    }

    // adjust heights
    AVLNode<Key, Value>* heightNode = currNode->getParent();
    while (getGreatestChildHeight(heightNode) != (heightNode)->getHeight() - 1) {
        refreshHeight(heightNode);
        heightNode = heightNode->getParent();
        if (heightNode == nullptr)
            break;
    }

    AVLNode<Key, Value>* unbalNode = currNode->getParent();
    // if tree is unbalanced, balance it
    if (!this->isBalanced()) {
        // find first parent that is unbalanced
        while (this->balancedHelper(unbalNode) != -1) {
            // if not unbalanced node, move up
            unbalNode = unbalNode->getParent();
        }
        // find which side is unbalanced
        if (unbalNode->getLeft() == nullptr)
            rotateLeft(unbalNode->getRight());
        else if (unbalNode->getRight() == nullptr)
            rotateRight(unbalNode->getLeft());
        else if (unbalNode->getLeft()->getHeight() < unbalNode->getRight()->getHeight())
            rotateLeft(unbalNode->getRight());
        else
            rotateRight(unbalNode->getLeft());
    }

    // delete
    delete currNode;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif
