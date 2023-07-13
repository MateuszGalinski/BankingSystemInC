#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define NAME_LENGTH 30
#define NAME_FORMAT "%29s"
#define ADDRES_FORMAT "%[^\n]29"

struct cusAcc {
	int accNum;
	char name[NAME_LENGTH];
	char surname[NAME_LENGTH];
	char addres[NAME_LENGTH];
	char pesel[12];
	int regBalance;
	int saveBalance;
};

void initialState();
void addAcc();
struct cusAcc inputAnAcc();
void listAll();
/////////////////////////////////
int searchByAcc(int accSearched, int printFlag);
int searchByPesel(char* peselSearched, int printFlag);
void searchByName(char* nameSearched);
void searchBySurname(char* surnameSearched);
void searchByAddress(char* addresSearched);
int getAccMoney(int accNum, int fromReg);
/////////////////////////////////
int changeAccBalance(int accNum, int moneySum);
void transToDiffAcc(int accNumFrom, int accNumTo, int moneySum);
void transToSub(int accNum, int moneySum);
void transFromSub(int accNum, int moneySum);
/////////////////////////////////
void clearBuffer();
int isName(char* name);
int isPesel(char* pesel);
/////////////////////////////////
void mainMenu(int* end);
void printingSearchOption();
void printingMoneyDeposit();
void printingTransferForTwo();
void printingTransferSub();
/////////////////////////////////
void printAcc(struct cusAcc accToPrint);
struct cusAcc inputName(struct cusAcc inputAcc);
struct cusAcc inputSurname(struct cusAcc inputAcc);
struct cusAcc inputAddres(struct cusAcc inputAcc);
struct cusAcc inputPesel(struct cusAcc inputAcc);
struct cusAcc inputAccBalance(struct cusAcc inputAcc);
struct cusAcc inputSubaccBalance(struct cusAcc inputAcc);
struct cusAcc inputAccNum(struct cusAcc inputAcc);
/////////////////////////////////
void makeBackUp();
void delBackUp();
void listAllBackUp();
int checkIfPropExit();
void loadBackUp();


int main(int argc, char* argv[])
{
	int end;
	initialState();
	makeBackUp();
	listAll();

	while (!end) {
		mainMenu(&end);
	}

	delBackUp();
	return 0;
}

void initialState()
{
	int instruction;
	srand(time(NULL));
	
	if(!checkIfPropExit()){
		printf("Do you want to upload backup?");
		printf("1 - yes, 2 - no\n");
		while (!scanf("%d", &instruction) || instruction > 2 || instruction < 1) {
			printf("incorrect input\n");
			clearBuffer();
		}
	}
	if(instruction == 1){
		loadBackUp();	
	}
}

void addAcc()
{
	FILE* fptr;
	if ((fptr = fopen("clients.bin", "a")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	struct cusAcc accToAdd = inputAnAcc();

	if (accToAdd.accNum == -1) {
		return;
	}

	fwrite(&accToAdd, sizeof(struct cusAcc), 1, fptr);
	makeBackUp();
	fclose(fptr);
}

struct cusAcc inputAnAcc()
{
	FILE* fptr;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening clietn file");
		struct cusAcc nullAcc = {-1, "NULL", "NULL", "NULL", "NULL", 0, 0};
		return nullAcc;
	}

	struct cusAcc inputAcc;

	fseek(fptr, 0L, SEEK_END);
	if (ftell(fptr)) {
		fseek(fptr, -sizeof(struct cusAcc), SEEK_END);
		fread(&inputAcc.accNum, sizeof(int), 1, fptr);
		inputAcc.accNum++;
	}
	else {
		inputAcc.accNum = 1;
	}

	inputAcc = inputName(inputAcc);

	inputAcc = inputSurname(inputAcc);

	inputAcc = inputAddres(inputAcc);

	inputAcc = inputPesel(inputAcc);

	inputAcc = inputAccBalance(inputAcc);

	inputAcc = inputSubaccBalance(inputAcc);

	return inputAcc;
}

void listAll()
{
	system("clear");
	FILE* fptr;
	struct cusAcc listedAcc;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	while (fread(&listedAcc, sizeof(struct cusAcc), 1, fptr)) {
		printAcc(listedAcc);
	}

	printf("\n");

	fclose(fptr);
}

/////////////////////////////////////////////////////////////////////////////////////

int searchByAcc(int accSearched, int printFlag)
{
	FILE* fptr;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening clietn file\n");
		return 0;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (searchedAcc.accNum == accSearched) {
			if (printFlag) {
				printAcc(searchedAcc);
			}
			fclose(fptr);
			return 1;
		}
	}

	if (printFlag) {
		printf("No such account found\n");
	}
	fclose(fptr);
	return 0;
}

