#include "prefix_tree.h"

prefix_tree::prefix_tree()
{
    root = new node;
}

void prefix_tree::insert(string key, int value)
{
    node *begin_node = root;

    for (size_t i = 0; i < key.size(); i++) {
        int index = find_letter(begin_node, key[i]);

        if (index == -1) {
            node *new_node = new node(key[i]);
            begin_node->array[begin_node->size++] = new_node;
            begin_node = new_node;
        }
        else {
            begin_node = begin_node->array[index];
        }
    }

    if (begin_node->last == nullptr) {
        node *last_node = new node('$');
        last_node->value_ = value;
        begin_node->last = last_node;
    }
    else {
        cout << key <<" already insert" << endl;
    }
}

int prefix_tree::look_up(string key)
{
    node *begin_node = root;

    for (size_t i = 0; i < key.size(); i++) {
        int index = find_letter(begin_node, key[i]);
        if (index != -1) {
            begin_node = begin_node->array[index];
        }
        else {
            cout << "Node didn't find" << endl;
            exit(0);
        }
    }
    if (begin_node->last != nullptr)
        return begin_node->last->value_;
    else {
        cout << "Node didn't find" << endl;
        exit(0);
    }
}   

void prefix_tree::remove(string key)
{
    node *begin_node = root;

    for (size_t i = 0; i < key.size(); i++) {
        int index = find_letter(begin_node, key[i]);
        if (index != -1) {
            begin_node = begin_node->array[index];
        }
        else {
            cout << "Node didn't insert or alredy removed" << endl;
            return;
        }
    }

    if (begin_node->last != nullptr) {
        delete begin_node->last;
        begin_node->last = nullptr;
    }
    else {
        cout << "Node didn't insert or alredy removed" << endl;
    } 
}

void prefix_tree::print(int level)
{
    string *word = new string;
    print_dfs(root, word, level);

    delete word;
}

void prefix_tree::print_dfs(node *nnode, string *word, int level)
{
    string *new_word = new string(*word);
    new_word->push_back(nnode->letter_);
    for (int i = 0; i < nnode->size; i++) {
        print_dfs(nnode->array[i], new_word, level + 1);
    }

    if (nnode->last != nullptr) {
        cout << "level = " << level << "    word = \"" << *new_word << '\"' << endl;
    }

    delete new_word;
}

int prefix_tree::find_letter(node *nodes, char letter)
{
    for (int i = 0; i < nodes->size; i++) {
        if (nodes->array[i]->letter_ == letter) {
            return i;
        }
    }

    return -1;
}