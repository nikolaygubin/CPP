#ifndef BLOOM_FILTER_H

#define BLOOM_FILTER_H

#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

constexpr int num_hash_def = 20;

class bloom_filter
{
private:
    uint8_t *bit_array = nullptr;
    int size;

    int hash_func(string value, int multip);

public:
    bloom_filter(int size);
    void insert(string value);
    bool is_insert(string value);
};

#endif