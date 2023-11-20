#include <iostream>

using namespace std;

FILE *in = freopen("in.txt", "r", stdin);
FILE *out = freopen("out.txt", "w", stdout);

struct TreeNode {
    int value;
    int height;
    int subtreeSum;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : value(0), height(0), subtreeSum(0), left(nullptr), right(nullptr) {}

    TreeNode(int key) : value(key), height(0), subtreeSum(0), left(nullptr), right(nullptr) {}
};

struct BST {
    TreeNode *root;

    BST() : root(nullptr) {}

    void preOrderTraversal(TreeNode *node) {
        if (node != nullptr) {
            cout << node->value << endl;
            preOrderTraversal(node->left);
            preOrderTraversal(node->right);
        }
    }

    void insert(int key) {
        if (root == nullptr) {
            root = new TreeNode(key);
        }
        TreeNode *temp = root;
        TreeNode *prev = nullptr;
        int nodeComparison = 0;
        while (temp != nullptr) {
            prev = temp;
            nodeComparison = key - temp->value;
            if (nodeComparison > 0) {
                temp = temp->right;
            } else if (nodeComparison < 0) {
                temp = temp->left;
            } else if (nodeComparison == 0) {
                return;
            }
        }
        if (nodeComparison > 0) {
            prev->right = new TreeNode(key);
        } else if (nodeComparison < 0) {
            prev->left = new TreeNode(key);
        }
    }

    void heightAndSubtreeSum(TreeNode *node) {
        if (node->right != nullptr) {
            heightAndSubtreeSum(node->right);
            node->height = node->right->height;
        } else {
            node->height = 0;
        }
        if (node->left != nullptr) {
            heightAndSubtreeSum(node->left);
            node->subtreeSum = node->left->height;
        } else {
            node->subtreeSum = 0;
        }
        if (node->right != nullptr) {
            node->subtreeSum += node->right->height;
            node->height = max(node->height, node->right->height + 1);
        }
        node->subtreeSum += node->height;
    }

    void findRootOfMaxSubtreeSumPath() {
        int maximum = 0;
        TreeNode *maxRoot = nullptr;
        findRootOfMaxSubtreeSumPath(root, maximum, maxRoot);
        if (maxRoot != nullptr) {
            deleteRoot_right(maxRoot, nullptr, root);
        }
    }

    void findRootOfMaxSubtreeSumPath(TreeNode *node, int &maximum, TreeNode *&maxRoot) {
        if (node->left != nullptr) {
            findRootOfMaxSubtreeSumPath(node->left, maximum, maxRoot);
        }
        if (node->subtreeSum > maximum) {
            maximum = node->subtreeSum;
            maxRoot = node;
        }
        if (node->right != nullptr) {
            findRootOfMaxSubtreeSumPath(node->right, maximum, maxRoot);
        }
    }

    void deleteRoot_right(TreeNode *node, TreeNode *prev, TreeNode *&root) {
        if (node == root) {
            if (node->left == nullptr) {
                root = root->right;
            } else if (node->right == nullptr) {
                root = root->left;
            }
        }
        if (node->left == nullptr && prev != nullptr) {
            if (prev->left == node) {
                prev->left = node->right;
            } else if (prev->right == node) {
                prev->right = node->right;
            }
        }
        if (node->right == nullptr && prev != nullptr) {
            if (prev->left == node) {
                prev->left = node->left;
            } else if (prev->right == node) {
                prev->right = node->left;
            }
        }
        if (node->left != nullptr && node->right != nullptr) {
            TreeNode *tempNode = node->right;
            TreeNode *tempPrev = node;
            while (true) {
                if (tempNode->left != nullptr) {
                    tempPrev = tempNode;
                    tempNode = tempNode->left;
                } else {
                    break;
                }
            }
            node->value = tempNode->value;
            if (tempNode->right != nullptr) {
                if (tempPrev != node) {
                    tempPrev->left = tempNode->right;
                } else {
                    tempPrev->right = tempNode->right;
                }
            } else {
                if (tempPrev != node) {
                    tempPrev->left = nullptr;
                } else {
                    tempPrev->right = nullptr;
                }
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    BST tree;

    int key;
    while (cin >> key) {
        tree.insert(key);
    }

    tree.heightAndSubtreeSum(tree.root);
    tree.findRootOfMaxSubtreeSumPath();
    tree.preOrderTraversal(tree.root);

    return 0;
}
