#include "fib_heap.hpp"

//fib_tree____________________________________________________________________________________________________________________________________________________________

fib_tree::fib_tree(int key, string value) : key(key), value(value) { }

//fib_heap____________________________________________________________________________________________________________________________________________________________

fib_tree* fib_heap::insert(int key, string value)
{
    fib_tree *new_node = new fib_tree(key, value);
    if (begin == nullptr) {
        begin = new_node;
        end = new_node;
        min_key = new_node;
    }
    else {
        new_node->prev = min_key;
        new_node->next = min_key->next;

        min_key->next = new_node;
        if (new_node->next != nullptr)
            new_node->next->prev = new_node;

        if (min_key->key > new_node->key) {
            min_key = new_node;
        }
        if (new_node->next == nullptr)
            end = new_node;
    }
    size++;

    return new_node;
}

int fib_heap::get_min()
{
    if (begin == nullptr) return int();
    return min_key->key;
}

fib_heap* fib_heap::merge(fib_heap *a, fib_heap *b)
{
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    fib_heap *c = new fib_heap;

    c->begin = a->begin;
    c->end = b->end;

    c->end->next = c->begin;
    c->begin->prev = c->end;

    return c;
}

string fib_heap::remove_min()
{
    if (begin == nullptr) return string();
    string value = min_key->value;
    if (min_key->begin_childs == nullptr) {
        if (min_key == begin) {
            if (begin->next == nullptr) {
                begin = nullptr;
                delete min_key;
                return value;
            }
            else {
                begin = min_key->next;
                begin->prev = nullptr;
            }
        }
        else if (min_key == end) {
            end = min_key->prev;
            end->next = nullptr;
        }
        else {
            min_key->prev->next = min_key->next;
            min_key->next->prev = min_key->prev;
        }
    }
    else {
        if (min_key == begin) {
            if (begin->next == nullptr) {
                begin = min_key->begin_childs;
                end = min_key->end_childs;
            }
            else {
                begin = min_key->begin_childs;
                if (min_key->end_childs != nullptr) {
                    min_key->end_childs->next = min_key->next;
                    min_key->next->prev = min_key->end_childs;
                }
                else {
                    min_key->begin_childs->next = min_key->next;
                    min_key->next->prev = min_key->begin_childs;
                } 
            }
        }
        else if (min_key == end) {
            min_key->prev->next = min_key->begin_childs;
            min_key->begin_childs->prev = min_key->prev;
            if (min_key->end_childs != nullptr)
                end = min_key->end_childs;
            else 
                end = min_key->begin_childs;
        }
        else {
            min_key->prev->next = min_key->begin_childs;
            min_key->begin_childs->prev = min_key->prev;

            if (min_key->end_childs != nullptr) {
                min_key->end_childs->next = min_key->next;
                min_key->next->prev = min_key->end_childs;
            }
            else {
                min_key->begin_childs->next = min_key->next;
                min_key->next->prev = min_key->begin_childs;
            }
        }
    }
    delete min_key;

    fib_tree *min = begin;
    fib_tree *current = begin;
    while (current != nullptr) {
        if (current->key < min->key) min = current;
        current = current->next;
    }
    min_key = min;
    
    consolidate();

    size--;
    return value;
}

