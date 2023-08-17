#include <iostream>
#include <vector>

using pairs = std::vector<std::pair<double, double>>;

void show_result(pairs result)
{
    for (auto pair : result)
        std::cout << "f(" << pair.first << ") = " << pair.second << std::endl;
}

int main()
{
    double step = 0.1, x0 = 1, y0 = 0;  // f(x0) = y0
    int count_step = 10;
    pairs coshi = {{y0, x0}};
    auto func = [](double x, double y) {return (1 + x) * y;};

    for (int st = 0; st < count_step; st++) {
        double last_x = coshi.back().first,
               last_y = coshi.back().second;
        coshi.push_back(std::make_pair(last_x + step, last_y + step * func(last_x, last_y))); // y(x0 + h) = y0 + h * f(x0, y0)
    }
    show_result(coshi);
}