#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void print_matrix(vector<vector<float>> matrix)
{
    for (vector<float> row : matrix) {
        for (float number : row) {
            cout << number << ' ';
        }
        cout << endl;
    }
}

vector<vector<float>> read_file(ifstream &file)
{
    vector<vector<float>> matrix;
    vector<float> number_row;
    char *number = new char[5], simbol;
    int index = 0;

    while (1) {
        file.get(simbol);
        if (file.eof()) {
            number[index] = '\0';
            number_row.push_back(atof(number));
            matrix.push_back(number_row);
            break;
        }
        if ((simbol >= '0' && simbol <= '9') || simbol == '-' || simbol == '.') {
            number[index++] = simbol;
        }
        else if (simbol == ' ') {
            number[index] = '\0';
            number_row.push_back(atof(number));
            delete number;
            number = new char[5];
            index = 0;
        }
        else if (simbol == '\r') {
            number[index] = '\0';
            number_row.push_back(atof(number));
            index = 0;
            delete number;
            number = new char[5];
            matrix.push_back(number_row);
            number_row.clear();
        }
    }

    return matrix;
}

float module(float num)
{
    if (num >= 0) return num;
    else return -num;
}

vector<float> get_solution(vector<vector<float>> &matrix, float degree)
{   
    // for (size_t i = 0; i < matrix.size(); i++) {
    //     float num1 = module(matrix[i][i]), num2 = 0;
    //     for (size_t j = 0; j < matrix.size(); j++) {
    //         if (j == i) continue;
    //         num2 += module(matrix[i][j]);
    //     }
    //     if (num1 < num2) {
    //         cout << "Надо двигать ряды или просто не решать!\n";
    //         exit(0);
    //     }
    // }

    vector<float> rezult(matrix.size(), 0), temp_rezult, rez_slau;

    for (size_t i = 0; i < matrix.size(); i++) {            // получаем еденицы на главной диагонали, затем обнуляем главную диагональ
        int number = matrix[i][i];
        for (size_t j = 0; j < matrix[i].size(); j++) {
            matrix[i][j] /= number;
        }
        matrix[i][i] = 0;
    }
    print_matrix(matrix);

    for (size_t i = 0; i < matrix.size(); i++) {           // в переменную rez_slau записываем результаты системы уравнений
        rez_slau.push_back(matrix[i][matrix.size()]);
    }

    int k = 0;
    float num;
    do {
        temp_rezult = rezult;                              // копируем вектор rezult в вектор temp_rezult

        for(size_t i = 0; i < matrix.size(); i++) {        // умножаем матрицу matrix на матрицу rezult и записываем в rezult разность rez_slau[i] - rezult[i]
            float number = 0;
            for (size_t j = 0; j < matrix.size(); j++) {
                number += matrix[i][j] * rezult[j];
            }
            rezult[i] = rez_slau[i] - number;
        }

        cout << k << " итерация\n";
        for (size_t i = 0; i < rezult.size(); i++) {
            cout << "x" << i << " = " << rezult[i] << endl;
        }
        cout << '\n';

        vector<float> diff;                               // вектор diff будет хранить разницу итераций i и i-1
        for (size_t i = 0; i < rezult.size(); i++) {
            diff.push_back(module(rezult[i] - temp_rezult[i]));
        }
        num = *max_element(diff.begin(), diff.end());     // находим максимальную разность по модулю и сравниваем её с переменной degree
        cout << "num = " << num << endl;
        k++;
    } while (num > degree);                               // если максимальная разность меньше degree, то выходим из цикла

    cout << "количество итераций k = " << k << endl;

    return rezult;
}

int main()
{
    ifstream file("matrix.txt");

    if (!file.is_open()) {
        cout << "Cannot open file!\n";
        exit(1);
    }

    vector<vector<float>> matrix = read_file(file);


    vector<float> rezult = get_solution(matrix, 1e-10);

    for (size_t i = 0; i < rezult.size(); i++) {
        cout << "x"  << i + 1 << " = " << rezult[i] << endl;
    }
}