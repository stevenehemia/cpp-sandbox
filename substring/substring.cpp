
bool is_prefix(const char *string_1, const char *string_2) {

    // recursive version
    if(string_1[0] == '\0') {
        return true;
    }

    if(string_1[0] == string_2[0]) {
        if(is_prefix(&string_1[1], &string_2[1])) {
            return true;
        } else {
            return false;
        }
    }

    return false;

    // iterative version
    /*
    int i = 0;
    while(string_1[i] != '\0') {
        if(string_1[i] != string_2[i]) {
            return false;
        }
        i++;
    }
    return true;
    */
}


int substring_position(const char *string_a, const char *string_b) {
    
    // recursive version
    if(string_b[0] == '\0') {
        return 0;
    }

    if(is_prefix(string_a, string_b)) {
        return 0;
    } else {
        return 1 + substring_position(string_a, &string_b[1]);
    }

    // iterative version
    /*
    int i = 0;
    while(string_b[i] != '\0') {
        if(is_prefix(string_a, &string_b[i])) {
            return i;
        }
        i++;
    }
    return -1;
    */
}