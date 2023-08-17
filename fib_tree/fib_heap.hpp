#include <iostream>
#include <string>
#include <math.h>

using namespace std;

struct fib_tree
{
    fib_tree(int key, string value);

    int key;
    int degree = 1;
    string value;
    bool mark = false;

    fib_tree *next = nullptr;
    fib_tree *prev = nullptr;
    fib_tree *parent = nullptr;

    fib_tree *begin_childs = nullptr;
    fib_tree *end_childs = nullptr;
};

class fib_heap 
{
private:
    fib_tree *begin = nullptr;
    fib_tree *end = nullptr;
    fib_tree *min_key = nullptr;
    int size = 0;

    fib_tree* merge_fib_trees(fib_tree *a, fib_tree *b);
    void consolidate();

public:
    fib_tree* insert(int key, string value);
    int get_min();
    fib_heap* merge(fib_heap *a, fib_heap *b);
    string remove_min();
    void decrease_key(fib_tree *node, int key);

};