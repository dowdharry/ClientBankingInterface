#pragma once
#ifndef USERCLASSES
#endif USERCLASSES // !USERCLASSES
#include <iostream>
#include <iomanip>

// Customer class declaration
class customer
{
public:
	// Constructors
	customer();
	customer(std::string _firstname, std::string _lastName, std::string _address, std::string _phoneNum, std::string _email, std::string _username);

	// Setters
	void setfirstName(std::string _firstName);
	void setlastName(std::string _lastName);
	void setaddress(std::string _address);
	void setphoneNum(std::string _phoneNum);
	void setemail(std::string _email);
	void setusername(std::string _username);

	// Getters
	std::string getfirstName() const;
	std::string getlastName() const;
	std::string getaddress() const;
	std::string getphoneNum() const;
	std::string getemail() const;
	std::string getusername() const;

	// Other functions
	void printInfo() const;

private:
	// Attributes
	std::string firstName;
	std::string lastName;
	std::string address;
	std::string phoneNum;
	std::string email;
	std::string username;
};

// Account class declaration
class account
{
public:
	// Constructors
	account();
	account(customer _accountCustomer, float _balance, int _id, int _depositCount, int _withdrawalCount);

	// Setters
	void setaccountCustomer(customer _accountCustomer);
	void setbalance(float _balance);
	void setid(int _id);
	void setdepositCount(int _depositCount);
	void setwithdrawalCount(int _withdrawalCount);

	// Getters
	customer getaccountCustomer() const;
	float getbalance() const;
	int getid() const;
	int getdepositCount() const;
	int getwithdrawalCount() const;

	// Other functions
	void printInfo() const;
	void deposit(float _depAmount);
	void withdrawal(float _withAmount);

private:
	// Attributes
	customer accountCustomer;
	float balance;
	int id;
	int depositCount;
	int withdrawalCount;
};

// SavingAccount class declaration
class savingAccount : public account
{
public:
	// Constructors
	savingAccount();
	savingAccount(float _interestRate, customer _accountCustomer, float _balance, int _id, int _depositCount, int _withdrawalCount);

	// Setters
	void setinterestRate(float _interestRate);

	// Getters
	float getinterestRate() const;

	// Other functions
	void printInfo() const;
	void payInterest();
private:
	// Attributes
	float interestRate;
};

// CheckingAccount class declaration
class checkingAccount : public account
{
public:
	// Constructors
	checkingAccount();
	checkingAccount(float _overdraftLimit, customer _accountCustomer, float _balance, int _id, int _depositCount, int _withdrawalCount);

	// Setters
	void setoverdraftLimit(float _overdraftLimit);

	// Getters
	float getoverdraftLimit() const;

	// Other functions
	void printInfo() const;
private:
	// Attributes
	float overdraftLimit;
};
