/**
 * @brief 159201 assignment 2
 * 
 * @author Wan Qian ID:17326163
 *
 */

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct StackNode {
	int mValue;
	StackNode *next;
	StackNode(int _x) :mValue{ _x }, next(nullptr) {}
};

struct ExpressionNode {
	int mValue;
	char mOperator;
	ExpressionNode(int _x) : mValue{ _x }, mOperator('0') {}
	ExpressionNode(char _op) :mValue(0), mOperator{ _op } {}
};

class Stack {
public:
	Stack() :linkedlistPtr(nullptr) {}
	~Stack() {}
	void push(int);
	void pop();
	int top();
	bool isEmpty();
	void print();

private:
	StackNode * linkedlistPtr;
};

void Stack::push(int _x) {
	StackNode *newNode = new StackNode(_x);
	newNode->next = linkedlistPtr;
	linkedlistPtr = newNode;
}

void Stack::pop() {
	if (linkedlistPtr == nullptr) {
		return;
	}
	StackNode *temp = linkedlistPtr;
	linkedlistPtr = linkedlistPtr->next;
	delete temp;
}

int Stack::top() {
	return linkedlistPtr->mValue;
}

bool Stack::isEmpty() {
	return linkedlistPtr == nullptr;
}

void Stack::print() {
	if (linkedlistPtr == nullptr) {
		return;
	}
	StackNode *current = linkedlistPtr;
	while (current != nullptr) {
		cout << current->mValue << " ";
		current = current->next;
	}
}

//Implements a calculator with Reverse Polish notation
class Calculator {

public:
	bool readFile(const char*);
	bool calculate();
	bool printResult();

private:
	Stack stack;
	vector<ExpressionNode> expression;

	bool extractOperandFromStack(int&);
	void compute(int, int, char);
};

int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "Please input a file name" << endl;
		exit(0);
	}
	Calculator cal;
	if (!cal.readFile(argv[1]) || !cal.calculate() || !cal.printResult()) {
		exit(0);
	}
}

/**
 * tested
 */
bool Calculator::readFile(const char* fileName) {
	ifstream fin;
	fin.open(fileName);
	if (!fin) {
		cerr << "Can not read file " << fileName << ". Program terminates" << endl;
		return false;
	}
	while (!fin.eof()) {
		string line;
		int value = 0;
		char op = '0';
		getline(fin, line);
		if (line.empty()) {
			continue;
		}
		//store an operator
		if (line == "+" || line == "-" || line == "*" || line == "/") {
			stringstream stoc(line);
			stoc >> op;
			expression.push_back(ExpressionNode(op));
		}
		//store a negative number
		else if (line[0] == '-' && line.size() > 1) {
			stringstream stoi(line);
			stoi >> value;
			expression.push_back(ExpressionNode(value));
		}
		//store a non-negative operand
		else if (isdigit(line[0])) {
			stringstream stoi(line);
			stoi >> value;
			expression.push_back(ExpressionNode(value));
		}
		//Invalid token
		else {
			cerr << "Invalid operator " << line << "\nProgram terminated" << endl;
			exit(1);
		}
	}
	fin.close();
	return true;
}

/**
 * tested
 */
bool Calculator::calculate() {
	if (expression.empty()) {
		cout << "No expression to evaluate.Program terminated" << endl;
		return false;
	}
	for (auto token : expression) {
		int operand1 = 0, operand2 = 0;
		//When operator equals to '0',the node represents an operand.
		if (token.mOperator == '0') {
			cout << "reading number " << token.mValue << endl;
			stack.push(token.mValue);
		}
		//When encounter an operator,do the evaluation.
		else {
			cout << "reading operator " << token.mOperator << endl;
			if (extractOperandFromStack(operand1) && extractOperandFromStack(operand2)) {
				compute(operand1, operand2, token.mOperator);
			}
			else {
				cerr << "too many operators" << endl;
				return false;
			}
		}
	}
	return true;
}

bool Calculator::printResult() {
	int result = 0;
	extractOperandFromStack(result);
	if (!stack.isEmpty()) {
		cerr << "too many numbers" << endl;
		return false;
	}
	cout << "The result is " << result << endl;
	return true;
}

bool Calculator::extractOperandFromStack(int &_operand) {
	if (stack.isEmpty()) {
		return false;
	}
	_operand = stack.top();
	stack.pop();
	return true;
}

void Calculator::compute(int op1, int op2, char oper) {
	switch (oper) {
	case '+':
		stack.push(op1 + op2);
		break;
	case '-':
		stack.push(op2 - op1);
		break;
	case '*':
		stack.push(op1 * op2);
		break;
	case '/':
		stack.push(op2 / op1);
		break;
	}
}
