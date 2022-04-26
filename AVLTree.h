

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H

#include <iostream>
//#include <math.h>
using std::shared_ptr;
using std::weak_ptr;

template<class T, class Pred>
class AVLTree {
    struct TreeNode {
        shared_ptr<TreeNode> right;
        shared_ptr<TreeNode> left;
        weak_ptr<TreeNode> father;
        shared_ptr<T> data;
        int height;
    };

    Pred predicate;
    shared_ptr<TreeNode> root;
    shared_ptr<TreeNode> max;
    int size; // initialize size

    AVLTree() =default;
    ~AVLTree() =default;


    // private functions
    shared_ptr<TreeNode> initNode(shared_ptr<T> data) {
        shared_ptr<TreeNode> node = shared_ptr<TreeNode>(new TreeNode());

        node->data = data;
        node->left = nullptr;
        node->right = nullptr;
        node->father = weak_ptr<TreeNode>();
        node->height = 0;

        return node;
    }
    shared_ptr<TreeNode> findNode(shared_ptr<TreeNode> treeNode, shared_ptr<T> toFind) const {
        if (treeNode == nullptr) {
            return nullptr;
        }
        if (treeNode->data == toFind) {
            return treeNode;
        }

        if (predicate(toFind, treeNode->data)) {
            return findNode(treeNode->left, toFind);
        }
        return findNode(treeNode->right, toFind);
    }
    int BF(shared_ptr<TreeNode> treeNode) const {
        int hl = 0, hr = 0;
        if (treeNode->height == 0) {
            return hl -hr;
        }

        if (treeNode->left != nullptr) {
            hl = treeNode->left->height;
        }

        if (treeNode->right == nullptr) {
            hr = treeNode->right->height;
        }

        return hl - hr;
    }
    int getHeight(shared_ptr<TreeNode> node) const {
        return node == nullptr ? -1 : node->height;
    }
    shared_ptr<TreeNode> insertNode(shared_ptr<TreeNode> toAdd, shared_ptr<TreeNode> target,
                                    shared_ptr<TreeNode> targetFather) {
        if (target == nullptr) {
            toAdd->father = targetFather;
            return toAdd;
        }

        // toAdd < target
        if (predicate(toAdd->data, target->data)) {
            target->left = insertNode(toAdd, target->left, target);
        } else { // target >= toAdd
            target->right = insertNode(toAdd, target->right, target);
        }

        target->height = maxInt(getHeight(target->left), getHeight(target->right)) + 1;

        balanceTree(target);

        target->height = maxInt(getHeight(target->left), getHeight(target->right)) + 1;
        return target;
    }
    void balanceTree(shared_ptr<TreeNode> target) {
        if (BF(target) < -1) {
            if (BF(target->right) == 1) {
                rotateRL(target);
            } else {
                rotateRR(target);
            }
        } else if (BF(target) > 1) {
            if (BF(target->left) == -1) {
                rotateLR(target);
            } else {
                rotateLL(target);
            }
        }
    }

    void removeNode(shared_ptr<TreeNode> toRemove) {
        if (toRemove->right && toRemove->left) {
            shared_ptr<TreeNode> temp = toRemove->right;
            while (temp->left) {
                temp = temp->left;
            }
            shared_ptr<T> tempData = temp->data;
            temp->data = toRemove->data;
            toRemove->data = tempData;

            toRemove = temp;
        }

        shared_ptr<TreeNode> toBalance = toRemove->father.lock();
        removeNodeWithLessThanTwoSons(toRemove);


        while (toBalance) {
            toBalance->height = maxInt(getHeight(toBalance->left), getHeight(toBalance->right)) + 1;
            balanceTree(toBalance);
            toBalance = toBalance->father;
        }
    }
    void removeNodeWithLessThanTwoSons(shared_ptr<TreeNode> toRemove) {
        if (!toRemove->left && !toRemove->right) { // toRemove is a leaf
            if (!toRemove->father) { // root is a leaf
                this->root = nullptr;
            } else if (toRemove->father->left == toRemove) {
                toRemove->father->left = nullptr;
            } else {
                toRemove->father->right = nullptr;
            }
        }
        if (!toRemove->left) { // toRemove has right son only
            if (!toRemove->father) { // root is a
                this->root = toRemove->right;
                toRemove->right->father = nullptr;
            } else if (toRemove->father->left == toRemove) {
                toRemove->father->left = toRemove->right;
                toRemove->right->father = toRemove->father;
            } else {
                toRemove->father->right = toRemove->right;
                toRemove->right->father = toRemove->father;
            }
        }

        if (!toRemove->right) { // toRemove has left son only
            if (!toRemove->father) { // root is a
                this->root = toRemove->left;
                toRemove->left->father = nullptr;
            } else if (toRemove->father->left == toRemove) {
                toRemove->father->left = toRemove->left;
                toRemove->left->father = toRemove->father;
            } else {
                toRemove->father->right = toRemove->left;
                toRemove->left->father = toRemove->father;
            }
        }
    }
    void fixMax() {
        this->max = root;
        if (!root) {
            while (this->max->right) {
                this->max = this->max->right;
            }
        }
    }