int searchByPesel(char* peselSearched, int printFlag)
{
	FILE* fptr;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening clietn file\n");
		return 0;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (!strcmp(searchedAcc.pesel, peselSearched)) {
			if (printFlag) {
				printAcc(searchedAcc);
			}
			fclose(fptr);
			return 1;
		}
	}

	if (printFlag) {
		printf("No such account found\n");
	}
	fclose(fptr);
	return 0;
}

void searchByName(char* nameSearched)
{
	FILE* fptr;
	int foundFlag = 0;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening client file");
		return;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (!strcmp(nameSearched, searchedAcc.name)) {
			printAcc(searchedAcc);
			foundFlag = 1;
		}
	}

	if (!foundFlag) {
		printf("No such account found\n");
	}

	fclose(fptr);
}

void searchBySurname(char* surnameSearched)
{
	FILE* fptr;
	int foundFlag = 0;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening client file");
		return;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (!strcmp(surnameSearched, searchedAcc.surname)) {
			printAcc(searchedAcc);
			foundFlag = 1;
		}
	}

	if (!foundFlag) {
		printf("No such account found\n");
	}

	fclose(fptr);
}

void searchByAddress(char* addresSearched)
{
	FILE* fptr;
	int foundFlag = 0;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening client file");
		return;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (!strcmp(addresSearched, searchedAcc.addres)) {
			printAcc(searchedAcc);
			foundFlag = 1;
		}
	}

	if (!foundFlag) {
		printf("No such account found\n");
	}

	fclose(fptr);
}

int getAccMoney(int accSearched, int fromReg)
{
	FILE* fptr;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening clietn file\n");
		return -1;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (searchedAcc.accNum == accSearched) {
			if (fromReg) {
				fclose(fptr);
				return searchedAcc.regBalance;
			}
			else {
				fclose(fptr);
				return searchedAcc.saveBalance;
			}
		}
	}

	if (!fclose(fptr)) {
		printf("error closing file");
		return -1;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////////

int changeAccBalance(int accNum, int moneySum)
{
	FILE* fptr;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r+")) == NULL) {
		perror("error opening clietn file");
		return 1;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (searchedAcc.accNum == accNum) {
			if (moneySum >= 0) {
				if (moneySum > INT_MAX - moneySum) {
					printf("Errorous ammount of money\n");
					return 1;
				}
			}

			if (moneySum < 0) {
				if (moneySum < INT_MIN - moneySum) {
					printf("Errorous ammount of money\n");
					return 1;
				}
			}

			if (moneySum < 0 && searchedAcc.regBalance + moneySum < 0) {
				printf("can't perform that operation\n");
				return 1;
			}

			searchedAcc.regBalance += moneySum;
			fseek(fptr, -sizeof(struct cusAcc), SEEK_CUR);
			fwrite(&searchedAcc, sizeof(struct cusAcc), 1, fptr);
			makeBackUp();
			fclose(fptr);
			return 0;
		}
	}

	printf("No such account found\n");
	return 1;
	fclose(fptr);
}

void transToDiffAcc(int accNumFrom, int accNumTo, int moneySum)
{
	if (moneySum <= 0) {
		printf("Invalid money sum\n");
		return;
	}
	if (searchByAcc(accNumFrom, 0) && searchByAcc(accNumTo, 0)) {
		if (!changeAccBalance(accNumFrom, -moneySum)) {
			changeAccBalance(accNumTo, moneySum);
			makeBackUp();
			return;
		}
	}
	printf("can't perform operation\n");
}

