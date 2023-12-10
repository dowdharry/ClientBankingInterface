/*
	Client Banking Interface
	December 2023
	William H. Dowd

	This program illustrates the use of classes, input validation, file manipulation, documentation
	and other important fundamentals of coding.

	The program is a banking interface to be used by multiple clients to create saving and checking accounts,
	withdraw and deposit from those accounts, and other various actions. The program supports multiple clients
	by saving data to multiple text files to be read from the program as needed.

	Bugs:
		Input validation fails to prevent inputs such as "1 0" for int, and passes the input through as "1"

	Improvements:
		Password hashing
		Input validation using regex for user profile creation
*/

#include "UserClasses.h"
#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <sstream>

#pragma region Function prototypes: Not logged in
int welcome();
bool login(customer& _user, std::fstream& _userData, std::fstream& _userAccountData, checkingAccount& _userChecking, savingAccount& _userSaving);
void signUp(std::fstream& _userData);
#pragma endregion

#pragma region Function prototypes: Logged in
int options(customer _user);
void createAccount(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData);
void displayAccount(checkingAccount _userChecking, savingAccount _userSaving);
void withdrawAccount(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData);
void depositAccount(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData);
bool logout(customer& _user, checkingAccount& _userChecking, savingAccount& _userSaving);
#pragma endregion

#pragma region Function prototypes: Other functions
bool loadUser(customer& _user, std::fstream& _userData, std::string _passwordInput, std::string _usernameInput);
void loadAccounts(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData);
std::string inputPasswd(bool& _valid);
bool userExistance(std::fstream& _userData, std::string _usernameInput);
bool savingExistance(std::fstream& _userAccountData, customer _user);
bool checkingExistance(std::fstream& _userAccountData, customer _user);
void createSaving(std::fstream& _userAccountData, customer _user, savingAccount& _userSaving);
void createChecking(std::fstream& _userAccountData, customer _user, checkingAccount& _userChecking);
void updateSavingAccount(customer _user, savingAccount _userSaving, std::fstream& _userAccountData);
void updateCheckingAccount(customer _user, checkingAccount _userChecking, std::fstream& _userAccountData);
#pragma endregion

int main()
{
	/*
		The main function displays menus available to the user depending on if that
		user is logged into a profile or not. The welcome menu is displayed to users not logged in
		and the options menu is diplayed to users logged in. Both menus return an int to the navigator variable.
		The navigator variable indicates through a switch case statement which function to run.

		The login and logout functions return true/false to the currentlyLogged variable as well as loading/unloading data.

		The exit variable will exit the do while loop that keeps the program running if the user selects 3 from the welcome menu.

		user, userChecking, and userSaving are the classes that are loaded/unloaded with data when logged in or logged out.

		userData and userAccountData are the file streams for storing and retrieving data from text files.
	*/

	int navigator = NULL;
	bool currentlyLogged = false;
	bool exit = false;

	customer user;
	checkingAccount userChecking;
	savingAccount userSaving;

	std::fstream userData;
	std::fstream userAccountData;

	do
	{
		if (currentlyLogged == false)
		{
			navigator = welcome();
			switch (navigator)
			{
			case 1:
				currentlyLogged = login(user, userData, userAccountData, userChecking, userSaving);
				break;
			case 2:
				signUp(userData);
				break;
			case 3:
				exit = true;
				break;
			}
		}
		else if (currentlyLogged == true)
		{
			navigator = options(user);
			switch (navigator)
			{
			case 1:
				createAccount(user, userChecking, userSaving, userAccountData);
				break;
			case 2:
				displayAccount(userChecking, userSaving);
				break;
			case 3:
				withdrawAccount(user, userChecking, userSaving, userData);
				break;
			case 4:
				depositAccount(user, userChecking, userSaving, userData);
				break;
			case 5:
				currentlyLogged = logout(user, userChecking, userSaving);
				break;
			}
		}
	} while (exit == false);
	return 0;
}

#pragma region Functions: Not logged in 
int welcome()
{
	/*
		Asks the user for a number from 1-3 in a while loop.
		Uses stringstream to validate that the input is an integer,
		If it is, break the loop and return the integer.

		Note: checking if the valid integer is 1, 2, or 3 is not required because
		the welcome function will loop again if the navigator variable does not
		lead to a function in the switch case statement.
	*/

	int tempNav;
	while (true)
	{
		std::string input;
		std::stringstream ss;

		system("cls");
		std::cout << "Welcome to the Project Bank\n\n";
		std::cout << "[1] Login\n";
		std::cout << "[2] Create Profile\n";
		std::cout << "[3] Exit Program\n\n";
		std::cout << "Select: ";
		getline(std::cin, input);
		ss << input;

		if (ss >> tempNav)
		{
			break;
		}
	}
	return tempNav;
}

