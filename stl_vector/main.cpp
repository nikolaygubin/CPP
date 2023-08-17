#include "vector.hpp"

template <typename T>
void print(stud_work::vector<T> &vector)
{
    for (size_t i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    stud_work::vector<int> int_vector;

    srand(time(nullptr));

    for (int i = 0; i < 18; i++) {
        int_vector.push_back(rand() % 1000);
    }
    print(int_vector);
    int_vector.sort();
    print(int_vector);
    std::cout << "Ёмкость вектора : " << int_vector.capacity() << std::endl;
    int_vector.shrink_to_fit();
    std::cout << "Ёмкость вектора : " << int_vector.capacity() << std::endl;

    stud_work::vector<char> char_vector;

    for (int i = 0; i < 30; i++) {
        char letter = 'A' + rand() % (('z' - 'A') * 2);
        if (std::isalpha(letter))
            char_vector.push_back(letter);
    }
    print(char_vector);
    char_vector.sort();
    print(char_vector);


    return 0;
}