void transToSub(int accNum, int moneySum)
{
	FILE* fptr;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r+")) == NULL) {
		perror("error opening clietn file\n");
		return;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (searchedAcc.accNum == accNum) {
			if (searchedAcc.regBalance - moneySum < 0 ||
				searchedAcc.saveBalance > INT_MAX - moneySum) {
				printf("can't perfrom that operation\n");
				return;
			}

			searchedAcc.regBalance -= moneySum;
			searchedAcc.saveBalance += moneySum;
			fseek(fptr, -sizeof(struct cusAcc), SEEK_CUR);
			fwrite(&searchedAcc, sizeof(struct cusAcc), 1, fptr);
			makeBackUp();
			fclose(fptr);
			return;
		}
	}

	printf("No such account found\n");
	fclose(fptr);
	return;
}

void transFromSub(int accNum, int moneySum)
{
	FILE* fptr;
	struct cusAcc searchedAcc;
	if ((fptr = fopen("clients.bin", "r+")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	while (fread(&searchedAcc, sizeof(struct cusAcc), 1, fptr)) {
		if (searchedAcc.accNum == accNum) {
			if (searchedAcc.saveBalance - moneySum < 0 ||
				searchedAcc.regBalance > INT_MAX - moneySum) {
				printf("can't perfrom that operation\n");
				return;
			}

			searchedAcc.saveBalance -= moneySum;
			searchedAcc.regBalance += moneySum;
			fseek(fptr, -sizeof(struct cusAcc), SEEK_CUR);
			fwrite(&searchedAcc, sizeof(struct cusAcc), 1, fptr);
			makeBackUp();
			fclose(fptr);
			return;
		}
	}

	printf("No such account found\n");
	fclose(fptr);
	return;
}

void clearBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
	}
}

int isName(char* name)
{
	for (int i = 0; i < strlen(name); i++) {
		if (!isalpha(name[i])) {
			return 0;
		}
	}
	return 1;
}

int isPesel(char* pesel)
{
	if (strlen(pesel) != 11) {
		return 0;
	}
	for (int i = 0; i < strlen(pesel); i++) {
		if (!isdigit(pesel[i])) {
			return 0;
		}
	}
	return 1;
}

void mainMenu(int* end)
{

	printf("To create new account input - 1\n");
	printf("To list all acounts - 2\n");
	printf("To search for account - 3\n");
	printf("To make a deposit/withdrawal - 4\n");
	printf("To transfer money between two accounts- 5\n");
	printf("To transfer money to/from subaccount - 6\n");
	printf("To exit program - 7\n");
	int instruction;
	while (!scanf("%d", &instruction) || instruction > 7 || instruction < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}
	switch (instruction) {
	case 1:
		addAcc();
		break;
	case 2:
		listAll();
		break;
	case 3:
		printingSearchOption();
		break;
	case 4:
		printingMoneyDeposit();
		break;
	case 5:
		printingTransferForTwo();
		break;
	case 6:
		printingTransferSub();
		break;
	case 7:
		*end = 1;
		break;
	default:
		return;
	}
}

void printingSearchOption()
{
	system("clear");
	int instruction = 0;
	struct cusAcc searchedAcc;
	printf("Search by:\n");
	printf("1 - by account number\n");
	printf("2 - by name\n");
	printf("3 - by surname\n");
	printf("4 - by addres\n");
	printf("5 - by pesel\n");
	while (!scanf("%d", &instruction) || instruction > 5 || instruction < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}
	switch (instruction) {
	case 1:
		searchedAcc = inputAccNum(searchedAcc);
		searchByAcc(searchedAcc.accNum, 1);
		break;
	case 2:
		searchedAcc = inputName(searchedAcc);
		searchByName(searchedAcc.name);
		break;
	case 3:
		searchedAcc = inputSurname(searchedAcc);
		searchBySurname(searchedAcc.surname);
		break;
	case 4:
		searchedAcc = inputAddres(searchedAcc);
		searchByAddress(searchedAcc.addres);
		break;
	case 5:
		searchedAcc = inputPesel(searchedAcc);
		searchByPesel(searchedAcc.pesel, 1);
		break;
	}
}