bool login(customer& _user, std::fstream& _userData, std::fstream& _userAccountData, checkingAccount& _userChecking, savingAccount& _userSaving)
{
	/*
		Asks the user for a username and password.

		Calls loadUser which checks if the inputs match records.
			if there's a match, return true and load user data
				call loadAccounts to load user account data if found
					if a saving account is loaded, pay interest
			if there's no match, return false

		Function returns true or false depending on whether user data was loaded or not

		Parameters:
			_user: Passed by reference to allow loading user data
			_userData: (File streams must be passed by ref)
			_userAccountData: (File streams must be passed by ref)
			_userChecking: Passed by reference to allow loading user account data
			_userSaving: Passed by reference to allow loading user account data
	*/

	std::string usernameInput, passwordInput;
	bool loginSuccess = false;

	system("cls");
	std::cout << "Username: ";
	getline(std::cin, usernameInput);
	std::cout << "password: ";
	getline(std::cin, passwordInput);

	loginSuccess = loadUser(_user, _userData, passwordInput, usernameInput);

	if (loginSuccess == true)
	{
		loadAccounts(_user, _userChecking, _userSaving, _userAccountData);
		if (_userSaving.getid() != 0.0)
		{
			_userSaving.payInterest();
			updateSavingAccount(_user, _userSaving, _userAccountData);
		}
	}
	else if (loginSuccess == false)
	{
		std::cout << "\nLogin failed\n\n";
		system("pause");
	}
	return loginSuccess;
}

void signUp(std::fstream& _userData)
{
	/*
		Asks the user to enter general information in a loop until the information is not blank
		Calls inputPasswd in a loop until inputPasswd returns a valid password string
		Calls userExistance to check if the username input already belongs to a profile
			if it does, return true
			else false
		Loads profile into userData.txt if userExistance returns false

		Parameters:
			_userData: (File streams must be passed by ref) Stores created profile data into _userData file stream.
	*/

	std::string firstName, lastName, address, phoneNum, password, email, usernameInput;
	bool validPasswd = false, emptyField = false, userExists;

	do
	{
		system("cls");
		if (emptyField == true)
		{
			std::cout << "Fields cannot be blank\n\n";
		}
		std::cout << "Enter first name: ";
		getline(std::cin, firstName);
		std::cout << "Enter last name: ";
		getline(std::cin, lastName);
		std::cout << "Enter address: ";
		getline(std::cin, address);
		std::cout << "Enter phone number: ";
		getline(std::cin, phoneNum);
		std::cout << "Enter email: ";
		getline(std::cin, email);
		std::cout << "Enter username: ";
		getline(std::cin, usernameInput);

		if (firstName == "" || lastName == "" || address == "" || phoneNum == "" || email == "" || usernameInput == "")
		{
			emptyField = true;
		}
		else
		{
			emptyField = false;
		}

	} while (emptyField == true);


	do
	{
		password = inputPasswd(validPasswd);
	} while (validPasswd == false);

	userExists = userExistance(_userData, usernameInput);

	if (userExists == false)
	{
		_userData.open("userData.txt", std::ios::app);

		_userData << usernameInput;
		_userData << ":" << firstName;
		_userData << ":" << lastName;
		_userData << ":" << address;
		_userData << ":" << phoneNum;
		_userData << ":" << email;
		_userData << ":" << password << ":\n";

		_userData.close();
		std::cout << "User profile created\n\n";
	}
	else
	{
		std::cout << "Profile already exists\n\n";
	}
	system("pause");
}
#pragma endregion

#pragma region Functions: Logged in
int options(customer _user)
{
	/*
		Asks the user for a number from 1-5 in a while loop.
		Uses stringstream to validate that the input is an integer,
		If it is, break the loop and return the integer.

		Note: checking if the valid integer is 1-5 is not required because
		the options function will loop again if the navigator variable does not
		lead to a function in the switch case statement.
	*/

	int tempNav;
	while (true)
	{
		std::string input;
		std::stringstream ss;

		system("cls");
		std::cout << "Welcome " << _user.getfirstName() << "\n\n";
		std::cout << "[1] Create checking or saving account\n";
		std::cout << "[2] Display an account\n";
		std::cout << "[3] Withdraw from account\n";
		std::cout << "[4] Deposit to account\n";
		std::cout << "[5] Logout\n\n";
		std::cout << "Select: ";
		getline(std::cin, input);
		ss << input;

		if (ss >> tempNav)
		{
			break;
		}
	}
	return tempNav;
}

