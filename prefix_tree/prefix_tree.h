#ifndef PREFIX_TREE_H

#define PREFIX_TREE_H

#include <iostream>
#include <string>
#include <string>

using namespace std;

struct node
{
    node() : letter_('\0') {}
    node(char letter) : letter_(letter) {}

    char letter_;
    int value_;
    node *last = nullptr;
    node **array = new node*[26];
    int size = 0;
};

class prefix_tree
{
private:
    node *root;
    int find_letter(node *nodes, char letter);
    void print_dfs(node *nnode, string *word, int level);

public:
    prefix_tree();
    void insert(string key, int value);
    int look_up(string key);
    void remove(string key);
    void print(int level);
};

#endif