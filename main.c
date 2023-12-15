#include <stdio.h>

#define HASH_MOD 1000000009  /* Hash module */
#define HASH_B1 43  /* Hash base 1 */
#define HASH_B2 47  /* Hash base 2 */
#define MAXLEN_N 128  /* Max file name length */
#define MAXLEN_K 128  /* Max keyword length */
#define MAXKEYS 256  /* Max keywords count */
#define MAXLEN_D 64  /* Max delimiters length */
#define MAXLEN_S 512 /* Max text lines length */
#define MAXLINES 512  /* Max lines count */

/* The function checks if there is "c" in the array delimiters */
int is_delimiter(char c, char *delimiters);

int main() {
    /* Block of variables */
    int r;                          /* Module of hash */
    int t;                          /* First hash base */
    int b;                          /* Second hash base */
    char type_of_input;             /* Character variable for input type ('f' or 'c') */
    char filename[MAXLEN_N];        /* Array to store the filename */
    FILE *f;                        /* File pointer */
    int cnt_of_keywords;            /* Count of keywords */
    char keyword[MAXLEN_K];         /* Array to store keywords */
    char *x;                        /* Character pointer */
    long long hash_of_word1;        /* Long integer for first hash value */
    long long hash_of_word2;        /* Long integer for second hash value */
    int keysh[MAXKEYS][3];          /* 2D array for storing hash values and counts */
    char delimiters[MAXLEN_D];      /* Array to store delimiters */
    int flag;                       /* Flag for control input */
    int lines;                      /* Count of lines */
    char text[MAXLINES][MAXLEN_S];  /* 2D array to store text */
    char *start;                    /* Character pointer for the start of a word */
    int min_line;                   /* Minimum word length in line */
    int cnt_words_bef;              /* Count of founded words before */
    int cnt_words_now;              /* Count of founded words now */
    char *min_word[MAXLINES][2];    /* 2D array to store minimum words in each line */
    char type_of_output;            /* Character variable for output type ('f' or 'c') */


    /* Block of initialization */
    r = HASH_MOD;
    t = HASH_B1;
    b = HASH_B2;
    flag = 1;
    lines = 0;
    cnt_words_bef = 0;

    /* Info */
    printf("Hello! The program removes the minimum words in the lines that contain keywords.\n");

    /* Block of input */
    /* Prompt the user for input type (file or console) */
    printf("Input from file or console? (f/c)\n");
    type_of_input = getchar();

    /* Validate the input type; loop until a valid input is provided */
    while (type_of_input != 'f' && type_of_input != 'c') {
        printf("Something went wrong! Please enter \'f\' or \'c\':\n");
        type_of_input = getchar();
    }

    /* Process based on the input type */
    if (type_of_input == 'f') {
        /* Prompt user for the file name and attempt to open the file */
        printf("Please enter the file name (limit: %i chars):\n", MAXLEN_N);
        scanf("%s", filename);
        f = fopen(filename, "r");

        /* Continue prompting until a valid file is opened */
        while (f == NULL) {
            printf("Something went wrong! Perhaps such a file does not exist."
                   "\nPlease enter the file name again:\n");
            scanf("%s", filename);
            f = fopen(filename, "r");
        }
    } else {
        /* If console input, set the file pointer to standard input */
        f = stdin;
    }

    /* For console input, prompt for the count of keywords */
    if (type_of_input == 'c') printf("Please enter the count of keywords (limit: %i):\n", MAXKEYS);
    fscanf(f, "%i", &cnt_of_keywords);

    /* For console input, prompt for each keyword and compute hash values */
    if (type_of_input == 'c') printf("Please enter the keywords (limit: %i chars):\n", MAXLEN_K);
    for (int i = 0; i < cnt_of_keywords; ++i) {
        fscanf(f, "%s", keyword);
        x = keyword;
        hash_of_word1 = 0;
        hash_of_word2 = 0;

        /* Compute double hash values for each character in the keyword */
        while (*x != '\0') {
            hash_of_word1 = (hash_of_word1 * t + (int) *x) % r;
            hash_of_word2 = (hash_of_word2 * b + (int) *x) % r;
            ++x;
        }

        /* Store the double hash values and initialize count in the keysh array */
        keysh[i][0] = hash_of_word1;
        keysh[i][1] = hash_of_word2;
        keysh[i][2] = 0;
    }

    /* For console input, prompt for the line of delimiter characters */
    if (type_of_input == 'c') printf("Please enter the line of delimiter characters (limit: %i chars):\n", MAXLEN_D);
    fgets(delimiters, MAXLEN_D, f);
    fgets(delimiters, MAXLEN_D, f);

    /* For console input, prompt for the lines of text with specified limits */
    if (type_of_input == 'c')
        printf("Please enter the lines of text (limit of line\'s length: %i; limit\n"
               "count of lines: %i):\n", MAXLEN_S, MAXLINES);


    /* Block of main logic */
    /* Loop through lines of text from the input file or console */
    while (flag == 1 && fgets(text[lines], MAXLEN_S, f)) {
        x = text[lines];
        start = text[lines];
        hash_of_word1 = 0;
        hash_of_word2 = 0;
        min_line = MAXLEN_S;

        /* Iterate through characters in the current line */
        while (*x != '\0') {
            /* Check if the character is a delimiter */
            if (is_delimiter(*x, delimiters) == 1) {
                /* Process the word if hash values are not zero */
                if (hash_of_word1 != 0 || hash_of_word2 != 0) {
                    /* Check if the word matches any keyword and update counts */
                    for (int i = 0; i < cnt_of_keywords; ++i) {
                        if (hash_of_word1 == keysh[i][0] && hash_of_word2 == keysh[i][1]) {
                            ++keysh[i][2];
                        }
                    }
                    /* Reset hash values for the next word */
                    hash_of_word1 = 0;
                    hash_of_word2 = 0;
                }

                /* Update minimum word information if conditions are met */
                if (x - start != 0 && min_line > x - start) {
                    min_line = x - start;
                    min_word[lines][0] = start;
                    min_word[lines][1] = x;
                }
                start = x + 1; /* Move start pointer to the next character */
            } else {
                /* Update hash values for the current word */
                hash_of_word1 = (hash_of_word1 * t + (int) *x) % r;
                hash_of_word2 = (hash_of_word2 * b + (int) *x) % r;
            }
            ++x;
        }

        /* Update flag and word count information */
        flag = 0;
        cnt_words_now = 0;

        /* Check if any keyword count is still zero, set the flag accordingly */
        for (int i = 0; i < cnt_of_keywords; ++i) {
            if (keysh[i][2] == 0) flag = 1;
            cnt_words_now += keysh[i][2];
        }

        /* Update minimum word information if the current word count matches the previous one */
        if (cnt_words_now == cnt_words_bef) {
            min_word[lines][0] = text[lines];
            min_word[lines][1] = text[lines];
        }

        cnt_words_bef = cnt_words_now; /* Update the previous word count */
        lines++; /* Move to the next line in the text */
    }

    /* Close the file if input is from a file */
    if (type_of_input == 'f') fclose(f);


    /* Block of output */
    /* Prompt the user for output type (file or console) */
    printf("Output to file or console? (f/c)\n");

    /* If the input was from a file, consume an extra newline character from the buffer */
    if (type_of_input == 'f') getchar();

    /* Get the user's choice for output type */
    type_of_output = getchar();

    /* Validate the output type; loop until a valid input is provided */
    while (type_of_output != 'f' && type_of_output != 'c') {
        printf("Something went wrong! Please enter \'f\' or \'c\':\n");
        type_of_output = getchar();
    }

    /* Process based on the output type */
    if (type_of_output == 'f') {
        /* Prompt user for the file name and attempt to open the file for writing */
        printf("Please enter the file name (limit: %i chars):\n", MAXLEN_N);
        scanf("%s", filename);
        f = fopen(filename, "w");

        /* Continue prompting until a valid file is opened */
        while (f == NULL) {
            printf("Something went wrong! Please enter the file name again:\n");
            scanf("%s", filename);
            f = fopen(filename, "w");
        }
    } else {
        /* If console output, set the file pointer to standard output */
        printf("Processed text:\n");
        f = stdout;
    }

    /* Process and output each line of text, excluding the minimum words */
    for (int i = 0; i < lines; ++i) {
        x = text[i];
        while (*x != '\0') {
            /* Skip characters within the minimum words and update the pointer */
            if (x == min_word[i][0]) {
                x = min_word[i][1];
                min_word[i][0] = NULL;
            }
            /* Output the character to the file or console */
            fprintf(f, "%c", *x);
            ++x;
        }
    }

    /* If the output type is a file, display a completion message, close the file, and finalize */
    if (type_of_output == 'f') {
        printf("DONE");
        fclose(f);
    }

    return 0;
}

int is_delimiter(char c, char *delimiters) {
    char *x;
    int ans = 0;
    x = delimiters;
    while (*x != '\0') {  /* Iterating until the end of delimiters */
        if (c == *x) ans = 1;
        ++x;
    }
    return ans;  /* Return 1 if the element in the array 0 else */
}
