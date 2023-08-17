#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>


struct var
{
    var()
    {
        degree = 1;
        mult = 1;
    }

    void take_derivative()
    {
        if (name == "x") {
            if (degree == 0) {
                mult = 0;
            }
            else {
                mult *= degree--;
            }
        }
        else if (name == "c") {
            mult = 0;
        }
        else {    // e
            // nothing if x^1 && x > 0
        }
    }
    double get_value(double value)
    {
        if (name == "c") {
            return mult;
        }
        else if (!e_degree.empty()) {
            return pow (M_E, value);
        }
        return pow(value, degree) * mult;
    }

    std::string e_degree;
    std::string name;
    double degree;
    double mult;

};

void get_data(var &v, std::string line)
{
    size_t index = 0;
    if ((index = line.find('*')) == std::string::npos) {
        v.mult = 1;
        if (line.find('-') != std::string::npos) {
            v.mult *= -1;
        }
    }
    else 
        v.mult = std::atof(std::string(line.begin(), line.begin() + index).c_str());

    if ((index = line.find('^')) == std::string::npos) v.degree = 1;
    else {
        v.degree = line.at(index + 1) - '0';
    }

    if (line.find('e') != std::string::npos) v.e_degree += line, v.degree = 1;
    else if (line.find('x') != std::string::npos) v.name += 'x';
    else {
        v.mult = atof(line.c_str());
        v.name = 'c';
    }
}

int main()
{
    var var1, var2, var3, var4, var5, var6, var7, var8, var9;
    double precision, lower_bound, upper_bound, rezult;
    std::string line;
    std::fstream file("input.txt");

    std::getline(file, line);                                                                                       // IN: x^3 + 4 * x - 3 = 0
    line.erase(line.begin(), line.begin() + 4);
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    std::string::iterator current_begin = line.begin(), find_place;
    if ((find_place = std::find(current_begin, line.end(), '+')) == line.end())
        find_place = std::find(current_begin, line.end(), '-');
    get_data(var1, std::string(current_begin, find_place));
    current_begin = ++find_place;
    if ((find_place = std::find(current_begin, line.end(), '+')) == line.end())
        find_place = std::find(current_begin, line.end(), '-');   
    get_data(var2, std::string(--current_begin, find_place));
    current_begin = find_place;
    get_data(var3, std::string(current_begin, std::find(current_begin, line.end(), '=')));

    std::getline(file, line);                                                                                       //     eps = 0.001
    auto it_begin = ++std::find(line.begin(), line.end(), '=');
    auto it_end = line.end();
    precision = std::atof(std::string(it_begin, it_end).c_str());

    std::getline(file, line);                                                                                       //     [0; 1]
    it_begin = ++std::find(line.begin(), line.end(), '['), it_end = std::find(line.begin(), line.end(), ';');
    lower_bound = std::atof(std::string(it_begin, it_end).c_str());
    
    it_begin = ++it_end, it_end = std::find(it_begin, line.end(), ']');
    upper_bound = std::atof(std::string(it_begin, it_end).c_str());

    std::getline(file, line);                                                                                       // OUT: 0.674
    it_begin = std::find(line.begin(), line.end(), ' '), it_end = line.end();
    rezult = std::atof(std::string(it_begin, it_end).c_str());

    var7 = var4 = var1, var8 = var5 = var2, var9 = var6 = var3;
    var4.take_derivative(), var5.take_derivative(), var6.take_derivative();
    var7.take_derivative(), var8.take_derivative(), var9.take_derivative();
    var7.take_derivative(), var8.take_derivative(), var9.take_derivative(); 

    double x_cur, x_last;
    if (((var1.get_value(lower_bound) + var2.get_value(lower_bound) + var3.get_value(lower_bound)) 
    * (var7.get_value(lower_bound) + var8.get_value(lower_bound) + var9.get_value(lower_bound)) > 0)) {
        x_cur = lower_bound;
    }
    else {
        x_cur = upper_bound;
    }

    double num;
    do {
        x_last = x_cur;
        num = ((var1.get_value(x_last) + var2.get_value(x_last) + var3.get_value(x_last))) /
        ((var4.get_value(x_last) + var5.get_value(x_last) + var6.get_value(x_last)));
        x_cur = x_last - num;
        //std::cout << x_cur << std::endl;
        
    } while (fabs(num) > precision);

    if (fabs(rezult - x_cur) < 0.01) {
        std::cout << "\E[1;33m\E[44m ..........Succesfull!.........." << std::endl;
        std::cout << "EXP = " << rezult << "\t REAL = " << x_cur << "\E[0m" << std::endl;
    }
    else  {
        std::cout << "\E[1;31m\E[41m .........Unsuccesfull!........." << std::endl;
        std::cout << "EXP = " << rezult << "    REAL = " << x_cur << "\E[0m" << std::endl;
    }
    return 0;
}