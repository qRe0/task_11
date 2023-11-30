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
        if (root->left == nullptr && root->right == nullptr && !flag) {
            cout << "Node with maximum subtree sum not found" << endl;
        }
    }

    void deleteRoot_right(TreeNode *root, TreeNode *previous) {
        if (root) {
            // Если у удаляемого узла нет левого потомка, то заменяем корень на его правого потомка
            if (root->left == nullptr) {
                root = root->right;
            }
            // Если у удаляемого узла нет правого потомка, то заменяем корень на его левого потомка
            if (root->right == nullptr) {
                root = root->left;
            }
        }

        // Если у удаляемого узла нет левого потомка и есть родительский узел, обновляем ссылку родителя на правого потомка
        if (root->left == nullptr && previous != nullptr) {
            if (previous->left == root) {
                previous->left = root->right;
            }
            if (previous->right == root) {
                previous->right = root->right;
            }
        }

        // Если у удаляемого узла нет правого потомка и есть родительский узел, обновляем ссылку родителя на левого потомка
        if (root->right == nullptr && previous != nullptr) {
            if (previous->left == root) {
                previous->left = root->left;
            }
            if (previous->right == root) {
                previous->right = root->left;
            }
        }

        // Если у удаляемого узла есть и левый, и правый потомок
        if ((root->right != nullptr) && (root->left != nullptr)) {
            TreeNode *tempNode = root->right;
            TreeNode *tempPred = root;

            // Находим самый левый узел в правом поддереве (наименьший элемент в правом поддереве)
            while (true) {
                if (tempNode->left != nullptr) {
                    tempPred = tempNode;
                    tempNode = tempNode->left;
                } else {
                    break;
                }
            }

            // Заменяем значение удаляемого узла на значение найденного узла
            root->value = tempNode->value;

            // Обновляем ссылку родительского узла на правого потомка найденного узла
            if (tempNode->right != nullptr) {
                if (tempPred != root) {
                    tempPred->left = tempNode->right;
                } else {
                    tempPred->right = tempNode->right;
                }
            } else {
                // Если у найденного узла нет правого потомка, убираем ссылку на него у его родителя
                if (tempPred != root) {
                    tempPred->left = nullptr;
                } else {
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

    tree.searchRoot(tree.root, nullptr); // Поиск и удаление корня полупути
    tree.preOrderTravesal(tree.root);

    return 0;
}