void createAccount(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData)
{
	/*
		checks for existance of accounts and returns true/false to corresponding variables

		Asks the user for a number from 1-3 in a while loop.
		Uses stringstream to validate that the input is an integer,
		If it is, break the loop.

		Note: checking if the valid integer is 1-3 is not required because
		the function will loop again if the navigator variable does not
		lead to a valid switch case statement.

		Case 1:
			If checking does not exist, call createChecking to create one

		Case 2:
			If saving does not exist, call createSaving to create one

		Case 3:
			Exits function by breaking do while loop

		Parameters:
			_user: for reading user data
			_userAccountData: (File streams must be passed by ref) for storing created account data
			_userChecking: Passed by reference to allow creating user account data
			_userSaving: Passed by reference to allow creating user account data
	*/

	int tempNav;
	bool tempExit = false, savingExists = false, checkingExists = false;

	savingExists = savingExistance(_userAccountData, _user);
	checkingExists = checkingExistance(_userAccountData, _user);

	do
	{
		while (true)
		{
			std::string input;
			std::stringstream ss;
			system("cls");
			std::cout << "[1] Create checking account\n";
			std::cout << "[2] Create saving account\n";
			std::cout << "[3] Cancel\n\n";
			std::cout << "Select: ";
			getline(std::cin, input);
			ss << input;

			if (ss >> tempNav)
			{
				break;
			}
		}

		switch (tempNav)
		{
		case 1:
			if (checkingExists == false)
			{
				system("cls");
				createChecking(_userAccountData, _user, _userChecking);
				std::cout << "Checking account created\n\n";
				tempExit = true;
			}
			else
			{
				system("cls");
				std::cout << "Checking account already created\n\n";
			}
			system("pause");
			break;

		case 2:
			if (savingExists == false)
			{
				system("cls");
				createSaving(_userAccountData, _user, _userSaving);
				std::cout << "Saving account created\n\n";
				tempExit = true;
			}
			else
			{
				system("cls");
				std::cout << "Saving account already created\n\n";
			}
			system("pause");
			break;

		case 3:
			tempExit = true;
			break;
		}
	} while (tempExit == false);
}

void displayAccount(checkingAccount _userChecking, savingAccount _userSaving)
{
	/*
		Asks the user for a number from 1-3 in a while loop.
		Uses stringstream to validate that the input is an integer,
		If it is, break the loop.

		Note: checking if the valid integer is 1-3 is not required because
		the function will loop again if the navigator variable does not
		lead to a valid switch case statement.

		Case 1:
			If checking exists and is loaded, print checking info

		Case 2:
			If saving exists and is loaded, print saving info

		Case 3:
			Exits function by breaking do while loop

		Parameters:
			_userChecking: Passed by value since data is only being read/output
			_userSaving: Passed by value since data is only being read/output
	*/

	int tempNav;
	bool tempExit = false;

	do
	{
		while (true)
		{
			std::string input;
			std::stringstream ss;
			system("cls");
			std::cout << "[1] Display checking account\n";
			std::cout << "[2] Display saving account\n";
			std::cout << "[3] Cancel\n\n";
			std::cout << "Select: ";
			getline(std::cin, input);
			ss << input;

			if (ss >> tempNav)
			{
				break;
			}
		}

		switch (tempNav)
		{
		case 1:
			if (_userChecking.getid() == 0)
			{
				system("cls");
				std::cout << "No checking account found\n\n";
			}
			else
			{
				system("cls");
				_userChecking.printInfo();
				tempExit = true;
			}
			system("pause");
			break;

		case 2:
			if (_userSaving.getid() == 0)
			{
				system("cls");
				std::cout << "No saving account found\n\n";
			}
			else
			{
				system("cls");
				_userSaving.printInfo();
				tempExit = true;
			}
			system("pause");
			break;

		case 3:
			tempExit = true;
			break;
		}
	} while (tempExit == false);
}

