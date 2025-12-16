#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500
#define KEY 7                 // Caesar + XOR key

// Function to read password from file
void getPassword(char *pass) {
    FILE *fp = fopen("password.txt", "r");

    // If password file doesn't exist → create default password
    if (!fp) {
        fp = fopen("password.txt", "w");
        fprintf(fp, "admin123");
        fclose(fp);
        strcpy(pass, "admin123");
        return;
    }

    fscanf(fp, "%s", pass);
    fclose(fp);
}

// Function to write new password to file
void setPassword(char *newpass) {
    FILE *fp = fopen("password.txt", "w");
    fprintf(fp, "%s", newpass);
    fclose(fp);
}

// Caesar cipher encrypt
void caesar_encrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++)
        text[i] = text[i] + KEY;
}

// Caesar cipher decrypt
void caesar_decrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++)
        text[i] = text[i] - KEY;
}

// XOR encrypt/decrypt
void xor_encrypt_decrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++)
        text[i] ^= KEY;
}

int main() {

    int choice, attempts = 0;
    char inputFile[50], outputFile[50], buffer[MAX];
    char savedPass[50], enteredPass[50], newPass[50];

    while (1) {
        getPassword(savedPass);  // load current password

        printf("\n==============================\n");
        printf(" FILE ENCRYPTION TOOL (C)\n");
        printf("==============================\n");
        printf("1. Encrypt File\n");
        printf("2. Decrypt File\n");
        printf("3. Reset Password\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 4) break;

        // ---------------- RESET PASSWORD ----------------
        if (choice == 3) {
            printf("\nEnter current password: ");
            scanf("%s", enteredPass);

            if (strcmp(enteredPass, savedPass) != 0) {
                printf(" Wrong current password. Cannot reset.\n");
                continue;
            }

            printf("Enter new password: ");
            scanf("%s", newPass);

            setPassword(newPass);
            printf(" Password successfully changed!\n");
            continue;
        }

        printf("Enter input file name: ");
        scanf("%s", inputFile);

        printf("Enter output file name: ");
        scanf("%s", outputFile);

        FILE *fin = fopen(inputFile, "r");
        if (!fin) {
            printf(" Error: Could not open input file.\n");
            continue;
        }

        FILE *fout = fopen(outputFile, "w");
        if (!fout) {
            printf(" Error: Could not create output file.\n");
            fclose(fin);
            continue;
        }

        // ---------------- ENCRYPTION ----------------
        if (choice == 1) {
            printf("\nEncrypting file...\n");

            while (fgets(buffer, MAX, fin)) {
                caesar_encrypt(buffer);
                xor_encrypt_decrypt(buffer);
                fputs(buffer, fout);
            }

            printf(" File Encrypted Successfully → %s\n", outputFile);
        }

        // ---------------- DECRYPTION ----------------
        else if (choice == 2) {

            while (attempts < 3) {
                printf("Enter Password to Decrypt: ");
                scanf("%s", enteredPass);

                if (strcmp(enteredPass, savedPass) == 0) break;

                attempts++;
                printf(" Incorrect Password! Attempts left: %d\n", 3 - attempts);
            }

            if (attempts == 3) {
                printf("\n ACCESS DENIED — Too many incorrect attempts!\n");
                fclose(fin);
                fclose(fout);
                return 0;
            }

            printf("\nDecrypting file...\n");

            while (fgets(buffer, MAX, fin)) {
                xor_encrypt_decrypt(buffer);
                caesar_decrypt(buffer);
                fputs(buffer, fout);
            }

            printf(" File Decrypted Successfully → %s\n", outputFile);
        }

        fclose(fin);
        fclose(fout);
    }

    return 0;
}