void printingMoneyDeposit()
{
	system("clear");
	int confirmation = 0;
	int instructions = 0;
	int accNum = 0;
	int moneyAmount = 0;
	printf("For deposit enter 1 for withdrawal enter 2\n");
	while (!scanf("%d", &instructions) || instructions > 2 ||
		   instructions < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}
	printf("Enter account number\n");
	while (!scanf("%d", &accNum) || accNum < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}
	printf("Enter amount of money for operation\n");
	while (!scanf("%d", &moneyAmount) || moneyAmount < 1 ||
		   moneyAmount >= INT_MAX) {
		printf("incorrect input\n");
		clearBuffer();
	}
	if (instructions == 2) {
		moneyAmount *= -1;
	}
	if (instructions == 1) {
		printf("Are you sure you want to deposit %d to account %d\n",
			   moneyAmount, accNum);
	}
	else {
		printf("Are you sure you want to withdraw %d from account %d\n",
			   -moneyAmount, accNum);
	}
	printf("If so input 1 if no input 0\n");
	while (!scanf("%d", &confirmation) || confirmation > 1 ||
		   confirmation < 0) {
		printf("incorrect input\n");
		clearBuffer();
	}

	if (confirmation) {
		changeAccBalance(accNum, moneyAmount);
	}
}

void printingTransferForTwo()
{
	system("clear");
	int accNumFrom = 0, accNumTo = 0, moneyAmount = 0, confirmation = 0;
	printf("Enter account number to transfer from\n");
	while (!scanf("%d", &accNumFrom) || accNumFrom < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}

	printf("Enter account number to transfer to\n");
	while (!scanf("%d", &accNumTo) || accNumTo < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}

	printf("Enter amount of money for operation\n");
	while (!scanf("%d", &moneyAmount) || moneyAmount < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}

	printf(
		"Are you sure you want to transfer %d from account %d to account %d\n",
		moneyAmount, accNumFrom, accNumTo);
	printf("If so input 1 if no input 0\n");
	while (!scanf("%d", &confirmation) || confirmation > 1 ||
		   confirmation < 0) {
		printf("incorrect input\n");
		clearBuffer();
	}

	if (confirmation) {
		transToDiffAcc(accNumFrom, accNumTo, moneyAmount);
	}
}

void printingTransferSub()
{
	system("clear");
	int accNum = 0, moneyAmount = 0, instructions = 0, confirmation = 0;
	printf("For savings deposit type 1 for savings withdrawal type 2\n");
	while (!scanf("%d", &instructions) || instructions > 2 ||
		   instructions < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}
	printf("Enter account number\n");
	while (!scanf("%d", &accNum) || accNum < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}
	printf("Enter amount of money for operation\n");
	while (!scanf("%d", &moneyAmount) || moneyAmount < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}

	printf("Are you sure you want to transfer %d\n", moneyAmount);
	printf("If so input 1 if no input 0\n");
	while (!scanf("%d", &confirmation) || confirmation > 1 ||
		   confirmation < 0) {
		printf("incorrect input\n");
		clearBuffer();
	}

	if (confirmation) {
		if (instructions == 1) {
			transToSub(accNum, moneyAmount);
		}
		else {
			transFromSub(accNum, moneyAmount);
		}
	}
}

void printAcc(struct cusAcc accToPrint)
{
	printf("%d;", accToPrint.accNum);
	printf("%s;", accToPrint.name);
	printf("%s;", accToPrint.surname);
	printf("%s;", accToPrint.addres);
	printf("%s;", accToPrint.pesel);
	printf("%d;", accToPrint.regBalance);
	printf("%d\n", accToPrint.saveBalance);
}

struct cusAcc inputName(struct cusAcc inputAcc)
{
	char temp;
	printf("Input name: ");
	while (!scanf(NAME_FORMAT, inputAcc.name) || !isName(inputAcc.name)) {
		printf("incorrect input\n");
		clearBuffer();
	}
	scanf("%c", &temp);
	return inputAcc;
}

