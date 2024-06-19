#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void caesarEncode(char plaintext[], int key) {
	int i = 0;
	while (plaintext[i] != '\0') {
		if (plaintext[i] >= 'A' && plaintext[i]<= 'Z') {
			char c = plaintext[i] - 'A';
			c += key;
			c = c % 26;
			plaintext[i] = c + 'A';
		}
		i++;
	}
	printf("%s", plaintext);
}

void caesarDecode(char ciphertext[], int key) {
  int i = 0;
  while (ciphertext[i] != '\0') {
    if (ciphertext[i] >= 'A' && ciphertext[i]<= 'Z') {
        char c = ciphertext[i] - 'A';
        c -= key;
        c = c % 26;
        ciphertext[i] = c + 'A';
    }
    i++;
  }
  printf("%s", ciphertext);
}

//int main(int argc, char *argv[])
//{
//	int key = atoi(argv[3]);
//
//	if (argc != 4) {
//		printf("please make sure you give the 3 required inputs in order: '-e' or '-d' flag, the string for encryption, and a key.");
//	} else {
//		if (strncmp (argv[1], "-e", 2147483647) == 0) {
//			caesarEncode(argv[2], key);
//		} else if (strncmp (argv[1], "-d", 2147483647) == 0) {
//			caesarDecode(argv[2], key);
//		}
//	}
//	return 0;
//}