void withdrawAccount(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData)
{
	/*
		Asks the user for a number from 1-3 in a while loop.
		Uses stringstream to validate that the input is an integer,
		If it is, break the loop.

		Note: checking if the valid integer is 1-3 is not required because
		the function will loop again if the navigator variable does not
		lead to a valid switch case statement.

		If the user inputs 1 or 2 (indicating they would like to make a withdrawal),
		ask the user for a float and validate it is a float using a stringstream

		Case 1:
			If checking exists and is loaded
				if the balance minus the withdrawal amount is equal to or greater than 0 and the withdrawal amount is greater than 0
					withdrawal the money and update accountData by calling updateCheckingAccount
				if the balance minus the withdrawal amount is equal to or greater than the overdraft limit and the withdrawal amount is greater than 0
					withdrawal the money and subtract a overdraft fee from the balance and update accountData by calling updateCheckingAccount
				if withdrawal amount is equal to or less than 0
					output error
				else
					output insufficient balance
		Case 2:
			If saving exists and is loaded
				if the balance minus the withdrawal amount is equal to or greater than 0 and the withdrawal amount is greater than 0
					withdrawal the money and update accountData by calling updateSavingAccount
				if withdrawal amount is equal to or less than 0
					output error
				else
					output insufficient balance
		Case 3:
			Exits function by breaking do while loop

		Parameters:
			_user: Passed by value since data from _user is not being modified
			_userAccountData: (File streams must be passed by ref) for modifying stored account data
			_userChecking: Passed by reference to allow modifying user account data
			_userSaving: Passed by reference to allow modifying user account data
	*/

	int tempNav;
	bool tempExit = false;

	do
	{
		float withAmount = 0;
		while (true)
		{
			std::string input;
			std::stringstream ss;
			system("cls");
			std::cout << "[1] Withdraw from checking account\n";
			std::cout << "[2] Withdraw from saving account\n";
			std::cout << "[3] Cancel\n\n";
			std::cout << "Select: ";
			getline(std::cin, input);
			ss << input;

			if (ss >> tempNav)
			{
				break;
			}
		}

		while (tempNav == 1 || tempNav == 2)
		{
			std::string input;
			std::stringstream ss;
			system("cls");
			std::cout << "Enter withdrawal amount: ";
			getline(std::cin, input);
			ss << input;

			if (ss >> withAmount)
			{
				break;
			}
		}

		switch (tempNav)
		{
		case 1:
			if (_userChecking.getid() == 0)
			{
				system("cls");
				std::cout << "No checking account found\n\n";
			}
			else
			{
				if ((_userChecking.getbalance() - withAmount) >= 0 && (withAmount > 0))
				{
					_userChecking.withdrawal(withAmount);
					system("cls");
					updateCheckingAccount(_user, _userChecking, _userAccountData);
					std::cout << "Successful withdrawal of " << withAmount << "\n";
					std::cout << "New balance:  " << _userChecking.getbalance() << "\n\n";
					tempExit = true;
				}
				else if ((_userChecking.getbalance() - withAmount) >= (0 - _userChecking.getoverdraftLimit()) && (withAmount > 0))
				{
					system("cls");
					_userChecking.withdrawal(withAmount);;
					_userChecking.setbalance(_userChecking.getbalance() - 5);
					updateCheckingAccount(_user, _userChecking, _userAccountData);
					std::cout << "Successful withdrawal of " << withAmount << "\n";
					std::cout << "Service fee of $5 charged" << "\n";
					std::cout << "New balance:  " << _userChecking.getbalance() << "\n\n";
					tempExit = true;
				}
				else if (withAmount <= 0)
				{
					system("cls");
					std::cout << "Withdrawal must be greater than 0$\n\n";
				}
				else
				{
					system("cls");
					std::cout << "Insufficient Balance\n";
				}
			}
			system("pause");
			break;

		case 2:
			if (_userSaving.getid() == 0)
			{
				system("cls");
				std::cout << "No saving account found\n\n";
			}
			else
			{
				if ((_userSaving.getbalance() - withAmount) >= 0 && (withAmount > 0))
				{
					_userSaving.withdrawal(withAmount);
					system("cls");
					updateSavingAccount(_user, _userSaving, _userAccountData);
					std::cout << "Successful withdrawal of " << withAmount << "\n";
					std::cout << "New balance:  " << _userSaving.getbalance() << "\n\n";
					tempExit = true;
				}
				else if (withAmount <= 0)
				{
					system("cls");
					std::cout << "Withdrawal must be greater than 0$\n\n";
				}
				else
				{
					system("cls");
					std::cout << "Insufficient Balance\n\n";
				}
			}
			system("pause");
			break;

		case 3:
			tempExit = true;
			break;
		}
	} while (tempExit == false);
}

void depositAccount(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData)
{
	/*
		Asks the user for a number from 1-3 in a while loop.
		Uses stringstream to validate that the input is an integer,
		If it is, break the loop.

		Note: checking if the valid integer is 1-3 is not required because
		the function will loop again if the navigator variable does not
		lead to a valid switch case statement.

		If the user inputs 1 or 2 (indicating they would like to make a deposit),
		ask the user for a float and validate it is a float using a stringstream

		Case 1:
			If checking exists and is loaded
				if the deposit is greater than 0
					deposit the money and update accountData by calling updatCheckingAccount
				if deposit amount is equal to or less than 0
					output error
		Case 2:
			If saving exists and is loaded
				if the deposit is greater than 0
					deposit the money and update accountData by calling updateSavingAccount
				if deposit amount is equal to or less than 0
					output error
		Case 3:
			Exits function by breaking do while loop

		Parameters:
			_user: Passed by value since data from _user is not being modified
			_userAccountData: (File streams must be passed by ref) for modifying stored account data
			_userChecking: Passed by reference to allow modifying user account data
			_userSaving: Passed by reference to allow modifying user account data
	*/

	int tempNav;
	bool tempExit = false;

	do
	{
		float depAmount = 0;
		while (true)
		{
			std::string input;
			std::stringstream ss;
			system("cls");
			std::cout << "[1] Deposit into checking account\n";
			std::cout << "[2] Deposit into saving accoun\n";
			std::cout << "[3] Cancel\n\n";
			std::cout << "Select: ";
			getline(std::cin, input);
			ss << input;

			if (ss >> tempNav)
			{
				break;
			}
		}

		while (tempNav == 1 || tempNav == 2)
		{
			std::string input;
			std::stringstream ss;
			system("cls");
			std::cout << "Enter deposit amount: ";
			getline(std::cin, input);
			ss << input;

			if (ss >> depAmount)
			{
				break;
			}
		}

		switch (tempNav)
		{
		case 1:
			if (_userChecking.getid() == 0)
			{
				system("cls");
				std::cout << "No checking account found\n\n";
			}
			else
			{
				if (depAmount > 0)
				{
					_userChecking.deposit(depAmount);
					system("cls");
					updateCheckingAccount(_user, _userChecking, _userAccountData);
					std::cout << "Successful deposit of " << depAmount << "\n";
					std::cout << "New balance:  " << _userChecking.getbalance() << "\n\n";
					tempExit = true;
				}
				else
				{
					system("cls");
					std::cout << "Must deposit greater than $0\n\n";
				}
			}
			system("pause");
			break;

		case 2:
			if (_userSaving.getid() == 0)
			{
				system("cls");
				std::cout << "No saving account found\n\n";
			}
			else
			{
				if (depAmount > 0)
				{
					_userSaving.deposit(depAmount);
					system("cls");
					updateSavingAccount(_user, _userSaving, _userAccountData);
					std::cout << "Successful deposit of " << depAmount << "\n";
					std::cout << "New balance:  " << _userSaving.getbalance() << "\n\n";
					tempExit = true;
				}
				else
				{
					system("cls");
					std::cout << "Must deposit greater than $0\n\n";
				}
			}
			system("pause");
			break;

		case 3:
			tempExit = true;
			break;
		}
	} while (tempExit == false);
}