void fib_heap::decrease_key(fib_tree *node, int key)
{
    if (!begin) return;
    node->key = key;
    if (node->parent == nullptr) {
        if (min_key->key > key) {
            min_key = node;
        }
    }
    else {
        if (node->prev == nullptr && node->next == nullptr) {
            node->parent->begin_childs = nullptr;
        }
        else if (node->next == nullptr && node->prev != nullptr) {
            node->prev->next = nullptr;
            node->parent->end_childs = node->prev;
        }
        else if (node->prev == nullptr && node->next != nullptr) {
            node->parent->begin_childs = node->next;
            node->parent->begin_childs->prev = nullptr;
        }
        else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }


        node->prev = end;
        end->next = node;
        end = node;
        end->next = nullptr;

        if (min_key->key > key) {
            min_key = node;
        }

        if (!node->parent->mark)
            node->parent->mark = true;
        else {
            fib_tree *parent = node->parent;
            node->parent = nullptr;
            while (parent->parent != nullptr) {
                if (parent->prev == nullptr && parent->next == nullptr) {
                    parent->parent->begin_childs = nullptr;
                }
                else if (parent->next == nullptr && parent->prev != nullptr) {
                    parent->prev->next = nullptr;
                    parent->parent->end_childs = parent->prev;
                }
                else if (parent->prev == nullptr && parent->next != nullptr) {
                    parent->parent->begin_childs = parent->next;
                    parent->parent->begin_childs->prev = nullptr;
                }
                else {
                    parent->prev->next = parent->next;
                    parent->next->prev = parent->prev;
                }


                parent->prev = end;
                end->next = node;
                end = parent;
                end->next = nullptr;

                if (min_key->key > parent->key) {
                    min_key = parent;
                }

                if (!parent->parent->mark) {
                    parent->parent->mark = true;
                    parent->parent = nullptr;
                    break;
                }
                else {
                    parent->parent->mark = false;
                    fib_tree *last_parent = parent->parent;
                    parent->parent = nullptr;
                    parent = last_parent;
                }
            }
        }
        node->parent = nullptr;
    }
}

fib_tree* fib_heap::merge_fib_trees(fib_tree *a, fib_tree *b)
{
    if (a->key < b->key || a == min_key) {
        if (b == begin) {
            begin = b->next;
            begin->prev = nullptr;
        }
        else if (b == end) {
            end = b->prev;
            end->next = nullptr;
        }
        else {
            b->prev->next = b->next;
            b->next->prev = b->prev;
        }

        if (a->begin_childs == nullptr) {
            b->prev = nullptr;
            b->next = nullptr;
            a->begin_childs = b;

            b->parent = a;
        }
        else {
            if (a->end_childs == nullptr) {
                b->prev = a->begin_childs;
                a->begin_childs->next = b;
                b->next = nullptr;
                a->end_childs = b;

                a->begin_childs->parent = a;
                a->end_childs->parent = a;
            }
            else {
                a->end_childs->next = b;
                b->prev = a->end_childs;
                a->end_childs = b;
                b->next = nullptr;

                fib_tree *beg = a->begin_childs;
                while (beg != nullptr) {
                    beg->parent = a;
                    beg = beg->next;
                }
            }
        }
        a->degree += b->degree;
        return a;
    }
    else {
        if (a == begin) {
            begin = a->next;
            begin->prev = nullptr;
        }
        else if (a == end) {
            end = a->prev;
            end->next = nullptr;
        }
        else {
            a->prev->next = a->next;
            a->next->prev = a->prev;
        }

        if (b->begin_childs == nullptr) {
            a->prev = nullptr;
            a->next = nullptr;
            b->begin_childs = a;

            a->parent = b;
        }
        else {
            if (b->end_childs == nullptr) {
                a->prev = b->begin_childs;
                b->begin_childs->next = a;
                a->next = nullptr;
                b->end_childs = a;

                b->begin_childs->parent = b;
                b->end_childs->parent = b;
            }
            else {
                b->end_childs->next = a;
                a->prev = b->end_childs;
                b->end_childs = a;
                a->next = nullptr;

                fib_tree *beg = b->begin_childs;
                while (beg != nullptr) {
                    beg->parent = b;
                    beg = beg->next;
                }
            }
        }
        b->degree += a->degree;
        return b; 
    }
}

void fib_heap::consolidate()
{
    int max_rank = log2(size) + 1;
    fib_tree **nodes = new fib_tree*[max_rank];
    for (int i = 0; i < max_rank; i++) {
        nodes[i] = nullptr;
    }

    fib_tree *beg = begin;
    while (beg != nullptr) {
        int rank = log2(beg->degree);
        if (nodes[rank] == nullptr) 
            nodes[rank] = beg;
        else {
            if (beg == nodes[rank]) {
                beg = beg->next;
                continue;
            }
            fib_tree *check = merge_fib_trees(beg, nodes[rank]);
            if (check == nodes[rank]) {
                beg = nodes[rank];
            }
            nodes[rank] = nullptr;  
            continue;
        }
        beg = beg->next;
    }
}