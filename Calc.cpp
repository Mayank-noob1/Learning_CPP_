/*
Grammar:
    BLOCK := EXPR ;
    EXPR := TERM + EXPR | TERM - EXPR | TERM 
    TERM := PREPRIMARY / TERM | PREPRIMARY * TERM  | PREPRIMARY % TERM| PREPRIMARY
    PREPRIMARY = PRIMARY ^ PREPRIMARY | PRIMARY !
    PRIMARY := (EXPR) | FUN PRIMARY | NUM | -PRIMARY | CONSTANT | +PRIMARY
    NUM := DIGIT NUM | DIGIT
    FUN := SIN | COS | TAN | COT | COSEC | SEC | SQRT | LOG
    DIGIT := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    CONSTANT := E | PI
*/

//  <------------------------------------------------------------------------------->
// Libraries used
#include <stdexcept>    //For throwing error
#include <vector>
#include <iostream>     
#include <string>       
#include <map>          

//  <------------------------------------------------------------------------------->
using namespace std;

// Function by Stroustoup.
inline void keep_window_open()
{
    cin.clear();
    cout << "Please enter a character to exit\n";
    char ch;
    cin >> ch;
    return;
}
inline void keep_window_open(string s)
{
    if (s == "")
        return;
    cin.clear();
    cin.ignore(120, '\n');
    for (;;)
    {
        cout << "Please enter " << s << " to exit\n";
        string ss;
        while (cin >> ss && ss != s)
            cout << "Please enter " << s << " to exit\n";
        return;
    }
}

//  <------------------------------------------------------------------------------->
/*
Function definition:
Tolerance limits on output. Not all outputs will be correct as taylor expansion used with limit on running sum.
*/
// x*y
double mul(double x, double y)
{
    return x * y;
};
// x+y
double add(double x, double y)
{
    // if (2 >> 32 - int(y) < int(x) + 1)
    // {
    //     throw std::invalid_argument("Input is out of bounds.");
    // }
    return x + y;
};
// x-y
double sub(double x, double y)
{
    // if (-(2 >> 32) + y < x)
    // {
    //     throw std::invalid_argument("Input is out of bounds.");
    // }
    return x - y;
};
// |x|
double abso(double n)
{
    if (double(0) < n)
    {
        return n;
    }
    return sub(double(0), n);
};
// x%y
double mod(double x, double y)
{
    if (y == double(0))
    {
        throw std::invalid_argument("Zero division error.");
    }
    return (int(x) % int(y));
};
// x/y
double div(double x, double y)
{
    if (y == double(0))
    {
        throw std::invalid_argument("Zero division error.");
    }
    return (x / y);
};
// 1/x
double inv(double x)
{
    if (x == double(0))
    {
        throw std::invalid_argument("Zero division error.");
    }
    return div(double(1), x);
}
// x^n , integer power
double pow(double x, int n)
{
    double ans = double(1);
    if (n > (0))
    {
        while (n > 0)
        {
            if (n & int(1))
            {
                ans = ans * x;
            }
            n /= 2;
            x *= x;
        }
    }
    else if (n < (0))
    {
        return inv(pow(x, abso(n)));
    }
    return ans;
};
// n!
double fact(int n)
{
    if (n < (0))
    {
        throw std::invalid_argument("Received negative value");
    }
    double ans = double(1);
    int i = 1;
    while (i <= n)
    {
        ans *= i;
        i++;
    }
    return ans;
};
// sin(rad)
double sin(double rad)
{
    int i = 0;
    double ans = double(0);
    int limit = int(100);
    while (i < limit)
    {
        ans += (pow(-1 * rad * rad, i) * (rad)) / fact(2 * i + 1);
        i++;
    }
    return ans;
};
// cos(rad)
double cos(double rad)
{
    int i = (0);
    double ans = (0);
    int limit = (100);
    while (i < limit)
    {
        ans += pow((-1 * rad * rad), i) / fact(2 * i);
        i++;
    }
    return ans;
};
// tan(rad)
double tan(double rad)
{
    return div(sin(rad), cos(rad));
};
// cot(rad)
double cot(double rad)
{
    return div(cos(rad), sin(rad));
};
// cosec(rad)
double cosec(double rad)
{
    return inv(sin(rad));
};
// sec(rad)
double sec(double rad)
{
    return inv(cos(rad));
};
// log(x)
double log(double x)
{
    if (x < -1)
    {
        throw std::invalid_argument("Input is out of bounds.");
    }
    x = (x-1)/(x+1);
    int i = (1);
    double ans = double(0);
    int limit = (1000000);
    while (i < limit)
    {
        ans += double(pow(x, i)) / double(i);
        i+=2;
    }
    return 2*ans;
};
// x^0.5
double sqrt(double x){
    double tolerance = inv(double(pow(double(10), int(10))));
    double low = (0);
    double high = (x);
    double mid = double(high + low) / double(2);
    if (x < (0))
    {
        throw std::invalid_argument("Received negative value.");
    }
    while (true)
    {
        double mid = double(high + low) / double(2);
        if (abso(sub(mid * mid, x)) <= tolerance)
        {
            return mid;
        }
        else if (mul(mid, mid) < x)
        {
            low = add(mid, 1);
        }
        else
        {
            high = sub(mid, 1);
        }
    }
    return mid;
}

//  <------------------------------------------------------------------------------->

// User defined constant identifiers. Eg: e,pi etc.
static map<string,double> lookup_table;
// Name of functions user defined functions. Eg: sin, cos etc.
static vector<string> func_name;

