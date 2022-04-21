

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H

#include <iostream>
#include <math.h>
template<class T, class Pred>
class AVLTree {
    struct TreeNode {
        shared_ptr<TreeNode> right;
        shared_ptr<TreeNode> left;
        shared_ptr<TreeNode> father;
        shared_ptr<T> data;
        int height;
    };

    Pred predicate;
    shared_ptr<TreeNode> root;
    shared_ptr<TreeNode> max;
    AVLTree() =default;
    ~AVLTree() =default;


    // private functions
    shared_ptr<TreeNode> initNode(shared_ptr<T> data) {
        shared_ptr<TreeNode> node = shared_ptr<TreeNode>(new TreeNode());

        node->data = data;
        node->left = nullptr;
        node->right = nullptr;
        node->father = nullptr;
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
            return find(treeNode->left);
        }
        return find(treeNode->right);
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
    // change to void and make sure it doesn't exist..
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

        target->height = maxInt(getHeight(target->left), getHeight(target->right)) + 1;
        return target;
    }


    // rotations
    // **** adjust height of changed nodes.
    void rotateLL(shared_ptr<TreeNode> node) {
        shared_ptr<TreeNode> leftSon = node->left;

        leftSon->father = node->father;
        if (node->father == nullptr) {
            this->node = leftSon;
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
    }
    void rotateRR(shared_ptr<TreeNode> node) {
        shared_ptr<TreeNode> rightSon = node->right;

        rightSon->father = node->father;
        if (node->father == nullptr) {
            this->node = rightSon;
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
};




#endif //WET1_AVLTREE_H