    // rotations
    // **** adjust height of changed nodes.
    void rotateLL(shared_ptr<TreeNode> node) {
        shared_ptr<TreeNode> leftSon = node->left;

        leftSon->father = node->father;
        if (node->father == nullptr) {
            this->root = leftSon;
        } else if (node->father->left == node){
            node->father->left = leftSon;
        } else {
            node->father->right = leftSon;
        }

        node->left = leftSon->right;
        if (node->left != nullptr) {
            node->left->father = node;
        }

        leftSon->right = node;
        node->father = leftSon;

        node->height = maxInt(getHeight(node->left), getHeight(node->right)) + 1;
    }
    void rotateRR(shared_ptr<TreeNode> node) {
        shared_ptr<TreeNode> rightSon = node->right;

        rightSon->father = node->father;
        if (node->father == nullptr) {
            this->root = rightSon;
        } else if (node->father->left == node){
            node->father->left = rightSon;
        } else {
            node->father->right = rightSon;
        }

        node->right = rightSon->left;
        if (node->right != nullptr) {
            node->right->father = node;
        }

        rightSon->left = node;
        node->father = rightSon;

        node->height = maxInt(getHeight(node->left), getHeight(node->right)) + 1;
    }
    void rotateLR(shared_ptr<TreeNode> node) {
        rotateRR(node->left);
        rotateLL(node);
    }
    void rotateRL(shared_ptr<TreeNode> node) {
        rotateLL(node->right);
        rotateRR(node);
    }


    // aux
    int maxInt(int x1, int x2) {
        return x1 > x2 ? x1 : x2;
    }
    shared_ptr<TreeNode> findFirstBiggerThanAux(shared_ptr<T> data){
        if(predicate(max->data , data)){
            return nullptr;
        }
        shared_ptr<TreeNode> node = findLastNodeInSearch(root , nullptr , data);
//        if(node->data == data){
//            return node;
//        }
        if(!predicate(node->data , data)){
            return node;
        }
        while(node->father && node->father.lock()->right == node){
            node = node->father.lock();
        }
        return node->father.lock();
    }

public:

    class AVLIter;
    // exceptions
    class AlreadyExists : public std::exception {};
    class DoesntExist : public std::exception {};

    // throws exception AlreadyExists if already exists
    void insert(shared_ptr<T> data) {
        if (!root) {
            root = initNode(data);
        } else {
            if (!findNode(root, data)) {
                throw AlreadyExists();
            }
            insertNode(initNode(data), root, nullptr);
        }
        ++size;
        fixMax();
    }
    // returns nullptr if not found
    shared_ptr<T> find(shared_ptr<T> data) const {
        shared_ptr<TreeNode> toFind = findNode(root, data);
        return !toFind ? nullptr : toFind->data;
    }
    // throws DoesntExist
    void remove(shared_ptr<T> data) {
        shared_ptr<TreeNode> toRemove = findNode(root, data);
        if (!toRemove) {
            throw DoesntExist();
        }
        removeNode(toRemove);
        size--;
        fixMax();
    }
    shared_ptr<T> getMax() const {
        return this->max->data;
    }
    int getSize() const {
        return this->size;
    }
    bool isEmpty() const {
        return this->size == 0;
    }

    shared_ptr<TreeNode> findLastNodeInSearch(shared_ptr<TreeNode> treeNode , shared_ptr<TreeNode> father , shared_ptr<T> toFind){
        if(treeNode == nullptr){
            return father;
        }

        if (treeNode->data == toFind) {
            return treeNode;
        }

        if (predicate(toFind, treeNode->data)) {
            return findLastNodeInSearch(treeNode->left,treeNode ,  toFind);
        }
        return findLastNodeInSearch(treeNode->right, treeNode ,toFind);
    }

    AVLTree::AVLIter findFirstBiggerThan(shared_ptr<T> data){
        return AVLIter(findFirstBiggerThanAux(data) , max);
    }


    // iterator and merge
    class AVLIter {
        shared_ptr<TreeNode> current;
        shared_ptr<TreeNode> finish;
    public:
        AVLIter(shared_ptr<TreeNode> current, shared_ptr<TreeNode> finish) : current(current), finish(finish) {}
        AVLIter() {
            current = nullptr;
            finish = nullptr;
        }

        bool operator==(const AVLIter& iter) {
            return this->current == iter.current;
        }
        bool operator!=(const AVLIter& iter) {
            return this->current != iter.current;
        }
        AVLIter& operator++() {
            if (current == finish) {
                current = nullptr;
            } else {
                if (current->right) {
                    current = current->right;
                    while (current->left) {
                        current = current->left;
                    }
                } else if (!current->father.lock()) {
                    current = nullptr;
                } else if (current->father.lock()->left == current) {
                        current = current->father.lock();
                    } else {
                        while (current->father.lock()->right == current) {
                            current = current->father.lock();
                        }
                    }
                }
            return *this;
        }

        shared_ptr<T> operator*(){
            return current->data;
        }

        friend class AVLTree<T, Pred>;
    };

    AVLIter begin() {
        return AVLIter(this->min, this->max);
    }
    AVLIter end(){
        return AVLIter(nullptr , nullptr);
    }
};




#endif //WET1_AVLTREE_H
