/* Program 7.5.1 from C++ Programming Lecture notes  */

/* Author: Rob Miller and William Knottenbelt
   Program last changed: 28th September 2013    */

/* This program creates and prints out a linked list of strings. */

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

const int MAX_WORD_LENGTH = 80;

/* definition of a node */
struct Node;
typedef Node *Node_ptr;

struct Node
{
        char word[MAX_WORD_LENGTH];
        Node_ptr ptr_to_next_node;
};

/* Function to assign a linked list to "a_node" */
void assign_list(Node_ptr &a_list);

/* Function to assign a new dynamic node variable to "a_node" */
void assign_new_node(Node_ptr &a_node);

/* Function to print the strings in the list "a_node" */
void print_list(Node_ptr a_node);

void add_after(Node_ptr &list, char a_word[], char word_after[]);

void delete_node(Node_ptr &a_list, char a_word[]);

void list_selection_sort(Node_ptr &a_list);

/* MAIN PROGRAM */
int main()
{
        Node_ptr my_list = NULL;

        assign_list(my_list);

        cout << "\nTHE LIST IS NOW:\n";
        print_list(my_list);
        cout << endl << endl;

/*
        char extra_word[MAX_WORD_LENGTH];
        char after_word[MAX_WORD_LENGTH];

        cout << "AFTER WHICH WORD WOULD YOU LIKE TO ADD AN EXTRA WORD? ";
        cin >> after_word;
        cout << "WHICH WORD YOU LIKE TO ADD? ";
        cin >> extra_word;;
        add_after(my_list, extra_word, after_word);


        cout << "\nTHE LIST IS NOW:\n";
        print_list(my_list);
        cout << endl;


        char deleted_word[MAX_WORD_LENGTH];

        cout << "WHICH WORD WOULD YOU LIKE TO DELETE? ";
        cin >> deleted_word;
        delete_node(my_list, deleted_word);


        cout << "\nTHE LIST IS NOW:\n";
        print_list(my_list);
        cout << endl;
*/

        list_selection_sort(my_list);
        cout << "\nAFTER SORTING, THE LIST IS:\n";
        print_list(my_list);
        cout << endl;

        return 0;
}
/* END OF MAIN PROGRAM */

/* DEFINITION OF FUNCTION "assign_list" */
void assign_list(Node_ptr &a_list)
{
        Node_ptr current_node, last_node;

        assign_new_node(a_list);
        cout << "Enter first word (or '.' to end list): ";
        cin >> a_list->word;
        if (!strcmp(".",a_list->word))
        {
                delete a_list;
                a_list = NULL;
        }
        current_node = a_list;

        while (current_node != NULL)
        {
                assign_new_node(last_node);
                cout << "Enter next word (or '.' to end list): ";
                cin >> last_node->word;
                if (!strcmp(".",last_node->word))
                {
                        delete last_node;
                        last_node = NULL;
                }
                current_node->ptr_to_next_node = last_node;
                current_node = last_node;
        }
}
/* END OF FUNCTION DEFINITION */

/* DEFINITION OF FUNCTION "assign_new_node" */
void assign_new_node(Node_ptr &a_node)
{
        a_node = new (nothrow) Node;
        if (a_node == NULL)
        {
                cout << "sorry - no more memory\n";
                exit(1);
        }
}
/* END OF FUNCTION DEFINITION */

/* DEFINITION OF FUNCTION "print_list" */
void print_list(Node_ptr a_node)
{
        while (a_node != NULL)
        {
                cout << a_node->word << " ";
                a_node = a_node->ptr_to_next_node;
        }
}
/* END OF FUNCTION DEFINITION */

void add_after(Node_ptr &list, char a_word[], char word_after[])
{
        Node_ptr current_node = list;

        while(current_node != NULL) {
                if(strcmp(current_node->word, word_after) == 0) {
                        Node_ptr new_node;
                        assign_new_node(new_node);
                        strcpy(new_node->word, a_word);
                        new_node->ptr_to_next_node = current_node->ptr_to_next_node;
                        current_node->ptr_to_next_node = new_node;
                }
                current_node = current_node->ptr_to_next_node;
        }
}

void delete_node(Node_ptr &a_list, char a_word[])
{
        if(strcmp(a_list->word, a_word) == 0) {
                a_list = a_list->ptr_to_next_node;
                return;
        }

        Node_ptr current_node = a_list;

        while(current_node->ptr_to_next_node != NULL) {
                if(strcmp(current_node->ptr_to_next_node->word, a_word) == 0) {
                        Node_ptr temp_node = current_node->ptr_to_next_node;
                        current_node->ptr_to_next_node = temp_node->ptr_to_next_node;
                        delete temp_node;
                        temp_node = nullptr;
                }
                current_node = current_node->ptr_to_next_node;
        }
}


void list_selection_sort(Node_ptr &a_list)
{
        Node_ptr target_node = a_list;

        while(target_node->ptr_to_next_node != NULL) {
                // Search for min node
                Node_ptr test_node = target_node->ptr_to_next_node;
                Node_ptr min_node = target_node;
                while(test_node != NULL) {
                        if(strcmp(test_node->word, min_node->word) < 0) {
                                min_node = test_node;
                        }
                        test_node = test_node->ptr_to_next_node;
                }
                // Swap
                char temp[MAX_WORD_LENGTH];
                strcpy(temp, min_node->word);
                strcpy(min_node->word, target_node->word);
                strcpy(target_node->word, temp);

                target_node = target_node->ptr_to_next_node;
        }
}

/*
a_list -->      Node #1         the
                                ptr to Node #2

                Node #2         quick
                                ptr to Node #3

                Node #3         brown         
                                ptr to Node #4

                Node #4         fox
                                nullptr
*/
        

