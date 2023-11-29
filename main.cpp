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

    int maximum;

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

    bool flag = false;

    void searchRoot(TreeNode *root, TreeNode *previous) {
        if (root->left != nullptr) {
            searchRoot(root->left, root);
        }

        if (root->subtreeSum == maximum) {
            if (!flag) {
                TreeNode *nodeRoot;
                nodeRoot = root;
                flag = true;
                cout << nodeRoot->value << " is Root" << endl;
                deleteRoot_right(nodeRoot, previous);
            }
        }
        if (root->right != nullptr) {
            searchRoot(root->right, root);
        }
    }

    void deleteRoot_right(TreeNode *root, TreeNode *previous) {
        if (root)
        {
            if (root->left == nullptr)
            {
                root = root->right;
            }
            if (root->right == nullptr)
            {
                root = root->left;
            }

        }

        if (root->left == nullptr && previous != nullptr)
        {
            if (previous->left == root)
            {
                previous->left = root->right;
            }
            if (previous->right == root)
            {
                previous->right = root->right;
            }
        }
        if (root->right == nullptr && previous != nullptr)
        {
            if (previous->left == root)
            {
                previous->left = root->left;
            }
            if (previous->right == root)
            {
                previous->right = root->left;
            }
        }


        if ((root->right != nullptr) && (root->left != nullptr))
        {
            TreeNode *tempNode = root->right;
            TreeNode *tempPred = root;
            while (true)
            {
                if (tempNode->left != nullptr)
                {
                    tempPred = tempNode;
                    tempNode = tempNode->left;
                }
                else break;
            }
            root->value = tempNode->value;
            if (tempNode->right != nullptr)
            {
                if (tempPred != root)
                {
                    tempPred->left = tempNode->right;
                }
                else
                {
                    tempPred->right = tempNode->right;
                }
            }
            else
            {
                if (tempPred != root)
                {
                    tempPred->left = nullptr;
                }
                else
                {
                    tempPred->right = nullptr;
                }
            }
        }
    }

    void preOrderTravesal(TreeNode *root) {
        if (root != nullptr) {
            out << root->value << "\n";
            preOrderTravesal(root->left);
            preOrderTravesal(root->right);
        }
    }

};

int main() {
    BST tree;
    int key;
    while (in >> key) {
        tree.insert(key);
    }

    cout << "rootHeights of each root:" << endl;
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

    cout << "\nDeleting root of subtree with maximum sum:" << endl;
    tree.searchRoot(tree.root, nullptr); // Поиск и удаление корня полупути
    tree.preOrderTravesal(tree.root);

    return 0;
}
