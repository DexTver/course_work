#include <stdio.h>

#define HASH_MOD 1000000009  // hash module
#define HASH_B1 43  // hash base 1
#define HASH_B2 47  // hash base 2
#define MAXLEN_N 128  // file name
#define MAXLEN_K 128  // keyword
#define MAXLEN_D 64  // delimiters
#define MAXLEN_S 512 // text line
#define MAXLINES 512  // lines

int is_delimiter(char c, char *delimiters) {
    char *x;
    int ans = 0;
    x = delimiters;
    while (*x != '\0') {
        if (c == *x) ans = 1;
        ++x;
    }
    return ans;
}

int main() {
    // Block of variables
    int r;
    int t;
    int b;
    char type_of_input;
    char filename[MAXLEN_N];
    FILE *f;
    int cnt_of_keywords;
    char keyword[MAXLEN_K];
    char *x;
    long long hash_of_word1;
    long long hash_of_word2;
    char delimiters[MAXLEN_D];
    int flag;
    int lines;
    char text[MAXLINES][MAXLEN_S];
    char *start;
    int min_line;
    int cnt_words_bef;  // count of found words before
    int cnt_words_now;  //
    char *min_word[MAXLINES][2];
    char type_of_output;

    // Block of initialization
    r = HASH_MOD;
    t = HASH_B1;
    b = HASH_B2;
    flag = 1;
    lines = 0;
    cnt_words_bef = 0;

    // Block of input
    printf("Input from file or console? (f/c)\n");
    type_of_input = getchar();
    while (type_of_input != 'f' && type_of_input != 'c') {
        printf("Something went wrong! Please enter \'f\' or \'c\':\n");
        type_of_input = getchar();
    }
    if (type_of_input == 'f') {
        printf("Please enter the file name (limit: %i chars):\n", MAXLEN_N);
        scanf("%s", filename);
        f = fopen(filename, "r");
        while (f == NULL) {
            printf("Something went wrong! Perhaps such a file does not exist."
                   "\nPlease enter the file name again:\n");
            scanf("%s", filename);
            f = fopen(filename, "r");
        }
    } else {
        f = stdin;
    }

    if (type_of_input == 'c') printf("Please enter the count of keywords:\n");
    fscanf(f, "%i", &cnt_of_keywords);

    int keysh[cnt_of_keywords][3];  // ?!

    if (type_of_input == 'c') printf("Please enter the keywords:\n");
    for (int i = 0; i < cnt_of_keywords; ++i) {
        fscanf(f, "%s", keyword);
        x = keyword;
        hash_of_word1 = 0;
        hash_of_word2 = 0;
        while (*x != '\0') {
            hash_of_word1 = (hash_of_word1 * t + (int) *x) % r;
            hash_of_word2 = (hash_of_word2 * b + (int) *x) % r;
            ++x;
        }
        keysh[i][0] = hash_of_word1;
        keysh[i][1] = hash_of_word2;
        keysh[i][2] = 0;
    }

    if (type_of_input == 'c') printf("Please enter the line of delimiter characters (limit: %i chars):\n", MAXLEN_D);
    fgets(delimiters, MAXLEN_D, f);  // Êîñòûëü, ÷òîáû ñ÷èòàòü \n
    fgets(delimiters, MAXLEN_D, f);

    if (type_of_input == 'c')
        printf("Please enter the lines of text (limit of line\'s length: %i; limit\n"
               "count of lines: %i):\n", MAXLEN_S, MAXLINES);

    // Block of main logic
    while (flag == 1 && fgets(text[lines], MAXLEN_S, f)) {
        x = text[lines];
        start = text[lines];
        hash_of_word1 = 0;
        hash_of_word2 = 0;
        min_line = MAXLEN_S;
        while (*x != '\0') {
            if (is_delimiter(*x, delimiters) == 1) {
                for (int i = 0; i < cnt_of_keywords; ++i) {
                    if (hash_of_word1 == keysh[i][0] && hash_of_word2 == keysh[i][1]) {
                        ++keysh[i][2];
                    }
                    hash_of_word = 0;
                }
                if (x - start != 0 && min_line > x - start) {
                    min_line = x - start;
                    min_word[lines][0] = start;
                    min_word[lines][1] = x;
                }
                start = x + 1;
                hash_of_word1 = 0;
                hash_of_word2 = 0;
            } else {
                hash_of_word1 = (hash_of_word1 * t + (int) *x) % r;
                hash_of_word2 = (hash_of_word2 * b + (int) *x) % r;
            }
            ++x;
        }
        flag = 0;
        cnt_words_now = 0;
        for (int i = 0; i < cnt_of_keywords; ++i) {
            if (keysh[i][2] == 0) flag = 1;
            cnt_words_now += keysh[i][2];
        }
        if (cnt_words_now == cnt_words_bef) {  // Åñëè â ñòðîêå íå áûëî êë. ñëîâ, òî ìèíèìàëüíîå ñëîâî îáíóëÿåì
            min_word[lines][0] = text[lines];
            min_word[lines][1] = text[lines];
        }
        cnt_words_bef = cnt_words_now;
        lines++;
    }
    fclose(f);

    // Block of output
    printf("Output to file or console? (f/c)\n");
    getchar();
    type_of_output = getchar();
    while (type_of_output != 'f' && type_of_output != 'c') {
        printf("Something went wrong! Please enter \'f\' or \'c\':\n");
        type_of_output = getchar();
    }
    if (type_of_output == 'f') {
        printf("Please enter the file name (limit: %i chars):\n", MAXLEN_N);
        scanf("%s", filename);
        f = fopen(filename, "w");
        while (f == NULL) {
            printf("Something went wrong! Please enter the file name again:\n");
            scanf("%s", filename);
            f = fopen(filename, "w");
        }
    } else {
        printf("Processed text:\n");
        f = stdout;
    }

    for (int i = 0; i < lines; ++i) {
        x = text[i];
        while (*x != '\0') {
            if (x == min_word[i][0]) {
                x = min_word[i][1];
                min_word[i][0] = NULL;
            }
            fprintf(f, "%c", *x);
            ++x;
        }
    }
    fclose(f);
    return 0;
}
