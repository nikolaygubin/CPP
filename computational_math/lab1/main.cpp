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

vector<float> get_solution(vector<vector<float>> &matrix)
{   
    vector<float> rezult;

    for (size_t i = 0; i < matrix.size() - 1; i++) {                // Обнуляем все коэффициенты, стоящие снизу от главной диагонали.
        for (size_t j = i + 1; j < matrix.size(); j++) {            // Если на i ряду i столбце стоит 0, то спускаемся ниже, пока не найдём
            if (matrix[i][i] == float(0.0)) {                       // ряд, в котором на i месте стоит не 0 и меняем его с i рядом.
                size_t row = j;                                     // Если такой ряд не найден, значит ниже главной диагонали стоят нули и
                while (row < matrix.size()) {                       // мы должны рассматривать следующий столбец
                    if (matrix[row][i] != 0) {
                        break;
                    }
                    row++;
                    if (row == matrix.size()) {
                        row = -1;
                    }
                }
                if (row == -1) break;
                for (size_t k = 0; k < matrix[j].size(); k++) {     
                    float number = matrix[row][k];
                    matrix[row][k] = matrix[j - 1][k];
                    matrix[j - 1][k] = number;
                }
                j--;
                continue;
            }
            float number_to_multiply = -1 * (matrix[j][i] / matrix[i][i]);  // number_to_multiply - множитель на который надо умножить
            for (size_t k = 0; k < matrix[j].size(); k++) {                 // i ряд, чтобы при его сложении с j рядом получить 0 на j ряде i колонны.
                matrix[j][k] += matrix[i][k] * number_to_multiply;
            }
        }
    } 

    for (int row = matrix.size() - 1; row > 0; row--) {                 // Вычисляем значение переменной из последнего ряда,
        float rez = matrix[row][matrix.size()] / matrix[row][row];      // так как все коэффициенты этого ряда мы уже обнулили.
        for (int i = row - 1; i >= 0; i--) {                            // Далее поднимаемся вверх по матрице и переносим значение переменной 
            matrix[i][matrix.size()] -= matrix[i][row] * rez;           // умноженной на коэффициент в правую часть уравнения.
            matrix[i][row] = 0;                                         // Обнуляем переменную которую мы перенесли в правую часть.
        }
    } 

    print_matrix(matrix);
    cout << endl;
    for (size_t i = 0; i < matrix.size(); i++) {
        rezult.push_back(matrix[i][matrix[i].size() - 1] / matrix[i][i]); // В результате получим матрицу в которой все нули, кроме 
    }                                                                     // главной диагонали -> вычисляем все переменные.

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

    print_matrix(matrix);
    cout << endl;

    vector<float> rezult = get_solution(matrix);

    for (size_t i = 0; i < rezult.size(); i++) {
        cout << "x"  << i + 1 << " = " << rezult[i] << endl;
    }
}