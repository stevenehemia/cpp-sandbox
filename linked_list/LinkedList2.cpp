
void add_after(Node *&list, char a_word[], char word_after[])
{
    Node = *current = list;
    while(current) {
        // we know that current is not NULL
        if(strcmp(word_after,current->word) == 0) {
            // we found the node to add after

            Node *new_node = assign_new_node(); // create the new node (in the heap)
            strcpy(new_node->word, a_word);     // copy the word into the new node
            
            // insert the new node
            new_node->next = current->next
            current->next = new_node;
            
            return;
        }
        // end of the loop
        current = current->next;
        // we do not know if current is NULL or not
    }
}

// incomplete yet
void add_after_recursive(Node *list, char a_word[], char word_after[])
{
    if(list == NULL) {
        return;
    }

    // we know that current is not NULL
    if(strcmp(word_after,list->word) == 0) {
        // we found the node to add after

        Node *new_node = assign_new_node(); // create the new node (in the heap)
        strcpy(new_node->word, a_word);     // copy the word into the new node
            
        // insert the new node
        new_node->next = current->next
        current->next = new_node;
            
        return;
    }
}


// Confusing way to write the function
void foo(int *p) {
    if(p != NULL) {
        // p is not NULL
        // Do work here
    } else {
        // p is NULL
        return;
    }
}
 // Better way to write the function
void bar(int *p) {
    if (p == NULL) {
        return;
    }
}