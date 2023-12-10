#include "UserClasses.h"

#pragma region Customer Class Implementation
// Constructors, used for customer class creation
customer::customer()
{
	firstName = "";
	lastName = "";
	address = "";
	phoneNum = "";
	email = "";
	username = "";
}

customer::customer(std::string _firstName, std::string _lastName, std::string _address, std::string _phoneNum, std::string _email, std::string _username)
{
	firstName = _firstName;
	lastName = _lastName;
	address = _address;
	phoneNum = _phoneNum;
	email = _email;
	username = _username;
}

// Setters, used for setting specific private attributes
void customer::setfirstName(std::string _firstName)
{
	firstName = _firstName;
}

void customer::setlastName(std::string _lastName)
{
	lastName = _lastName;
}

void customer::setaddress(std::string _address)
{
	address = _address;
}

void customer::setphoneNum(std::string _phoneNum)
{
	phoneNum = _phoneNum;
}

void customer::setemail(std::string _email)
{
	email = _email;
}

void customer::setusername(std::string _username)
{
	username = _username;
}

// Getters, used for returning specific private attributes
std::string customer::getfirstName() const
{
	return firstName;
}

std::string customer::getlastName() const
{
	return lastName;
}

std::string customer::getaddress() const
{
	return address;
}

std::string customer::getphoneNum() const
{
	return phoneNum;
}

std::string customer::getemail() const
{
	return email;
}

std::string customer::getusername() const
{
	return username;
}

// Other functions

// printInfo function prints attributes of customer class
void customer::printInfo() const
{
	std::cout << std::setw(30) << std::left << "First name:" << firstName << "\n";
	std::cout << std::setw(30) << std::left << "Last name:" << lastName << "\n";
	std::cout << std::setw(30) << std::left << "Address:" << address << "\n";
	std::cout << std::setw(30) << std::left << "Phone number:" << phoneNum << "\n";
	std::cout << std::setw(30) << std::left << "Email:" << email << "\n";
	std::cout << std::setw(30) << std::left << "Username:" << username << "\n";
}
#pragma endregion

#pragma region Account Class Implementation
// Constructors, used for account class creation
account::account()
{
	accountCustomer = customer();
	balance = 0.0;
	id = 0;
	depositCount = 0;
	withdrawalCount = 0;
}

account::account(customer _accountCustomer, float _balance, int _id, int _depositCount, int _withdrawalCount)
{
	accountCustomer = customer(_accountCustomer);
	balance = _balance;
	id = _id;
	depositCount = _depositCount;
	withdrawalCount = _withdrawalCount;
}

// Setters, used for setting specific private attributes
void account::setaccountCustomer(customer _accountCustomer)
{
	accountCustomer = customer(_accountCustomer);
}

void account::setbalance(float _balance)
{
	balance = _balance;
}

void account::setid(int _id)
{
	id = _id;
}

void account::setdepositCount(int _depositCount)
{
	depositCount = _depositCount;
}

void account::setwithdrawalCount(int _withdrawalCount)
{
	withdrawalCount = _withdrawalCount;
}

// Getters, used for returning a specific private attribute
customer account::getaccountCustomer() const
{
	return accountCustomer;
}

float account::getbalance() const
{
	return balance;
}

int account::getid() const
{
	return id;
}

int account::getdepositCount() const
{
	return depositCount;
}

int account::getwithdrawalCount() const
{
	return withdrawalCount;
}

// Other functions

// printInfo function prints attributes of account class
void account::printInfo() const
{
	accountCustomer.printInfo();
	std::cout << std::setw(30) << std::left << "Balance:" << balance << "\n";
	std::cout << std::setw(30) << std::left << "ID:" << id << "\n";
	std::cout << std::setw(30) << std::left << "Number of deposits:" << depositCount << "\n";
	std::cout << std::setw(30) << std::left << "Number of withdrawals:" << withdrawalCount << "\n";
}

// deposit function add deposit amount parameter to balance and increments deposit count
void account::deposit(float _depAmount)
{
	balance = balance + _depAmount;
	depositCount++;
}

// withdrawal function subtracts withdrawal amount parameter to balance and increments withdrawal count
void account::withdrawal(float _withAmount)
{
	balance = balance - _withAmount;
	withdrawalCount++;
}
#pragma endregion

#pragma region Saving Account Class Implementation
// Constructors, used for saving account class creation
savingAccount::savingAccount() : account()
{
	interestRate = 0.0;
}

savingAccount::savingAccount(float _interestRate, customer _accountCustomer, float _balance, int _id, int _depositCount, int _withdrawalCount) : account(_accountCustomer, _balance, _id, _depositCount, _withdrawalCount)
{
	interestRate = _interestRate;
}

// Setters, used for setting specific private attributes
void savingAccount::setinterestRate(float _interestRate)
{
	interestRate = _interestRate;
}

// Getters, used for returning specific private attributes
float savingAccount::getinterestRate() const
{
	return interestRate;
}

// Other functions
void savingAccount::printInfo() const
{
	account::printInfo();
	std::cout << std::setw(30) << std::left << "Interest rate:" << interestRate << "\n";
}

void savingAccount::payInterest()
{
	float tempBalance = getbalance() * (1 + interestRate);
	setbalance(tempBalance);
}
#pragma endregion

#pragma region Checking Account Class Implementation
// Constructors, used for checking account class creation
checkingAccount::checkingAccount() :account()
{
	overdraftLimit = 0.0;
}

checkingAccount::checkingAccount(float _overdraftLimit, customer _accountCustomer, float _balance, int _id, int _depositCount, int _withdrawalCount) : account(_accountCustomer, _balance, _id, _depositCount, _withdrawalCount)
{
	overdraftLimit = _overdraftLimit;
}

// Setters, used for setting specific private attributes
void checkingAccount::setoverdraftLimit(float _overdraftLimit)
{
	overdraftLimit = _overdraftLimit;
}

// Getters, used for returning specific private attributes
float checkingAccount::getoverdraftLimit() const
{
	return overdraftLimit;
}

// Other functions
void checkingAccount::printInfo() const
{
	account::printInfo();
	std::cout << std::setw(30) << std::left << "Overdraft limit:" << overdraftLimit << "\n";
}
#pragma endregion
