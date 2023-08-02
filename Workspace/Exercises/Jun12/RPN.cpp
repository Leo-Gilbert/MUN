#include <iostream>
#include <stack>
#include <string>
#include <vector>

int RPN(std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end) {
    //stack to hold numbers in the given expression
    std::stack<int> operands;

    for (auto it = begin; it != end; ++it) {
        //holding the current character in the expression
        const std::string& token = *it;
        //performing operations based on operator found in expression
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            
            //removing top 2 operands from stack and performing operations
            //then, after a given operation, adding the resulting operand back
            //into the stack
            int operand2 = operands.top();
            operands.pop();
            int operand1 = operands.top();
            operands.pop();

            if (token == "+"){
                operands.push(operand1 + operand2);
            } 
            else if (token == "-"){
                operands.push(operand1 - operand2);
            } 
            else if (token == "*"){
                operands.push(operand1 * operand2);
            } 
            else if (token == "/"){
                operands.push(operand1 / operand2);
            }
        }

        else {
            //converting numbers from string to integer and storing them in the number stack
            operands.push(std::stoi(token));
        }
    }
    //returning the single remaining number left in the stack
    return operands.top();
}

int main(){
    //test case
    std::vector<std::string> rpnExpr = { "1", "2", "+", "3", "*" };
    int result = RPN(rpnExpr.begin(), rpnExpr.end());
    std::cout << "Result: " << result << std::endl;

    return 0;
}