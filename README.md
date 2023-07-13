# BankingSystemInC

The code includes several header files such as ctype.h, limits.h, stdio.h, stdlib.h, string.h, time.h, and unistd.h. These headers provide necessary functions and definitions used in the code.

It defines constants like NAME_LENGTH and format specifiers like NAME_FORMAT and ADDRES_FORMAT for formatting input and output.

It defines a structure cusAcc representing a customer account with various fields such as accNum, name, surname, address, pesel, regBalance, and saveBalance.
The code declares several function prototypes used in the program.

The main function serves as the entry point for the program. It initializes the program state, creates backups, lists all accounts, and enters a loop to display the main menu until the user chooses to exit.

The initialState function is responsible for setting the initial state of the program. It checks if a previous session was closed properly and asks the user if they want to upload a backup.

The addAcc function allows the user to add a new customer account by inputting various details such as name, surname, address, and balance. The account is then written to a file named "clients.bin".

The inputAnAcc function prompts the user to input details for a new customer account and returns a cusAcc structure with the entered values.

The listAll function lists all the customer accounts stored in the "clients.bin" file.

There are several search functions (searchByAcc, searchByPesel, searchByName, searchBySurname, searchByAddress) that search for customer accounts based on different criteria and display the results.

The getAccMoney function retrieves the balance of a customer account based on the account number and account type (regular or savings).

The changeAccBalance function changes the balance of a customer account based on the account number and a specified amount. It performs deposit or withdrawal operations and updates the account balance.

The transToDiffAcc function transfers money between two customer accounts based on the account numbers and a specified amount. It calls the changeAccBalance function internally.

The transToSub and transFromSub functions transfer money to or from a subaccount associated with a customer account.
There are several utility functions for input validation, clearing the input buffer, printing menus and account details, and inputting specific details of a customer account (inputName, inputSurname, inputAddres, inputPesel, inputAccBalance, inputSubaccBalance, inputAccNum).

The code also includes functions for creating and deleting backups (makeBackUp, delBackUp, listAllBackUp), checking if a previous session was closed properly (checkIfPropExit), and loading a backup (loadBackUp).

Overall, this code provides a basic command-line interface for managing customer accounts, performing transactions, and maintaining backups.
