#include <iostream>
#include <string>

using namespace std;

/*const int MAX = 1000;

string expressions[MAX];
int exprCount = 0;*/

#include <vector>
vector<string> expressions;

struct StringStack {
    string data[1000];
    int top = -1;
    void push(string s) { data[++top] = s; }
    string pop() { return data[top--]; }
    string peek() { return data[top]; }
    bool empty() { return top == -1; }
};

struct CharStack {
    char data[1000];
    int top = -1;
    void push(char c) { data[++top] = c; }
    char pop() { return data[top--]; }
    char peek() { return data[top]; }
    bool empty() { return top == -1; }
};

string removeSpaces(string str) {
    string res = "";
    for (char c : str)
        if (c != ' ') res += c;
    return res;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool greaterEqual(string a, string b) {
    if (a.length() != b.length()) return a.length() > b.length();
    return a >= b;
}

string reverseString(string s) {
    string res = "";
    for (int i = s.length() - 1; i >= 0; --i) res += s[i];
    return res;
}

string add(string a, string b) {
    if (a.length() < b.length()) swap(a, b);
    string res = "";
    int carry = 0, i = a.length() - 1, j = b.length() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int d1 = i >= 0 ? a[i--] - '0' : 0;
        int d2 = j >= 0 ? b[j--] - '0' : 0;
        int sum = d1 + d2 + carry;
        carry = sum / 10;
        res += (sum % 10) + '0';
    }
    return reverseString(res);
}

string subtract(string a, string b) {
    if (!greaterEqual(a,b)){
        return "-" + subtract(b, a);
    }

    string res = "";
    int borrow = 0, i = a.length() - 1, j = b.length() - 1;
    while (i >= 0) {
        int d1 = a[i--] - '0', d2 = j >= 0 ? b[j--] - '0' : 0;
        d1 -= borrow;
        if (d1 < d2) {
            d1 += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res += (d1 - d2) + '0';
    }
    while (res.length() > 1 && res.back() == '0') res.pop_back();
    return reverseString(res);
}

string multiply(string a, string b) {
    int n = a.size(), m = b.size();
    int result[200] = {0};
    for (int i = n - 1; i >= 0; --i)
        for (int j = m - 1; j >= 0; --j) {
            int mul = (a[i] - '0') * (b[j] - '0');
            result[i + j + 1] += mul;
        }
    for (int i = n + m - 1; i > 0; --i) {
        result[i - 1] += result[i] / 10;
        result[i] %= 10;
    }
    string res = "";
    int i = 0;
    while (i < n + m && result[i] == 0) ++i;
    for (; i < n + m; ++i) res += result[i] + '0';
    return res == "" ? "0" : res;
}

string divide(string a, string b) {
    if (b == "0") return "Error";
    string res = "", temp = "";
    for (int i = 0; i < a.length(); ++i) {
        temp += a[i];
        int count = 0;
        while (greaterEqual(temp, b)) {
            temp = subtract(temp, b);
            count++;
        }
        res += count + '0';
        while (temp.length() > 1 && temp[0] == '0') temp.erase(0, 1);
    }
    while (res.length() > 1 && res[0] == '0') res.erase(0, 1);
    return res.empty() ? "0" : res;
}

//void tokenizeInfixToPostfix(string expr, string postfix[], int& count) {
void tokenizeInfixToPostfix(string expr, vector<string>&  postfix){
    CharStack ops;
    string num = "";

    for (int i = 0; i < expr.length(); ++i) {
        char c = expr[i];
        if (isdigit(c)) {
            num += c;
        } else {
            if (!num.empty()) {
                //postfix[count++] = num;
                postfix.push_back(num);
                num = "";
            }
            if (c == '(') ops.push(c);
            else if (c == ')') {
                while (!ops.empty() && ops.peek() != '(')
                    postfix.push_back(string(1, ops.pop()));
                ops.pop(); // remove '('
            } else {
                while (!ops.empty() && precedence(ops.peek()) >= precedence(c))
                    postfix.push_back( string(1, ops.pop()));
                ops.push(c);
            }
        }
    }

    if (!num.empty()) postfix.push_back( num);

    while (!ops.empty())
        postfix.push_back( string(1, ops.pop()));
}

//string evaluatePostfix(string postfix[], int count) {
string evaluatePostfix(const vector<string>& postfix){
    StringStack stk;
    for (const string&token : postfix) {
        if (isdigit(token[0])) {
            stk.push(token);
        } else {
            string b = stk.pop();
            string a = stk.pop();
            if (token == "+") stk.push(add(a, b));
            else if (token == "-") stk.push(subtract(a, b));
            else if (token == "*") stk.push(multiply(a, b));
            else if (token == "/") {
                string res = divide(a, b);
                if (res == "Error") return "Error: Division by zero";
                stk.push(res);
            }
        }
    }
    return stk.pop();
}

int main() {
    freopen("tests.txt", "r", stdin);
    freopen("output_10423181.txt", "w", stdout);

    string line;
    while (getline(cin, line)) {
        line = removeSpaces(line);
        /*string postfix[1000];
        int count = 0;
        tokenizeInfixToPostfix(line, postfix, count);*/
        vector<string> postfix;
        tokenizeInfixToPostfix(line, postfix);
        string result = evaluatePostfix(postfix);
        cout << result << endl;
    }

    return 0;
}