bool logout(customer& _user, checkingAccount& _userChecking, savingAccount& _userSaving)
{
	/*
		Clears _user, _userChecking, and _userSaving and returns true

		Parameters:
			_user: Passed by reference to allow modifying user data
			_userChecking: Passed by reference to allow modifying user account data
			_userSaving: Passed by reference to allow modifying user account data
	*/

	_user.setusername("");
	_user.setfirstName("");
	_user.setlastName("");
	_user.setaddress("");
	_user.setphoneNum("");
	_user.setemail("");

	_userChecking.setaccountCustomer(customer());
	_userChecking.setbalance(0.0);
	_userChecking.setoverdraftLimit(0.0);
	_userChecking.setid(0);
	_userChecking.setdepositCount(0);
	_userChecking.setwithdrawalCount(0);

	_userSaving.setaccountCustomer(customer());
	_userSaving.setbalance(0.0);
	_userSaving.setinterestRate(0.0);
	_userSaving.setid(0);
	_userSaving.setdepositCount(0);
	_userSaving.setwithdrawalCount(0);
	return false;
}
#pragma endregion

#pragma region otherFunctions
bool loadUser(customer& _user, std::fstream& _userData, std::string _passwordInput, std::string _usernameInput)
{
	/*
		Called from login()

		Searchs userData.txt for a user with a username and password that matches input passed by value from login()

		By default, the program returns false.
		The function returns true and user data is loaded if a match is found.
	*/

	bool loginSuccess = false;

	_userData.open("userData.txt", std::ios::in);
	std::string line;
	while (std::getline(_userData, line))
	{
		std::stringstream ss(line);
		std::string firstName, lastName, address, phoneNum, email, password, username;

		std::getline(ss, username, ':');
		std::getline(ss, firstName, ':');
		std::getline(ss, lastName, ':');
		std::getline(ss, address, ':');
		std::getline(ss, phoneNum, ':');
		std::getline(ss, email, ':');
		std::getline(ss, password, ':');

		if ((_usernameInput == username) && (_passwordInput == password) && (_usernameInput != "") && (_passwordInput != ""))
		{
			_user.setusername(username);
			_user.setfirstName(firstName);
			_user.setlastName(lastName);
			_user.setaddress(address);
			_user.setphoneNum(phoneNum);
			_user.setemail(email);

			std::cout << std::endl << "Succesfully logged in\n\n";
			loginSuccess = true;
			system("pause");
			break;
		}
	}
	_userData.close();
	return loginSuccess;
}

