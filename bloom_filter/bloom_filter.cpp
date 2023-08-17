#include "bloom_filter.h"

bloom_filter::bloom_filter(int size)
{
    this->size = 2 * size * num_hash_def / 8 + 1;
    bit_array = new uint8_t[this->size];
}

void bloom_filter::insert(string value)
{
    for (auto i = 0; i < num_hash_def; i++) {
        uint8_t byte = 0x1;
        int hash_code = hash_func(value, i), array_index = 0, place_in_byte = 0;
        array_index = hash_code / 8;
        place_in_byte = hash_code % 8;

        bit_array[array_index] = bit_array[array_index] | (byte << place_in_byte);

    }
}

bool bloom_filter::is_insert(string value)
{
    for (auto i = 0; i < num_hash_def; i++) {
        int hash_code = hash_func(value, i), array_index = 0, place_in_byte = 0;
        array_index = hash_code / 8;
        place_in_byte = hash_code % 8;

        if (((bit_array[array_index] >> place_in_byte) & 0x1) == 0)
            return false;   
    }

    return true;
}

int bloom_filter::hash_func(string value, int multip)
{
    long long int hash_code = 0;

    for (size_t i = 0; i < value.size(); i++) {
        hash_code += value[i] * pow(multip, i);
    }

    return hash_code % (size * 8);
}