// Enumeration of states.
// Inspired from Stroustoup.
enum Token
{
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    FAC = '!',
    MOD = '%',
    POW = '^',
    LPAREN = '(',
    RPAREN = ')',
    PRINT = ';',
    CONSTANT,
    NUM,
    FUN,
    END,
};
// Gloabal values and states.
Token Crnt_token = PRINT;       //Current state
double number;                  //Current value(number)
string last;                    //Current string
Token get_token()
{
    char c = 0;
    cin >> c;
    switch (c)
    {
    case 0:
        return Crnt_token = END;        //If no input
    case '(':
    case ')':
    case '+':
    case '-':
    case '/':
    case '*':
    case '%':
    case '^':
    case '!':
    case ';':
    {
        return Crnt_token = Token(c);       //Operators
    }
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
    case '.':
    {
        cin.putback(c);
        cin >> number;
        return Crnt_token = NUM;        //Numbers
    }
    default:
        if (97 <= int(c) && int(c) <= 122)
        {
            last.clear();
            cin.putback(c);
            while ((97 <= int(c)) && (int(c) <= 122))
            {
                cin >> c;
                last.push_back(c);
            }
            cin.putback(c);
            last.pop_back();
            for (auto &a : func_name)
            {
                if (a == last)
                {
                    return Crnt_token = FUN;        //Functions
                }
            }
            for (auto &a : lookup_table)
            {
                if (a.first == last)
                {
                    number= a.second;
                    return Crnt_token = CONSTANT;        //Constants
                }
            }
            throw std::invalid_argument("Bad token(Func not defined). Found : " + last);
        }
        else
        {
            throw std::invalid_argument("Valid token expected.");
        }
    }
}
double expression();        //Definition to break cyclic dependency. Declaration below.

double primary()
{
    get_token();
    double v = number;
    switch (Crnt_token){
    case (NUM):
    case (CONSTANT):{
        get_token();
        ::number = 0;
        return v;
    }
    case (SUB):{
        double zero = 0;
        return sub(zero, primary());        //Urnary minus
    }
    case (ADD):{
        return primary();                   //Urnary plus
    }
    case (LPAREN):{
        double exp = expression();
        if (Crnt_token != RPAREN)
            throw std::invalid_argument("Rparen not found.");
        get_token();
        return exp;
    }
    case (FUN):{
        if (last == "sin")
        {
            last.clear();
            double exp = primary();
            return sin(exp);
        }
        else if (last == "cos")
        {
            last.clear();
            double exp = primary();
            return cos(exp);
        }
        else if (last == "tan")
        {
            last.clear();
            double exp = primary();
            return tan(exp);
        }
        else if (last == "cot")
        {
            last.clear();
            double exp = primary();
            return cot(exp);
        }
        else if (last == "cosec")
        {
            last.clear();
            double exp = primary();
            return cosec(exp);
        }
        else if (last == "sec")
        {
            last.clear();
            double exp = primary();
            return sec(exp);
        }
        else if (last == "inv")
        {
            last.clear();
            double exp = primary();
            return inv(exp);
        }
        else if (last == "log")
        {
            last.clear();
            double exp = primary();
            return log(exp);
        }
        else if (last == "sqrt")
        {
            last.clear();
            double exp = primary();
            return sqrt(exp);
        }
        else if (last == "abs")
        {
            last.clear();
            double exp = primary();
            return abso(exp);
        }
        else
        {
            throw std::invalid_argument("token expected.");
        }
    }
    default:
        throw std::invalid_argument("Valid token expected.");
    }
}
double pre_primary(){
    double left = primary();
    while (true)
    {
        switch (Crnt_token)
        {
        case (FAC):{
            left = fact(left);
            get_token();
            return left;
        }
        case (POW):{
            left = pow(left, int(pre_primary()));
            break;
        }
        default:
            return left;
        }
    }
}
double term()
{
    double left = pre_primary();
    while (true)
    {
        switch (Crnt_token)
        {
        case (DIV):{
            left = div(left, pre_primary());
            break;
        }
        case (MUL):{
            left = mul(left, pre_primary());
            break;
        }
        case (MOD):{
            left = mod(left, pre_primary());
            break;
        }
        default:
            return left;
        }
    }
}
double expression()
{
    double left = term();
    while (true){
        switch (Crnt_token){
        case (ADD):{
            left = add(left, term());
            break;
        }
        case (SUB):{
            left = sub(left, term());
            break;
        }
        default:
            return left;
        }
    }
}

//  <------------------------------------------------------------------------------->

int main()
{
    // User defined functions and constants
    func_name.push_back("sin");
    func_name.push_back("cos");
    func_name.push_back("tan");
    func_name.push_back("cot");
    func_name.push_back("cosec");
    func_name.push_back("sec");
    func_name.push_back("log");
    func_name.push_back("inv");
    func_name.push_back("log");
    func_name.push_back("abs");
    func_name.push_back("sqrt");
    lookup_table["e"] = 2.71828182845;
    lookup_table["pi"] = 3.14159265359;
    // Instruction:
    cout << "Add ';' character at the end of input for answer." << endl;
    // Taking input(From Stroustoup)
    try{
        while (cin)
        {
            // get_token();
            if (Crnt_token == END)
                break;
            cout << "Enter: " << '\n';
            double ans = (expression());
            cout << "= " << ans << endl;
        }
        keep_window_open();
    }
    catch (exception &e){
        cerr << e.what() << '\n';
        keep_window_open();
        return 1;
    }
    catch (...){
        cerr << "exception \n";
        keep_window_open();
        return 2;
    }
}

//  <------------------------------------------------------------------------------->

// Author: Mayank
// 22/05-24/05

/*
Improvements:
Better to use a hash map for function which hold function pointer. So application of function can be automated.
*/