void loadAccounts(customer _user, checkingAccount& _userChecking, savingAccount& _userSaving, std::fstream& _userAccountData)
{
	/*
		Called from login()

		Searchs userAccountData.txt for accounts with a matching username to the loaded user and loads those accounts

		When matches are found they are checked to see if they are saving or checking accounts,
		then the data is converted from how it was read (strings) to the correct data type using string streams
		finally the data is loaded into either _userSaving or _userChecking respectively.
	*/

	_userAccountData.open("userAccountData.txt", std::ios::in);
	std::string line;
	while (std::getline(_userAccountData, line))
	{
		std::stringstream ss(line);
		std::string accountType;
		std::string usernameAccount;

		std::getline(ss, accountType, ':');
		std::getline(ss, usernameAccount, ':');

		if ((_user.getusername() == usernameAccount) && (accountType == "Saving"))
		{
			std::string balanceString, interestRateString, idString, depositCountString, withdrawalCountString;
			float balance, interestRate;
			int id, depositCount, withdrawalCount;

			std::getline(ss, balanceString, ':');
			std::getline(ss, interestRateString, ':');
			std::getline(ss, idString, ':');
			std::getline(ss, depositCountString, ':');
			std::getline(ss, withdrawalCountString, ':');

			std::stringstream balanceConv, interestRateConv, idConv, DepositCountConv, WithdrawalCountConv;

			balanceConv << balanceString;
			balanceConv >> balance;
			interestRateConv << interestRateString;
			interestRateConv >> interestRate;
			idConv << idString;
			idConv >> id;
			DepositCountConv << depositCountString;
			DepositCountConv >> depositCount;
			WithdrawalCountConv << withdrawalCountString;
			WithdrawalCountConv >> withdrawalCount;

			_userSaving.setaccountCustomer(_user);
			_userSaving.setbalance(balance);
			_userSaving.setinterestRate(interestRate);
			_userSaving.setid(id);
			_userSaving.setdepositCount(depositCount);
			_userSaving.setwithdrawalCount(withdrawalCount);
		}
		else if ((_user.getusername() == usernameAccount) && (accountType == "Checking"))
		{
			std::string balanceString, overdraftLimitString, idString, depositCountString, withdrawalCountString;
			float balance, overdraftLimit;
			int id, depositCount, withdrawalCount;

			std::getline(ss, balanceString, ':');
			std::getline(ss, overdraftLimitString, ':');
			std::getline(ss, idString, ':');
			std::getline(ss, depositCountString, ':');
			std::getline(ss, withdrawalCountString, ':');

			std::stringstream balanceConv, overdraftLimitConv, idConv, DepositCountConv, WithdrawalCountConv;

			balanceConv << balanceString;
			balanceConv >> balance;
			overdraftLimitConv << overdraftLimitString;
			overdraftLimitConv >> overdraftLimit;
			idConv << idString;
			idConv >> id;
			DepositCountConv << depositCountString;
			DepositCountConv >> depositCount;
			WithdrawalCountConv << withdrawalCountString;
			WithdrawalCountConv >> withdrawalCount;

			_userChecking.setaccountCustomer(_user);
			_userChecking.setbalance(balance);
			_userChecking.setoverdraftLimit(overdraftLimit);
			_userChecking.setid(id);
			_userChecking.setdepositCount(depositCount);
			_userChecking.setwithdrawalCount(withdrawalCount);
		}
	}
	_userAccountData.close();
}

std::string inputPasswd(bool& _valid)
{
	/*
		Called by signUp()

		Asks the user to enter a password that matches certain requirements

		check that the requirements are met by setting requirement bools to true if they are met.
		any requirements not met output an error to the user

		if all requirements are met set _valid to true

		returns the input password
	*/

	std::string password, password2;
	bool hasUpper = false, hasSpecial = false, hasLower = false, hasNum = false, hasEight = false, match = false;

	system("cls");
	std::cout << "Create password \n";
	std::cout << "Minimum of 8 characters\n";
	std::cout << "At least one uppercase letter\n";
	std::cout << "At Least one lowercase letter\n";
	std::cout << "At least one number\n";
	std::cout << "At least one special character\n\n";
	std::cout << "Enter password: ";
	getline(std::cin, password);
	std::cout << "Re-enter password : ";
	getline(std::cin, password2);

	if (password == password2)
	{
		match = true;
	}

	if (password.length() >= 8) {
		hasEight = true;
	}

	for (int i = 0; i < password.length(); i++)
	{
		if (islower(password[i]))
		{
			hasLower = true;
		}
		else if (isupper(password[i]))
		{
			hasUpper = true;
		}
		else if (isdigit(password[i]))
		{
			hasNum = true;
		}
		else {
			hasSpecial = true;
		}
	}

	std::cout << std::endl;
	if (match == false)
	{
		std::cout << "Passwords do not match\n";
	}
	if (hasEight == false)
	{
		std::cout << "Password needs 8 characters.\n";
	}
	if (hasUpper == false)
	{
		std::cout << "Password needs an uppercase character.\n";
	}
	if (hasLower == false)
	{
		std::cout << "Password needs a lowercase character.\n";
	}
	if (hasNum == false)
	{
		std::cout << "Password needs a number.\n";
	}
	if (hasSpecial == false)
	{
		std::cout << "Password needs a special character.\n";
	}


	if (hasEight == false || hasUpper == false || hasLower == false || hasNum == false || hasSpecial == false || match == false)
	{
		std::cout << "\n";
		system("pause");
	}

	if (hasEight == true && hasUpper == true && hasLower == true && hasNum == true && hasSpecial == true && match == true)
	{
		_valid = true;
	}
	return password;
}

bool userExistance(std::fstream& _userData, std::string _usernameInput)
{
	/*
		Called by singUp()

		Checks userData.txt for a username matching the input username

		sets userExists to true if a match is found (false is the default)

		returns userExists
	*/

	bool userExists = false;

	_userData.open("userData.txt", std::ios::in);
	std::string line;
	while (std::getline(_userData, line))
	{
		std::stringstream ss(line);
		std::string username;

		std::getline(ss, username, ':');
		if (_usernameInput == username)
		{
			userExists = true;
			break;
		}
	}
	_userData.close();
	return userExists;
}

