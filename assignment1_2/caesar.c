#include <stdio.h>

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

int main()
{
    char str[] = "HELLO";
//    char strtwo[] = "ROVVY";
    caesarEncode(str, 10);
//    caesarDecode(strtwo, 10);
    return 0;
}
