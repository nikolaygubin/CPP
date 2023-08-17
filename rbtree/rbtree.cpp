#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

#define RED 0
#define BLACK 1

struct rbtree
{
    int key;
    int value;
    int color;

    struct rbtree *left;
    struct rbtree *right;
    struct rbtree *parent;
    struct rbtree *root;

    rbtree()
    {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        root = nullptr;
        color = RED;
    }
};

struct rbtree *create_node(int key, int value);
struct rbtree *rbtree_add(struct rbtree *root, int key, int value);
void rbtree_add_fixup(rbtree *node, rbtree **T);
struct rbtree *rbtree_lookup(struct rbtree *root, int key);
struct rbtree *rbtree_delete(struct rbtree *root, int value);
struct rbtree *rbtree_min(struct rbtree *root);
struct rbtree *rbtree_max(struct rbtree *root);
void rbtree_free(struct rbtree *root);
void rbtree_print_dfs(struct rbtree *root, int level, vector<int> &a);
void left_rotation(rbtree *node, rbtree **T);
void right_rotation(rbtree *node, rbtree **T);
void sort_vector(vector<int> &array, int maxValue);

static int num = 0;

int main()
{
    srand(time(nullptr));

    rbtree *myTree = new rbtree;

    int numberNodes = 50000, maxNumber = 100000000;

    vector<int> a;
    for (int i = 0; i < 100; i++) {
        a.push_back(0);
    }

    vector<int> keys;
    for (int i = 0; i < numberNodes; i++) {
        int number = rand() % maxNumber;
        keys.push_back(number);
    }
    sort_vector(keys, maxNumber);

    for (int i = 0; i <= numberNodes; i++) {
        myTree = rbtree_add(myTree, keys[i], i);
    }

    cout << "Number of collisions = " << num << endl;
    cout << "Sum of inserts element = " << numberNodes - num << endl; 

    rbtree *node;
    node = rbtree_min(myTree);
    cout << "Minimal key = " << node->key << endl;

    node = rbtree_max(myTree);
    cout << "Maximal key = " << node->key << endl;

    int random = rand() % numberNodes;
    node = rbtree_lookup(myTree, keys[random]);
    if (node->key == keys[random]) {
        cout << "(key)" << keys[random] << "  ==  " << "(node->key)" << node->key << " : " << "Node found!" << endl;

    }

    rbtree_print_dfs(myTree, 0, a);

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] == 0) {
            break;
        }
        cout << "On " << i << " level " <<  a[i] << " elements" << endl;
    }

    rbtree_free(myTree);

    return 0;
}

struct rbtree *create_node(int key, int value, rbtree *root)
{
    rbtree *node = new rbtree;
    node->key = key;
    node->value = value;
    node->root = root;

    return node;
}

struct rbtree *rbtree_add(struct rbtree *root, int key, int value)
{
    if (root->root == nullptr) {
        root->key = key;
        root->value =  value;
        root->color = BLACK;
        root->root = root;
        return root;
    }

    rbtree *node = create_node(key, value, root);
    
    int left = 0;
    rbtree *parent, *tree = root;
    if (tree->parent != nullptr) {
        tree = tree->parent;
    }
    while (tree != nullptr) {
        parent = tree;
        if (key > tree->key) {
            tree = tree->right;
            left = 0;
        }
        else if (key < tree->key) {
            tree = tree->left;
            left = 1;
        }
        else {
            //cout << "Node already insert!\n";
            num++;
            return root;
        }
    }

    if (left == 0) {
        parent->right = node;
        node->parent = parent;
    }
    else {
        parent->left = node;
        node->parent = parent;
    }

    rbtree_add_fixup(node, &root);

    return root;
}

struct rbtree *rbtree_lookup(struct rbtree *root, int key)
{
    while (root != nullptr) {
        if (key > root->key) {
            root = root->right;
        }
        else if (key < root->key) {
            root = root->left;
        }
        else {
            return root;
        }
    }
    cout << "Didn't find node!\n";
    return nullptr;
}

struct rbtree *rbtree_min(struct rbtree *root)
{
    if (root->left != nullptr) {
        return rbtree_min(root->left);
    }
    return root;
}

struct rbtree *rbtree_max(struct rbtree *root)
{
    if (root->right != nullptr) {
        return rbtree_max(root->right);
    }
    return root;
}

void rbtree_free(struct rbtree *root)
{
    if (root->left != nullptr) {
        rbtree_free(root->left);
    }
    if (root->right != nullptr) {
        rbtree_free(root->right);
    }

    delete root;
}

void rbtree_print_dfs(struct rbtree *root, int level, vector<int> &a)
{
    if (root->left != nullptr) {
        rbtree_print_dfs(root->left, level + 1, a);
    }
    if (root->right != nullptr) {
        rbtree_print_dfs(root->right, level + 1, a);
    }
    
    a[level]++; 
}

void rbtree_add_fixup(rbtree *node, rbtree **T)
{
    while (node != *T) {
        if (node->parent->color != RED) {
            return;
        }
        if (node->parent->parent->left == node->parent) {
            rbtree *uncle = node->parent->parent->right;
            if (uncle != nullptr) {   // first case
                if (uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                    continue;
                }
            }
            if (node == node->parent->right){  // second case
                node = node->parent;
                left_rotation(node, T);
            }
            node->parent->color = BLACK; //  case 3
            node->parent->parent->color = RED;
            right_rotation(node->parent->parent, T);
        }
        else {
            rbtree *uncle = node->parent->parent->left; 
            if (uncle != nullptr) {   // first case
                if (uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                    continue;
                }
            }
            if (node == node->parent->left){  // second case
                node = node->parent;
                right_rotation(node, T);
            }
            node->parent->color = BLACK; //  case 3
            node->parent->parent->color = RED;
            left_rotation(node->parent->parent, T);
        }

    }
    (*T)->color = BLACK;
}

void left_rotation(rbtree *node, rbtree **T)
{
    //cout << "Left Rotation\n";
    rbtree *x = node, *y = node->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->left = x;
    y->parent = x->parent;
    if (x->parent != nullptr) {
        if (x == x->parent->left) {
            x->parent->left  = y;
        }
        else {
            x->parent->right = y;
        }
    }
    x->parent = y;
    if (y->parent == nullptr) {
        *T = y;
    }
}

void right_rotation(rbtree *node, rbtree **T)
{
    //cout << "Right Rotation\n";
    rbtree *x = node, *y = node->left;

    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    y->right = x;
    y->parent = x->parent;
    if (x->parent != nullptr) {
        if (x == x->parent->left) {
            x->parent->left  = y;
        }
        else {
            x->parent->right = y;
        }
    }
    x->parent = y;
    if (y->parent == nullptr) {
        *T = y;
    }    
}

void sort_vector(vector<int> &array, int maxValue)
{
    int *mas = new int[maxValue];

    for (int i = 0; i < maxValue; i++) {
        mas[i] = 0;
    }

    for (size_t i = 0; i < array.size(); i++) {
        mas[array[i]]++;
    }

    int k = 0;
    for (int i = 0; i < maxValue; i++) {
        while (mas[i] > 0) {
            array[k] = i;
            k++;
            mas[i]--;
        }
    }

    delete [] mas;
}