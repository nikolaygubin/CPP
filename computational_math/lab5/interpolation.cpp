#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

double get_y(std::vector<std::pair<double, double>> func, double x) 
{
    double rezult = 0;

    std::vector<std::vector<double>> delta_y;
    std::vector<double> y;
    for (size_t i = 0; i < func.size(); i++) {
        y.push_back(func[i].second);
    }
    delta_y.push_back(y);

    int current_vector = 0;
    do {
        std::vector<double> cur_vec;
        for (size_t i = 1; i < delta_y[current_vector].size(); i++) {
            cur_vec.push_back(delta_y[current_vector][i] - delta_y[current_vector][i - 1]);
        }
        delta_y.push_back(cur_vec);
        current_vector++;

    } while (delta_y[current_vector].size() > 1);

    #if 0    // first method
        rezult += delta_y[0][0];
        int del = 1;
        double q = (x - func[0].first) / (func[1].first - func[0].first), current_q = q;
        for (size_t i = 1; i < delta_y.size(); i++) {
            rezult += delta_y[i][0] * current_q / del;
            del *= (i + 1);
            current_q *= (q - i);
        }
    #endif

    #if 1    // second method
        size_t max_index = delta_y.size() - 1;
        rezult += delta_y[0][max_index];
        int del = 1;
        double q = (x - func[max_index].first) / (func[1].first - func[0].first), current_q = q;
        for (int i = max_index - 1; i >= 0; i--) {
            rezult += delta_y[max_index - i][i] * current_q / del;
            del *= (max_index - i + 1);
            current_q *= ++q;
        }
    #endif

    return rezult;
}

int main()
{
    std::ifstream input_file("test.txt", std::ios::in);
    for (int k = 0; k < 4; k++) {
        std::string line_x, line_y, line_in, line_out, x, y;
        double in, out, rezult;
        std::getline(input_file, line_x);
        std::getline(input_file, line_y);
        std::getline(input_file, line_in);
        std::getline(input_file, line_out);

        std::stringstream ssx(line_x), ssy(line_y);
        ssx >> x, ssy >> y;
        std::vector<std::pair<double, double>> func;
        while (ssx >> x && ssy >> y) {
            func.push_back(std::make_pair(std::atof(x.c_str()), std::atof(y.c_str()) ));
        }

        line_in.erase(line_in.begin(), line_in.begin() + line_in.find(' '));
        line_out.erase(line_out.begin(), line_out.begin() + line_out.find(' '));

        in = std::atof(line_in.c_str()), out = std::atof(line_out.c_str()); 

        rezult = get_y(func, in);
        if (fabs(out - rezult) < 0.001) {
            std::cout << "\E[1;33m\E[44m ..........Succesfull!.........." << std::endl;
            std::cout << "EXP = " << rezult << "\t REAL = " << out << "\E[0m\n" << std::endl;
        }
        else  {
            std::cout << "\E[1;31m\E[41m .........Unsuccesfull!........." << std::endl;
            std::cout << "EXP = " << rezult << "    REAL = " << out << "\E[0m\n" << std::endl;
        } 

        func.push_back(std::make_pair(in, rezult));
        std::sort(func.begin(), func.end(), [] (std::pair<double, double> a, std::pair<double, double> b) { return (a.first < b.first); });

        std::ofstream output_file("./plots/rezult.txt", std::ios::out | std::ios::trunc);

        for (size_t i = 0; i < func.size(); i++) {
            std::string line = std::to_string(func[i].first) + ' ' + std::to_string(func[i].second) + '\n';
            output_file.write(line.c_str(), line.size());
        }
        output_file.close();

        char *str = new char[100];
        sprintf(str, "cd plots; gnuplot plot%d.plt; rm rezult.txt; cd ..", k + 1);
        system(str);
    }    

    input_file.close();

    return 0;
}