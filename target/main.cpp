#include <iostream>
#include <cassert>
#include <cstring>
#include <string>
#include <cctype>
#include <stack>
#include <list>
#include <chrono>

using namespace std;

// to control aspects of testing 
// see #define for SIMPLE_TEST in target.h

#include "target.h"

// function for internal use for testing purposes only
// uses the bc calculator to verify that the infix expressions found
// by solution to Question 4 evaluate to the number claimed
bool verify_achieved(string expression, int claimed) {
  char cmd[512];
  sprintf(cmd, "echo '%s' | bc -l", expression.c_str());
  FILE *fp = popen(cmd, "r");
  assert(fp);
  double result;
  int read = fscanf(fp, " %lg ", &result);
  assert(read == 1);
  pclose(fp);
  int result_int = result;
  assert(result_int - result < 0.0001);
  return (result_int == claimed);
}

int main() {

  cout << "============== Pre-supplied functions ==================" << endl << endl;

  cout << "postfix_to_infix(...):" << endl;
  {
    int numbers[6] = { 25, 1, 5, 9, 4, 10 };
    string infix = postfix_to_infix("12+45-*", numbers);
    cout << "12+45-* over " << numbers << " as an infix expression is " << infix << endl;
    cout << endl;
  }
  
  for (int n=1; n<=4; n++) {
    cout << "gen_shape_list(" << n << "):" << endl;
    list<string> shapes = gen_shape_list(n);
    for (string s : shapes)
      cout << s << endl;
    cout << endl;
  }
  
  cout << "====================== Question 1 ======================" << endl << endl;

  #ifdef SIMPLE_TEST
  {
    bool found = found_in("12+45-*", 3, '2');
    cout << "'2' is" << (found ? "" : " not")
	 << " found in first 3 characters of 12+45-*" << endl;    
  }
  #else
  {
    struct Q1Test {
      const char *str;
      int limit;
      char target;
      bool expected;
    };

    Q1Test tests[4] = {
      { "12+45-*",   3, '2', true },
      { "12+45-*",   4, '5', false },
      { "12-456/+*", 7, '3', false },
      { "12-456/+*", 7, '6', true }
    };
  
    for (int n=0; n<4; n++) {
      bool found = found_in(tests[n].str,tests[n].limit,tests[n].target);
      cout << "'" << tests[n].target << "' is" << (found ? "" : " not")
	   << " found in first " << tests[n].limit << " characters of "
	   << tests[n].str << " "
	   << (found == tests[n].expected ? "[expected]" : "[unexpected]")
	   << endl;
    }
  }
  #endif
  cout << endl;

  cout << "====================== Question 2 ======================" << endl << endl;

#ifdef SIMPLE_TEST
  {
    int numbers[6] = { 25, 1, 5, 9, 4, 10 };
    bool error_flag;
    int result = evaluate( "12+45-*", numbers, error_flag);
    cout << "Evaluating \"12+45-*\" over " << numbers << " gives ";
    if (error_flag) 
      cout << "error" << endl;
    else 
      cout << result << endl;
  }
  #else
  {

    int numbers[6] = { 25, 1, 5, 9, 4, 10 };
    bool error_flag;
    
    struct Q2Test {
      const char *postfix;
      int result;
      bool error_flag;      
    };
    
    Q2Test tests[9] = {
      { "4", 9, false },
      { "12+", 26, false},
      { "63/", 2, false},
      { "12+45-*",  130, false },
      { "13-6*4+5*", 836, false },
      { "425163/-*+*", 837, false },
      { "14/", 0, true }, // division with a remainder
      { "635+4-/", 0, true}, // attempted division by zero
      { "56**+", 0, true} // stack underflow
    };

    for (int n=0; n<9; n++) {      
      int result = evaluate( tests[n].postfix, numbers, error_flag);
      cout << "Evaluating \"" << tests[n].postfix << "\" over "
	   << numbers << " gives ";
      if (error_flag) 
	cout << "error" << " ";
      else 
	cout << result << " ";
      cout << ( (error_flag && tests[n].error_flag)
		|| (tests[n].result==result) ?
		"[expected]" : "[unexpected]") << endl;    }
    
  }
  #endif

  cout << endl;

  cout << "====================== Question 3 ======================" << endl << endl;

  #ifdef SIMPLE_TEST
  {
    cout << "Generating all instances matching shape \"N\":" << endl;
    list<string> instances = gen_instances_list("N");   
    for (string i:instances) 
      cout<< i << endl;
    cout << instances.size() << " total instances." << endl << endl;
  }

  {
    cout << "Generating all instances matching shape \"NNO\":" << endl;
    list<string> instances = gen_instances_list("NNO");   
    for (string i:instances) 
      cout<< i << endl;
    cout << instances.size() << " total instances." << endl << endl;
  }
  #else
  {
    struct Q3Test {
      const char *shape;
      int instances;
      bool display_instances;
    };
  
    Q3Test tests[4] = {
      { "N", 6, true},
      { "NNO", 120, false }, // change false to true to display instances
      { "NNNOO", 1920, false },
      { "NNONNOO", 23040, false }
    };

    for (int n=0; n<4; n++) {
      cout << "Generating all instances matching shape \""
	   << tests[n].shape << "\":" << endl;
      list<string> instances = gen_instances_list(tests[n].shape);
      if (tests[n].display_instances) {
	for (string i:instances) 
	  cout<< i << endl;
      } else {
	cout << "(instances not displayed)" << endl;
      }
      cout << instances.size() << " total instances. " <<
	(tests[n].instances == (int) instances.size() ? 
	 "[expected]" : "[unexpected]") << endl << endl;    
    }
  }
  #endif

  cout << "====================== Question 4 ======================" << endl << endl;

  #ifdef SIMPLE_TEST
  {
    int numbers[6] = { 25, 1, 5, 9, 4, 10 };
    int achieved;
    int target = 101;
    cout << "Aiming for " << target << " using " << numbers << ": "<< endl;
    string result = find_solution(target, numbers, achieved);
    cout << result << endl;
    cout << "Achieved = " << achieved << ", diff = "
	 << abs(target-achieved) << endl << endl;
    // assert(verify_achieved(result, achieved));
  }
  
  {
    int numbers[6] = { 25, 1, 5, 9, 4, 10 };
    int achieved;
    int target = 836;
    cout << "Aiming for " << target << " using " << numbers << ": "<< endl;
    const auto start = chrono::steady_clock::now();
    string result = find_solution(target, numbers, achieved);
    cout << result << endl;
    const auto end = chrono::steady_clock::now();  
    double elapsed_seconds = chrono::duration<double>(end - start).count();
    cout << "Achieved = " << achieved
	 << ", diff = " << abs(target-achieved)
	 << " in " << elapsed_seconds << " seconds" << endl << endl;
    // assert(verify_achieved(result, achieved));
  }
  #else
  {
    struct Q4Test {
      int numbers[6];
      int target;
      int ideal_diff;
    };

    Q4Test tests[6] = {
      { { 25, 1,  5, 9, 4, 10 }, 101, 0},
      { { 25, 1,  5, 9, 4, 10 }, 836, 0},
      { { 25, 1,  5, 9, 4, 10 }, 838, 1},
      { { 75, 6, 10, 3, 5,  4 }, 277, 0},
      { { 25, 6,  3, 3, 7, 50 }, 712, 0},
      { { 25, 7,  4, 3, 1,  1 }, 917, 3}
    };

    for (int n=0; n<6; n++) {
      int achieved;	  
      cout << "Aiming for " << tests[n].target
	   << " using " << tests[n].numbers << ": "<< endl;
      const auto start = chrono::steady_clock::now();
      string result = find_solution(tests[n].target, tests[n].numbers, achieved);
      cout << result << endl;
      const auto end = chrono::steady_clock::now();  
      double elapsed_seconds = chrono::duration<double>(end - start).count();
      cout << "Achieved = " << achieved
	   << ", diff = " << abs(tests[n].target-achieved) << " ";
      cout << ( (abs(tests[n].target-achieved) == tests[n].ideal_diff) ?
		"[expected]" : "[unexpected]") << " ";
    //   cout << (verify_achieved(result, achieved) ?
	//        "[verified]" : "[unverified]" ) << endl;
      cout << "Time taken: " << elapsed_seconds << " seconds" << endl;
      cout << endl;
    }
    
  }    
  #endif

  cout << "=================== Bonus Challenge ====================" << endl <<
    endl;

  // Can you solve all three of these famously difficult real Countdown
  // number rounds in 1 second or less?
    
  {
    struct BonusTest {
      int numbers[6];
      int target;
    };
    
    BonusTest tests[3] = {
      // Jon O'Neil https://www.youtube.com/watch?v=_JQYYz92-Uk
      { { 25, 50, 75, 100, 1, 10 }, 813},
      // James Martin https://www.youtube.com/watch?v=6mCgiaAFCu8
      { { 25, 50, 75, 100, 3, 6 }, 952},
      // George Ford https://www.youtube.com/watch?v=0q6PT4ad6BY       
      { { 50, 75, 100, 25, 2, 1 }, 940}
    };

    int achieved;

    const auto start = chrono::steady_clock::now();
    for (int n=0; n<3;n++) {
      cout << "Aiming for " << tests[n].target
	   << " using " << tests[n].numbers << ": "<< endl;
      string result = find_solution(tests[n].target, tests[n].numbers, achieved);
      cout << result << endl;
      cout << "Achieved = " << achieved
	   << ", diff = " << abs(tests[n].target-achieved) << " ";
      cout << ( (abs(tests[n].target-achieved) == 0) ?
		"[expected]" : "[unexpected]") << " ";
    //   cout << (verify_achieved(result, achieved) ?
	//        "[verified]" : "[unverified]" ) << endl;
      cout << endl;
    }
    const auto end = chrono::steady_clock::now();  
    double elapsed_seconds = chrono::duration<double>(end - start).count();
    cout << "All done in " << elapsed_seconds << " seconds" << endl << endl;      
  }

  cout << "======================= The End ========================" << endl << endl;
  
  return 0;
}