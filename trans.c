/*
    COMPLETE BANK MANAGEMENT SYSTEM
    --------------------------------
    Features:
    1. Create new account
    2. Update account
    3. Delete account
    4. Search account
    5. Display all accounts
    6. Deposit money
    7. Withdraw money
    8. Transfer money
    9. Export accounts to text file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure Definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function Prototypes
unsigned int enterChoice(void);

void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void transferMoney(FILE *fPtr);

int main()
{
    FILE *cfPtr;

    // Open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        // Create file if it does not exist
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created.\n");
            return 1;
        }

        // Initialize 100 blank records
        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient,
                   sizeof(struct clientData),
                   1,
                   cfPtr);
        }
    }

    unsigned int choice;

    while ((choice = enterChoice()) != 10)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            displayAll(cfPtr);
            break;

        case 6:
            searchAccount(cfPtr);
            break;

        case 7:
            depositMoney(cfPtr);
            break;

        case 8:
            withdrawMoney(cfPtr);
            break;

        case 9:
            transferMoney(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    return 0;
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========== BANK MANAGEMENT SYSTEM ==========\n");

    printf("1  - Export accounts to text file\n");
    printf("2  - Update account\n");
    printf("3  - Add new account\n");
    printf("4  - Delete account\n");
    printf("5  - Display all accounts\n");
    printf("6  - Search account\n");
    printf("7  - Deposit money\n");
    printf("8  - Withdraw money\n");
    printf("9  - Transfer money\n");
    printf("10 - Exit\n");

    printf("Enter your choice: ");
    scanf("%u", &choice);

    return choice;
}

// EXPORT TO TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,
            "%-10s%-16s%-12s%-10s\n",
            "Account",
            "Last Name",
            "First Name",
            "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-10u%-16s%-12s%-10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt created successfully.\n");
}

// CREATE NEW ACCOUNT
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter Last Name: ");
        scanf("%14s", client.lastName);

        printf("Enter First Name: ");
        scanf("%9s", client.firstName);

        printf("Enter Initial Balance: ");
        scanf("%lf", &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account created successfully.\n");
    }
}

// UPDATE ACCOUNT
void updateRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int account;
    double transaction;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Current Balance: %.2f\n",
               client.balance);

        printf("Enter amount (+ deposit / - withdraw): ");
        scanf("%lf", &transaction);

        if ((client.balance + transaction) < 0)
        {
            printf("Insufficient balance.\n");
            return;
        }

        client.balance += transaction;

        fseek(fPtr,
              -(long)sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Updated Balance: %.2f\n",
               client.balance);
    }
}

// DELETE ACCOUNT
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// DISPLAY ALL ACCOUNTS
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-10s%-16s%-12s%-10s\n",
           "Account",
           "Last Name",
           "First Name",
           "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-10u%-16s%-12s%-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// SEARCH ACCOUNT
void searchAccount(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Number : %u\n",
               client.acctNum);

        printf("Name           : %s %s\n",
               client.firstName,
               client.lastName);

        printf("Balance        : %.2f\n",
               client.balance);
    }
}

// DEPOSIT MONEY
void depositMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }

    client.balance += amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Deposit successful.\n");
}

// WITHDRAW MONEY
void withdrawMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    if (amount > client.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Withdrawal successful.\n");
}

// TRANSFER MONEY
void transferMoney(FILE *fPtr)
{
    struct clientData sender;
    struct clientData receiver;

    unsigned int fromAcc, toAcc;
    double amount;

    printf("Enter sender account number: ");
    scanf("%u", &fromAcc);

    printf("Enter receiver account number: ");
    scanf("%u", &toAcc);

    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    // Read sender
    fseek(fPtr,
          (fromAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&sender,
          sizeof(struct clientData),
          1,
          fPtr);

    // Read receiver
    fseek(fPtr,
          (toAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&receiver,
          sizeof(struct clientData),
          1,
          fPtr);

    if (sender.acctNum == 0 || receiver.acctNum == 0)
    {
        printf("Invalid account.\n");
        return;
    }

    if (amount > sender.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    // Update balances
    sender.balance -= amount;
    receiver.balance += amount;

    // Write sender
    fseek(fPtr,
          (fromAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&sender,
           sizeof(struct clientData),
           1,
           fPtr);

    // Write receiver
    fseek(fPtr,
          (toAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&receiver,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Transfer successful.\n");
}
