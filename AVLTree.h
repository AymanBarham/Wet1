

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H

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


    shared_ptr<TreeNode> find(shared_ptr<TreeNode> treeNode, shared_ptr<T> toFind) const {
        if (treeNode == nullptr) {
            return nullptr;
        }
        if (treeNode->data == toFind) {
            return treeNode;
        }

        if (predicate(toFind,)
    }

};




#endif //WET1_AVLTREE_H
