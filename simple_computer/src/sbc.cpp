#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string.h>
#include <array>

// транслируем строку симпл бейсика в 1 и более строк симпл ассемблера
std::string get_asm_line(std::string line, std::unordered_map<int, int> &goto_map, int &line_counter, std::unordered_map<char, int> &variables) 
{
    int number_line = 0;
    std::string command, expression, asm_line;
    std::stringstream ss(line);

    ss >> number_line >> command;
    std::string part_expr;
    while (ss >> part_expr) expression += part_expr + ' ';    

    if (!command.compare("REM")) {
        return std::string();
    }
    else if (!command.compare("INPUT")) {
        if (islower(expression[0])) { // все переменные должны быть большими буквами
            std::cout << "Error : incorrect VAR at " << number_line << " line !" << std::endl; 
            exit(0);
        }   
        auto it = variables.find(expression[0]); // ищем есть ли уже эта переменная
        if (it == variables.end()) {          
            variables.insert(std::make_pair(expression[0], 99 - variables.size())); // если нет записываем её с конца памяти, то есть все переменные будут в памяти : 99, 98, 97...
            it = variables.find(expression[0]); 
        }         
        char str[20];
        sprintf(str, "%02d READ %02d\n", line_counter, it->second); // записываем в строку номер строки, команду и операнд и возвращаем string(операнд в данном случае это ячейка в которой лежит переменная)
        asm_line += str;
        line_counter++;
    }
    else if (!command.compare("PRINT")) {
        if (islower(expression[0])) {
            std::cout << "Error : incorrect VAR at " << number_line << " line !" << std::endl; 
            exit(0);
        } 
        auto it = variables.find(expression[0]);
        if (it == variables.end()) {
            std::cout << "Error : undefined VAR at " << number_line << " line !" << std::endl; 
            exit(0);            
        }    
        char str[20];
        sprintf(str, "%02d WRITE %02d\n", line_counter++, it->second);
        asm_line += str;
    }
    else if (!command.compare("GOTO")) {
        goto_map.insert(std::make_pair(number_line / 10 - 1, atoi(expression.c_str()) / 10 - 1)); // в наш словарь заносим место откуда и куда нужно перейти
        char str[10];
        sprintf(str, "%02d JUMP ", line_counter++); // оставляем операнд пустым -> дополним его после чтения всех команд
        asm_line += str;
    }
    else if (!command.compare("IF")) {
        std::stringstream ss(expression);
        std::string operand, var1, var2, goto_pos, command_var1, command_var2;;
        ss >> var1 >> operand >> var2 >> goto_pos >> goto_pos; // expression будет иметь вид IF C < 0 GOTO 20, поэтому 2 раза записываем в goto_pos из стринг потока
        if (!operand.compare("<") || !operand.compare(">") || !operand.compare("==")) {
            if (!operand.compare("<")) { // меньше
                if (isdigit(var1 [0])) { // если левый операнд число
                    char str[50];
                    int number = atoi(var1.c_str());
                    sprintf(str, "%02d = %c%04X\n%02d LOAD %02d\n", line_counter, (number >= 0) ? '+' : '-', number, line_counter + 1, line_counter);
                    asm_line += str;
                    line_counter += 2;
                }
                else { // если первый операнд переменая 
                    if (variables.find(var1[0]) == variables.end()) { // ищем в словаре переменных
                        std::cout << "Error : undefined VAR at " << number_line << " line !" << std::endl; 
                        exit(0);                  
                    }
                    char str[20];
                    sprintf(str, "%02d LOAD %02d\n", line_counter++, variables[var1[0]]);
                    asm_line += str;
                }

                if (isdigit(var2[0])) { // аналогично со 2 переменной
                    char str[50];
                    int number = atoi(var2.c_str());
                    sprintf(str, "%02d = %c%04X\n%02d SUB %02d\n", line_counter, (number >= 0) ? '+' : '-', number, line_counter + 1, line_counter);
                    asm_line += str;
                    line_counter += 2;
                }
                else {
                    if (variables.find(var2[0]) == variables.end()) {
                        std::cout << "Error : undefined VAR at " << number_line << " line !" << std::endl; 
                        exit(0);                  
                    }
                    char str[20];
                    sprintf(str, "%02d SUB %02d\n", line_counter++, variables[var2[0]]);
                    asm_line += str;
                }
            }
            else { // если знак больше или равно
                if (isdigit(var2[0])) {
                    char str[50];
                    int number = atoi(var2.c_str());
                    sprintf(str, "%02d = %c%04X\n%02d LOAD %02d\n", line_counter, (number >= 0) ? '+' : '-', number, line_counter + 1, line_counter);
                    asm_line += str;
                    line_counter += 2;
                }
                else {
                    if (variables.find(var2[0]) == variables.end()) {
                        std::cout << "Error : undefined VAR at " << number_line << " line !" << std::endl; 
                        exit(0);                  
                    }
                    char str[20];
                    sprintf(str, "%02d LOAD %02d\n", line_counter++, variables[var2[0]]);
                    asm_line += str;
                }

                if (isdigit(var1[0])) {
                    char str[50];
                    int number = atoi(var1.c_str());
                    sprintf(str, "%02d = %c%04X\n%02d SUB %02d\n", line_counter, (number >= 0) ? '+' : '-', number, line_counter + 1, line_counter);
                    asm_line += str;
                    line_counter += 2;
                }
                else {
                    if (variables.find(var1[0]) == variables.end()) {
                        std::cout << "Error : undefined VAR at " << number_line << " line !" << std::endl; 
                        exit(0);                  
                    }
                    char str[20];
                    sprintf(str, "%02d SUB %02d\n", line_counter++, variables[var1[0]]);
                    asm_line += str;
                }
            }

            char str[20];
            goto_map.insert(std::make_pair(number_line / 10 - 1, atoi(goto_pos.c_str()) / 10 - 1)); // аналогично с оператором goto
            if (!operand.compare("=="))
                sprintf(str, "%02d JZ ", line_counter++); // если было равно то выполняеся команда, которая проверки аккумулятора на ноль
            else
                sprintf(str, "%02d JNEG ", line_counter++); // иначе проверяется аккумулятор на то, отрицательный он или нет, также оставляем место для условного перехода
            asm_line += str;
        }
    }    
    else if (!command.compare("LET")) { // функция присваения let. Пример : LET C = A + 67 - B * 12
        std::array<char, 5> var_array = {'V','W','X','Y','Z'}; // массив, который будет хранить переменные для замены выражений. Пример: было x + y * 5 -> x + v, так как v первая переменная в этом массиве
        auto it_array = var_array.begin();

        char var = expression[0];
        if (variables.find(var) == variables.end()) { // смотрим есть ли эта переменная в словаре
            if (islower(var)) {
                std::cout << "Error : incorrect VAR at " << number_line << " line !" << std::endl; 
                exit(0);
            }   
            variables.insert(std::make_pair(var, 99 - variables.size())); // добавляем в словарь если её нету
        }

        std::string math_expr;
        for (size_t i = 4; i < expression.size(); i++) { // начинаем с 4 индекса, чтобы пропустить LET C = . В переменной math_expr будет лежать математическое выражение без пробелов
            if (expression[i] != ' ') math_expr += expression[i];
        }

        while (1) { // в бесконечном цикле будем вычислять промежуточные результаты, выйдем, когда в math_expr будет содержать 1 символ выйдем из него
            char sign; // символ в котором будет лежать текущий знак
            std::string::iterator it_sign;
            if ((it_sign = std::find(math_expr.begin(), math_expr.end(), '*')) != math_expr.end()) { // cоблюдая правила математики ищем какое выражение будет выполняться первым
                sign = '*';
            }
            else if ((it_sign = std::find(math_expr.begin(), math_expr.end(), '/')) != math_expr.end()) {
                sign = '/';
            }            
            else if ((it_sign = std::find(math_expr.begin(), math_expr.end(), '+')) != math_expr.end()) {
                sign = '+';
            }
            else if ((it_sign = std::find(math_expr.begin(), math_expr.end(), '-')) != math_expr.end()) {
                sign = '-';
            }
            else {  // если ничего не совпало значит перед нами 1 число
                char str[50];
                int number = atoi(math_expr.c_str());
                sprintf(str, "%02d = %c%04x\n%02d LOAD %02d\n%02d STORE %02d\n", line_counter, (number >= 0) ? '+' : '-', number, line_counter + 1, line_counter, line_counter + 2, variables[var]);
                asm_line += str;
                line_counter += 2;
                break;
            }

            auto it_left_operand = --it_sign; // итератор на левый операнд выражения. ПРИМЕР ВЫРАЖЕНИЯ : 35 - A
            std::advance(it_sign, 2); // сдвигаем итератор на 2
            auto it_right_operand = it_sign; // итератор на правый операнд выражения
            it_sign--;

            if (isalpha(*it_left_operand)) {  // если левый операнд переменная
                auto it_letter = variables.find(*it_left_operand);
                if (it_letter == variables.end()) {
                    std::cout << "Error : undefined VAR at " << number_line << " line !" << std::endl; 
                    exit(0);             
                }
                char str[20];
                sprintf(str, "%02d LOAD %02d\n", line_counter++, it_letter->second);
                asm_line += str;
            }
            else { // если левый операнд число
                while (isdigit(*it_left_operand) && it_left_operand != math_expr.begin()) { // сдвигаемся слево пока перед нами число или не достигли начала всего выражения
                    it_left_operand--;
                }
                if (it_left_operand != math_expr.begin()) it_left_operand++; // если не начало сдвигаемся на вправо
                char str[40];
                int number = atoi(std::string(it_left_operand, it_sign).c_str());
                sprintf(str, "%02d = %c%04x\n%02d LOAD %02d\n", line_counter, (number < 0) ? '-' : '+', number, line_counter + 1, line_counter);
                line_counter += 2;
                asm_line += str; 
            }

            if (isalpha(*it_right_operand)) {  // аналогичные действия выполняем с правым операндом выражения
                auto it_letter = variables.find(*it_right_operand);
                if (it_letter == variables.end()) {
                    std::cout << "Error : undefined VAR at " << number_line << " line !" << std::endl; 
                    exit(0);             
                }
                it_right_operand++;
                char str[20];
                if(sign == '+') { // выполняем арифметические действия с аккумулятором в зависимости от считанного знака 
                    sprintf(str, "%02d ADD %02d\n", line_counter++, it_letter->second);
                }
                else if (sign == '-') {
                    sprintf(str, "%02d SUB %02d\n", line_counter++, it_letter->second);
                }
                else if (sign == '*') {
                    sprintf(str, "%02d MUL %02d\n", line_counter++, it_letter->second);
                }                
                else if (sign == '/') {
                    sprintf(str, "%02d DIVIDE %02d\n", line_counter++, it_letter->second);
                }                
                asm_line += str;                  
            }      
            else {
                while (isdigit(*it_right_operand) && it_right_operand != math_expr.end()) {
                    it_right_operand++;
                }
                int number = atoi(std::string(++it_sign, it_right_operand).c_str());
                char str[40];
                if(sign == '+') {
                    sprintf(str, "%02d = %c%04x\n%02d ADD %02d\n",line_counter, (number < 0) ? '-' : '+', number, line_counter + 1, line_counter);
                }
                else if (sign == '-') {
                    sprintf(str, "%02d = %c%04x\n%02d SUB %02d\n",line_counter, (number < 0) ? '-' : '+', number, line_counter + 1, line_counter);
                }
                else if (sign == '*') {
                    sprintf(str, "%02d = %c%04x\n%02d MUL %02d\n",line_counter, (number < 0) ? '-' : '+', number, line_counter + 1, line_counter);
                }                
                else if (sign == '/') {
                    sprintf(str, "%02d = %c%04x\n%02d DIVIDE %02d\n",line_counter, (number < 0) ? '-' : '+', number, line_counter + 1, line_counter);
                }                
                line_counter += 2;
                asm_line += str;    
            }
                   
            variables.insert(std::make_pair(*it_array, 99 - variables.size())); // добавляем к словарю переменную из array по итератору, который мы получили в начале
            char str[20];
            std::string letter(1, *it_array);
            math_expr.replace(it_left_operand, it_right_operand, letter.c_str()); // заменяем в общем выражении вычисленную операцию на переменную из array
            if (math_expr.size() == 1) break; // если в выражении осталась одна переменная выходим
            sprintf(str, "%02d STORE %02d\n", line_counter++, variables[*it_array]);
            asm_line += str;
            it_array++; // сдвигаем итератор на следующую переменную
            if (it_array == var_array.end()) { // если переменные закончились
                std::cout << "Error : temp varibales(" << var_array.size() << ") run out!" << std::endl; 
                exit(0);   
            }
        }
        char str[20];
        sprintf(str, "%02d STORE %02d\n", line_counter++, variables[var]);
        asm_line += str;
    }    
    else if (!command.compare("END")) {
        char str[20];
        sprintf(str, "%02d HALT 00\n", line_counter++);
        asm_line += str;   
    }    

    return asm_line;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Usage : sbc prog.bc bin.o\n";
        exit(1);
    }

    std::ifstream bc_file(argv[1], std::ios::in); // файл с кодом на симпл бейсике
    if (!bc_file.is_open()) {
        std::cout << "Error : no such file or directory " << argv[1] << std::endl;
        exit(1);        
    }

    std::unordered_map<char, int> variables;  // словарь переменных
    std::unordered_map<int, int> goto_map;  // словарь переходов
    std::string line;
    std::vector<std::string> lines; // вектор из строк на симпл ассемблере
    std::vector<int> goto_vec; // вектор, в котором индекс числа - номер строки симпл бейсика, значение по индексу - номер строки симпл ассемблера
    int line_counter = 0; // переменная в которой лежит текущая строка на симпл ассемблере

    while (std::getline(bc_file, line)) { // последовательно считываем строки из файла
        goto_vec.push_back(line_counter);
        lines.push_back(get_asm_line(line, goto_map, line_counter, variables));
    }

    for (auto it = goto_map.begin(); it != goto_map.end(); it++) { // проходим по словарю переходов и дополняем строки смипл ассемблера
        char str[10];
        sprintf(str, "%02d\n", goto_vec[it->second]);
        lines[it->first] += str;
    }

    bc_file.close();    

    std::ofstream asm_file("program.sasm", std::ios::out); // открываем файл, в котором будет лежать программа на симпл ассемблере
    if (!asm_file.is_open()) {
        std::cout << "Error : cannot create asm file!" << std::endl;
        exit(1);          
    }

    for (std::string asm_line : lines) { // каждую строку из вектора записываем в файл симпл ассемблера
        if (asm_line.empty()) continue; // переходим к следующей итерации, если строка пустая
        asm_file << asm_line; // записываем в файл строку
    }
    asm_file.close();

    char str[50];
    sprintf(str, "./sat program.sasm %s", argv[2]);
    system(str); // транслируем из симпл ассемблера в бинарный файл

    return 0;
}