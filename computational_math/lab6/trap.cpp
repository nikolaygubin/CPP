#include <iostream>
#include <math.h>


int main()
{
    constexpr double a = 0;
    constexpr double b = 5;
    auto func1_value = [] (double x) {return 7 / (pow(x, 2) + 1);};
    auto func2_value = [] (double x) {return (pow(x, 2) + 1);};
    auto func3_value = [] (double x) {return (x / (pow(x, 4) + 4));};
    int count = 10000;


    double distance_trap = (b - a) / count;
    double result = 0.0;

    for (int i = 1; i <= count; i++) {
        result += func2_value(distance_trap * i) + func2_value(distance_trap * (i - 1));
    }
    result *= distance_trap / 2;

    std::cout.precision(5);
    std::cout << "Result = " << result << std::endl;

    return 0;
}