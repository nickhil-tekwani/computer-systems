// CS5600-FinalF22-Tekwani.c
// Nickhil Tekwani
// CS5600
// Fall 2022

#include <stdio.h>
#include <stdlib.h>

// note: if there are any errors, main will return 1 and print error to terminal. if it succeeds, it prints the checksum and returns 0.
// assumption: record identifier is an integer

// readme/instructions: make sure this file and the text file (e.g. records.db) are both in the same directory. navigate to directory and run "gcc -o finalexam CS5600-FinalF22-Tekwani.c" Then run "./finalexam records.db"

int main(int argc, char *argv[]) {
    // check that an argument was given
    if (argc < 2) {
        printf("Error: no filename given. \n");
        return 1;
    }

    // open file
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error: could not open file %s.\n", argv[1]);
        return 1;
    }

    // initialize checksum to 0
    int checksum = 0;

    // read each record from the file
    char record[17];
    while (fgets(record, 17, fp)) {
        // check for end of file
        if (record[0] == '#') {
            break;
        }
        // add record identifier to checksum
        checksum += atoi(record);
    }

    // close file
    fclose(fp);

    // print checksum
    printf("%d\n", checksum);

    return 0;
}