struct cusAcc inputSurname(struct cusAcc inputAcc)
{
	char temp;
	printf("Input surname: ");
	while (!scanf(NAME_FORMAT, inputAcc.surname) || !isName(inputAcc.surname)) {
		printf("incorrect input\n");
		clearBuffer();
	}
	scanf("%c", &temp);
	return inputAcc;
}

struct cusAcc inputAddres(struct cusAcc inputAcc)
{
	char temp;
	printf("Input adres: ");
	while (!scanf(ADDRES_FORMAT, inputAcc.addres)) {
		printf("incorrect input\n");
		clearBuffer();
	}
	scanf("%c", &temp);
	return inputAcc;
}

struct cusAcc inputPesel(struct cusAcc inputAcc)
{
	char temp;
	printf("Input pesel: ");
	while (!scanf("%11s", inputAcc.pesel) || !isPesel(inputAcc.pesel) ||
		   searchByPesel(inputAcc.pesel, 0)) {
		printf("incorrect input\n");
		clearBuffer();
	}
	scanf("%c", &temp);
	return inputAcc;
}

struct cusAcc inputAccBalance(struct cusAcc inputAcc)
{
	char temp;
	printf("Input regular balance: ");
	while (!scanf("%d", &inputAcc.regBalance) || inputAcc.regBalance < 0) {
		printf("incorrect input\n");
		clearBuffer();
	}
	scanf("%c", &temp);
	return inputAcc;
}

struct cusAcc inputSubaccBalance(struct cusAcc inputAcc)
{
	char temp;
	printf("Input savings balance: ");
	while (!scanf("%d", &inputAcc.saveBalance) || inputAcc.saveBalance < 0) {
		printf("incorrect input\n");
		clearBuffer();
	}
	scanf("%c", &temp);
	return inputAcc;
}

struct cusAcc inputAccNum(struct cusAcc inputAcc)
{
	char temp;
	printf("Input account number: \n");
	while (!scanf("%d", &inputAcc.accNum) || inputAcc.accNum < 1) {
		printf("incorrect input\n");
		clearBuffer();
	}
	scanf("%c", &temp);
	return inputAcc;
}

void makeBackUp()
{
	FILE* backupFptr;
	FILE* mainFptr;
	struct cusAcc tempAcc;

	if ((backupFptr = fopen("clientstemp.bin", "wb")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	if ((mainFptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	while (fread(&tempAcc, sizeof(struct cusAcc), 1, mainFptr)) {
		fwrite(&tempAcc, sizeof(struct cusAcc), 1, backupFptr);
	}

	fclose(mainFptr);
	fclose(backupFptr);
}

void delBackUp()
{
	remove("clientstemp.bin");
}

void listAllBackUp()
{
	FILE* fptr;
	struct cusAcc listedAcc;
	if ((fptr = fopen("clientstemp.bin", "r")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	while (fread(&listedAcc, sizeof(struct cusAcc), 1, fptr)) {
		printAcc(listedAcc);
	}

	printf("\n");

	fclose(fptr);
}

int checkIfPropExit()
{
	if (fopen("clientstemp.bin", "r") == NULL) {
		printf("Session closed properly\n");
		sleep(1);
		return 1;
	}
	else{
		printf("Last session closed inproperly\n");
		sleep(1);
		return 0;
	}
}

void loadBackUp()
{
	FILE* backupFptr;
	FILE* mainFptr;
	struct cusAcc tempAcc;

	if ((backupFptr = fopen("clientstemp.bin", "wb")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	if ((mainFptr = fopen("clients.bin", "r")) == NULL) {
		perror("error opening clietn file");
		return;
	}

	while (fread(&tempAcc, sizeof(struct cusAcc), 1, backupFptr)) {
		fwrite(&tempAcc, sizeof(struct cusAcc), 1, mainFptr);
	}

	fclose(mainFptr);
	fclose(backupFptr);
}
