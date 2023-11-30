#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>

using namespace std;

ifstream in("in.txt");
ofstream out("out.txt");

struct TreeNode {
    int height;
    int subtreeSum;
    TreeNode *left;
    TreeNode *right;
    int value;

    TreeNode(int key) : height(0), subtreeSum(0), left(nullptr), right(nullptr), value(key) {}
};

struct BST {
    int maximum;
    bool flag = false;
    TreeNode *root;

    BST() : root(nullptr) {}

    void insert(int key) {
        if (root == nullptr) {
            root = new TreeNode(key);
            return;
        }

        TreeNode *tmp = root;
        TreeNode *previous = nullptr;
        while (tmp) {
            previous = tmp;
            if (key < tmp->value) {
                tmp = tmp->left;
            } else if (key > tmp->value) {
                tmp = tmp->right;
            } else {
                // Key already exists in tree
                return;
            }
        }

        if (key < previous->value) {
            previous->left = new TreeNode(key);
        } else {
            previous->right = new TreeNode(key);
        }
    }

    void subtreeHeight(TreeNode *root) {
        int leftH;
        int rightH;

        if (root->right) {
            subtreeHeight(root->right);
            rightH = root->right->height;
        } else rightH = 0;

        if (root->left != nullptr) {
            subtreeHeight(root->left);
            leftH = root->left->height;
        } else leftH = 0;

        root->height = (std::max(rightH, leftH) + 1);
        root->subtreeSum = rightH + leftH;
        if (root->subtreeSum > maximum) {
            maximum = root->subtreeSum;
        }
    }

    TreeNode *deleteKey(TreeNode *root, int key) {
        if (root == nullptr) {
            return root;
        }

        if (key < root->value) {
            root->left = deleteKey(root->left, key);
        } else if (key > root->value) {
            root->right = deleteKey(root->right, key);
        } else {
            if (root->left == nullptr) {
                TreeNode *temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                TreeNode *temp = root->left;
                delete root;
                return temp;
            }

            TreeNode *temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteKey(root->right, temp->value);
        }

        return root;
    }

    TreeNode *findMin(TreeNode *node) {
        TreeNode *current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void searchRoot(TreeNode *root, TreeNode *previous) {
        if (root->left != nullptr) {
            searchRoot(root->left, root);
        }
        if (root->right != nullptr) {
            searchRoot(root->right, root);
        }

        if (root->subtreeSum == maximum) {
            if (!flag) {
                flag = true;
                cout << endl << root->value << " IS ROOT" << endl;
                deleteRoot_right(root, previous);
            }
        }
    }

    void deleteRoot_right(TreeNode *root, TreeNode *previous) {
        if (root == this->root) {
            this->root = deleteKey(root, root->value);
        } else if (previous->right == root) {
            previous->right = deleteKey(root, root->value);
        } else {
            previous->left = deleteKey(root, root->value);
        }
    }

    void preOrderTraversal(TreeNode *root) {
        if (root != nullptr) {
            out << root->value << "\n";
            preOrderTraversal(root->left);
            preOrderTraversal(root->right);
        }
    }
};

int main() {
    BST tree;
    int key;
    while (in >> key) {
        tree.insert(key);
    }

    cout << "\nHeights of each root:" << endl;
    tree.subtreeHeight(tree.root);
    function<void(TreeNode *)> printHeights = [&](TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        cout << "TreeNode " << root->value << " has height: " << root->height << endl;
        printHeights(root->left);
        printHeights(root->right);
    };

    printHeights(tree.root);

    tree.searchRoot(tree.root, nullptr); // Search for and delete the root of the semi-path
    tree.preOrderTraversal(tree.root);

    return 0;
}