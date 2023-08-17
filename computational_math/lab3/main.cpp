#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

int main()
{
    const double precision = 1e-5;
    double degree, lower_bound, upper_bound, curr_precision;
    int free_member;

    cout << "This app can solve equation like : <x^2 - 3 = 0>\nEnter degree of variable : ";
    cin >> degree;

    cout << "Enter free member(without variable) : ";
    cin >> free_member;

    cout << "Enter lower_bound : ";
    cin >> lower_bound;

    cout << "Enter upper_bound : ";
    cin >> upper_bound;

    auto lambda_func = [degree, free_member] (double value) { return pow(value, degree) + free_member; };

    auto lambda_derv1 = [degree] (double value) {return degree * pow(value, degree - 1); };

    // auto lambda_derv2 = [degree] (double value) {(degree == 2) ? return degree; : return degree * pow(value, degree - 2); };

    auto lambda_derv2 = [degree] (double value) {if (degree == 2) {return degree;} else {return degree * pow(value, degree - 2);} };

    double x_next = (lambda_func(lower_bound) * lambda_derv2(lower_bound) > 0) ? lower_bound : upper_bound;
    double x0; 

    do {
        x0 = x_next;
        x_next = x0 - lambda_func(x0) / lambda_derv1(x0);

    } while ((x0 - x_next > 0) ? x0 - x_next : x_next - x0 > precision);

    cout << "x = " << x_next << endl;
 
}

