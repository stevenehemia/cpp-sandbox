#include <iostream>
#include <cassert>
#include <cstring>
#include <cmath>
#include <string>
#include <cctype>
#include <stack>
#include <list>
#include <chrono>

using namespace std;

#include "target.h"

/* pre-supplied functions start here */

// overloaded output operator so that array of given numbers can be
// easily output to a stream
ostream &operator<<(ostream &output, int number[]) {
  output << "(";
  for (int n=0; n<6; n++) {
    output << number[n];
    if (n<5)
      output << ",";    
  }
  output << ")";
  return output;
}

// converts given postfix expression in string pf over given numbers
// into an infix expression
string postfix_to_infix(string pf, int number[6]) {
  
  if (pf == "none")
    return pf;
  
  stack<string> s;
  
  for (int i=0; pf[i]; i++) {
    if (isdigit(pf[i])) {
      int idx = pf[i] - '1';
      assert(idx >=0 && idx < 6);
      s.push(to_string(number[idx]));
    } else {
      string arg1 = s.top();
      s.pop();
      string arg2 = s.top();
      s.pop();
      s.push( (pf[i+1]?"(":"") + arg2 + pf[i] + arg1 + (pf[i+1]?")":"") );
    }
  }
  return s.top();
}

// recursive helper function for gen_shape_list which populates
// list of candidate shapes by expanding selected "N"s into "NNO"
void gen_shape(const char *start, int unused_vars, list<string> &candidates, int index = 0) {

  if (!index) {
    if (unused_vars == 1) {
      // avoid duplicates
      for (string c : candidates)
	if (c==start)
	  return;
      candidates.push_back(start);
      return;
    }
  }

  if (!start[index])
    return;

  if (start[index] == 'O') {
    gen_shape(start, unused_vars, candidates, index + 1);
    return;
  }

  //  for each N in start, either leave it or replace it with NNO  
  if (start[index] == 'N') {
    if (unused_vars > 1) {
      char *next = new char[strlen(start) + 3];
      int n,p;
      for (n=0,p=0; n<index; n++,p++)
	next[n] = start[n];
      next[n++] = 'N';
      next[n++] = 'N';
      next[n++] = 'O';
      p++;
      for (; start[p]; p++, n++) 
	next[n] = start[p];
      next[n++] = '\0';	
      gen_shape(next, unused_vars - 1, candidates, 0);
      delete [] next;
    }
    gen_shape(start, unused_vars, candidates, index + 1);
  }
}

// generates all possible solution shapes that use a certain
// quantity of numbers (operands). A shape is a template for a
// postfix expression that uses the letter N to denote a number
// (operand) and the letter O to denote an operator.
list<string> gen_shape_list(int target_used) {
  list<string> candidates;
  gen_shape("N", target_used, candidates);
  return candidates;
}

/* add your own function definitions here */

const char OPERATORS[4] = {'+', '-', '*', '/'};
const char TOTAL_NUMBER = '6';

bool found_in(string str, int limit, char target) {
    for (int i = 0; i < limit; i++) {
        if (i > str.size()) { break; }
        if (str[i] == target) { return true; }
    }
    return false;
}

int evaluate(string str, int numbers[], bool &error_flag) {
    stack<int> op_stk;
    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
            int num_pos = str[i] - '1';
            op_stk.push(numbers[num_pos]);
            continue;
        }
        for (int j = 0; j < 4; j++) {
            if (str[i] == OPERATORS[j]) {
                error_flag = perform_operation(op_stk, str[i]);
                if (error_flag) { return 0; }
            }
        }
    }
    return op_stk.top();
}

bool perform_operation(stack<int> &stk, char opr) {
    if (stk.size() < 2) { return true; }
    int op1 = static_cast<int>(stk.top());
    stk.pop();
    int op2 = static_cast<int>(stk.top());
    stk.pop();
        switch (opr) {
            case '+': stk.push(op2 + op1); break;
            case '-': stk.push(op2 - op1); break;
            case '*': stk.push(op2 * op1); break;
            case '/': 
                if (op1 == 0) { return true; }
                if (op2 % op1 != 0) { return true; }
                stk.push(op2 / op1); break;
            default: return true;
        }
    return false;
}

list<string> gen_instances_list(string shape) {
    list<string> instances_list;
    string instance = "";
    gen_instance(instances_list, shape, instance);
    return instances_list;
}

void gen_instance(list<string> &list, string shape, string instance) {

    if (shape.empty()) {
        list.push_back(instance);
        return;
    }

    if (shape[0] == 'O') {
        for (char op: OPERATORS) {
            instance = instance + op;
            gen_instance(list, shape.substr(1, shape.length()), instance);
            instance.pop_back();
        }
        return;
    }

    if (shape[0] == 'N') {
        for (char i = '1'; i <= TOTAL_NUMBER; i++) {
            if (found_in(instance, instance.length(), i)) { continue; }
            instance = instance + i;
            gen_instance(list, shape.substr(1, shape.length()), instance);
            instance.pop_back();
        }
        return;
    }

    cerr << shape[0] << " is not a valid shape format!";
    return;
}

string find_solution(int target, int numbers[], int &achieved) {
    string best_instance;
    int result = 0;
    int diff = INT_MAX;
    for (int n = 2; n <= 6; n++) {
        list<string> shapes = gen_shape_list(n);
        for (string shape: shapes) {
            list<string> instances = gen_instances_list(shape);
            for (string instance: instances) {
                bool error_flag;
                result = evaluate(instance, numbers, error_flag);
                if (error_flag) { continue; }
                if ((result - target) == 0) {
                    achieved = result;
                    best_instance = instance;
                    return postfix_to_infix(best_instance, numbers);
                }
                if (abs(result - target) < diff) {
                    achieved = result;
                    best_instance = instance;
                    diff = abs(result - target);
                }
            }
        }
    }
    return postfix_to_infix(best_instance, numbers);
}