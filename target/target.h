// uncomment to activate simple test cases
// #define SIMPLE_TEST

// overloaded output operator so that array of given numbers can be
// easily output to a stream
ostream &operator<<(ostream &output, int number[]);

// converts given postfix expression in string pf over given numbers
// into an infix expression
string postfix_to_infix(string pf, int number[6]);

// generates all possible solution shapes that use a certain
// quantity of numbers (operands). A shape is a template for a
// postfix expression that uses the letter N to denote a number
// (operand) and the letter O to denote an operator.
list<string> gen_shape_list(int target_used);

/* place your function prototypes here */

bool found_in(string str, int limit, char target);

int evaluate(string str, int numbers[], bool &error_flag);
bool perform_operation(stack<int> &stk, char opr);
list<string> gen_instances_list(string shape);
void gen_instance(list<string> &list, string shape, string instance);
string find_solution(int target, int numbers[], int &achieved);