bool savingExistance(std::fstream& _userAccountData, customer _user)
{
	/*
		Called by createAccount()

		Checks userAccountData.txt for a saving account with the loaded _user username

		sets exists to true if a match is found (false is the default)

		returns exists
	*/

	bool exists = false;

	_userAccountData.open("userAccountData.txt", std::ios::in);
	std::string line;
	while (std::getline(_userAccountData, line))
	{
		std::stringstream ss(line);
		std::string accountType;
		std::string usernameAccount;

		std::getline(ss, accountType, ':');
		std::getline(ss, usernameAccount, ':');
		if ((_user.getusername() == usernameAccount) && (accountType == "Saving"))
		{
			exists = true;
		}
	}
	_userAccountData.close();

	return exists;
}

bool checkingExistance(std::fstream& _userAccountData, customer _user)
{
	/*
		Called by createAccount()

		Checks userAccountData.txt for a checking account with the loaded _user username

		sets exists to true if a match is found (false is the default)

		returns exists
	*/

	bool exists = false;

	_userAccountData.open("userAccountData.txt", std::ios::in);
	std::string line;
	while (std::getline(_userAccountData, line))
	{
		std::stringstream ss(line);
		std::string accountType;
		std::string usernameAccount;

		std::getline(ss, accountType, ':');
		std::getline(ss, usernameAccount, ':');
		if ((_user.getusername() == usernameAccount) && (accountType == "Checking"))
		{
			exists = true;
		}
	}
	_userAccountData.close();

	return exists;
}

void createSaving(std::fstream& _userAccountData, customer _user, savingAccount& _userSaving)
{
	/*
		Called by createAccount()

		uses mersenne twister random algorithm with a random device generated seed to generate a random id number between 100000 to 999999

		sets account values with default values and random ID

		stores account into userAccoundData.txt
	*/

	std::mt19937 rndEngine{ std::random_device{}() };
	std::uniform_int_distribution<> uniformDist(100000, 999999);
	int id = uniformDist(rndEngine);

	_userSaving.setaccountCustomer(_user);
	_userSaving.setbalance(0);
	_userSaving.setinterestRate(.05);
	_userSaving.setid(id);
	_userSaving.setdepositCount(0);
	_userSaving.setwithdrawalCount(0);

	_userAccountData.open("userAccountData.txt", std::ios::app);
	_userAccountData << "Saving";
	_userAccountData << ":" << _user.getusername();
	_userAccountData << ":" << 0;
	_userAccountData << ":" << .05;
	_userAccountData << ":" << id;
	_userAccountData << ":" << 0;
	_userAccountData << ":" << 0 << ":\n";
	_userAccountData.close();
}

void createChecking(std::fstream& _userAccountData, customer _user, checkingAccount& _userChecking)
{
	/*
		Called by createAccount()

		uses mersenne twister random algorithm with a random device generated seed to generate a random id number between 100000 to 999999

		sets account values with default values and random ID

		stores account into userAccoundData.txt
	*/

	std::mt19937 rndEngine{ std::random_device{}() };
	std::uniform_int_distribution<> uniformDist(100000, 999999);
	int id = uniformDist(rndEngine);

	_userChecking.setaccountCustomer(_user);
	_userChecking.setbalance(0);
	_userChecking.setoverdraftLimit(25);
	_userChecking.setid(id);
	_userChecking.setdepositCount(0);
	_userChecking.setwithdrawalCount(0);

	_userAccountData.open("userAccountData.txt", std::ios::app);
	_userAccountData << "Checking";
	_userAccountData << ":" << _user.getusername();
	_userAccountData << ":" << 0;
	_userAccountData << ":" << 25;
	_userAccountData << ":" << id;
	_userAccountData << ":" << 0;
	_userAccountData << ":" << 0 << ":\n";
	_userAccountData.close();
}

void updateSavingAccount(customer _user, savingAccount _userSaving, std::fstream& _userAccountData)
{
	/*
		Opens userAccountData.txt and newUserAccountData.txt
		Writes data from userAccountData.txt to newUserAccountData.txt
		if the line being read from userAccountData.txt contains the saving account data of the current user
			don't write it to newUserAccountData.txt
			write the data from the loaded saving account to newUserAccountData.txt instead
		Closes both text files
		deletes userAccountData.txt
		renames newUserAccountData.txt to userAccountData.txt

	*/

	std::fstream newUserAccountData;
	newUserAccountData.open("newUserAccountData.txt", std::ios::out);

	_userAccountData.open("userAccountData.txt", std::ios::in);
	std::string line;
	while (std::getline(_userAccountData, line))
	{
		std::stringstream ss(line);
		std::string accountType;
		std::string usernameAccount;

		std::getline(ss, accountType, ':');
		std::getline(ss, usernameAccount, ':');
		if ((_user.getusername() == usernameAccount) && ("Saving" == accountType))
		{
			newUserAccountData << "Saving";
			newUserAccountData << ":" << _user.getusername();
			newUserAccountData << ":" << _userSaving.getbalance();
			newUserAccountData << ":" << _userSaving.getinterestRate();
			newUserAccountData << ":" << _userSaving.getid();
			newUserAccountData << ":" << _userSaving.getdepositCount();
			newUserAccountData << ":" << _userSaving.getwithdrawalCount() << ":\n";
		}
		else
		{
			if (accountType == "Saving")
			{
				std::string balance, interestRate, id, depositCount, withdrawalCount;

				std::getline(ss, balance, ':');
				std::getline(ss, interestRate, ':');
				std::getline(ss, id, ':');
				std::getline(ss, depositCount, ':');
				std::getline(ss, withdrawalCount, ':');

				newUserAccountData << "Saving";
				newUserAccountData << ":" << usernameAccount;
				newUserAccountData << ":" << balance;
				newUserAccountData << ":" << interestRate;
				newUserAccountData << ":" << id;
				newUserAccountData << ":" << depositCount;
				newUserAccountData << ":" << withdrawalCount << ":\n";
			}
			else if (accountType == "Checking")
			{
				std::string balance, overdraftLimit, id, depositCount, withdrawalCount;

				std::getline(ss, balance, ':');
				std::getline(ss, overdraftLimit, ':');
				std::getline(ss, id, ':');
				std::getline(ss, depositCount, ':');
				std::getline(ss, withdrawalCount, ':');

				newUserAccountData << "Checking";
				newUserAccountData << ":" << usernameAccount;
				newUserAccountData << ":" << balance;
				newUserAccountData << ":" << overdraftLimit;
				newUserAccountData << ":" << id;
				newUserAccountData << ":" << depositCount;
				newUserAccountData << ":" << withdrawalCount << ":\n";
			}
		}
	}
	_userAccountData.close();
	newUserAccountData.close();
	std::remove("userAccountData.txt");
	if (rename("newUserAccountData.txt", "userAccountData.txt") != 0)
	{
		std::cout << "ERROR: File handling\n";
		system("pause");
	}
}

void updateCheckingAccount(customer _user, checkingAccount _userChecking, std::fstream& _userAccountData)
{
	/*
		Opens userAccountData.txt and newUserAccountData.txt
		Writes data from userAccountData.txt to newUserAccountData.txt
		if the line being read from userAccountData.txt contains the checking account data of the current user
			don't write it to newUserAccountData.txt
			write the data from the loaded checking account to newUserAccountData.txt instead
		Closes both text files
		deletes userAccountData.txt
		renames newUserAccountData.txt to userAccountData.txt
	*/

	std::fstream newUserAccountData;
	newUserAccountData.open("newUserAccountData.txt", std::ios::out);

	_userAccountData.open("userAccountData.txt", std::ios::in);
	std::string line;
	while (std::getline(_userAccountData, line))
	{
		std::stringstream ss(line);
		std::string accountType;
		std::string usernameAccount;

		std::getline(ss, accountType, ':');
		std::getline(ss, usernameAccount, ':');
		if ((_user.getusername() == usernameAccount) && ("Checking" == accountType))
		{
			newUserAccountData << "Checking";
			newUserAccountData << ":" << _user.getusername();
			newUserAccountData << ":" << _userChecking.getbalance();
			newUserAccountData << ":" << _userChecking.getoverdraftLimit();
			newUserAccountData << ":" << _userChecking.getid();
			newUserAccountData << ":" << _userChecking.getdepositCount();
			newUserAccountData << ":" << _userChecking.getwithdrawalCount() << ":\n";
		}
		else
		{
			if (accountType == "Saving")
			{
				std::string balance, interestRate, id, depositCount, withdrawalCount;

				std::getline(ss, balance, ':');
				std::getline(ss, interestRate, ':');
				std::getline(ss, id, ':');
				std::getline(ss, depositCount, ':');
				std::getline(ss, withdrawalCount, ':');

				newUserAccountData << "Saving";
				newUserAccountData << ":" << usernameAccount;
				newUserAccountData << ":" << balance;
				newUserAccountData << ":" << interestRate;
				newUserAccountData << ":" << id;
				newUserAccountData << ":" << depositCount;
				newUserAccountData << ":" << withdrawalCount << ":\n";
			}
			else if (accountType == "Checking")
			{
				std::string balance, overdraftLimit, id, depositCount, withdrawalCount;

				std::getline(ss, balance, ':');
				std::getline(ss, overdraftLimit, ':');
				std::getline(ss, id, ':');
				std::getline(ss, depositCount, ':');
				std::getline(ss, withdrawalCount, ':');

				newUserAccountData << "Checking";
				newUserAccountData << ":" << usernameAccount;
				newUserAccountData << ":" << balance;
				newUserAccountData << ":" << overdraftLimit;
				newUserAccountData << ":" << id;
				newUserAccountData << ":" << depositCount;
				newUserAccountData << ":" << withdrawalCount << ":\n";
			}
		}
	}
	_userAccountData.close();
	newUserAccountData.close();
	std::remove("userAccountData.txt");
	if (rename("newUserAccountData.txt", "userAccountData.txt") != 0)
	{
		std::cout << "ERROR: File handling\n";
		system("pause");
	}
}
#pragma endregion