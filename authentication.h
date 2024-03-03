#ifndef MAIN_CPP_AUTHENTICATION_H
#define MAIN_CPP_AUTHENTICATION_H
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include "iostream"

using namespace SQLite;
using namespace std;

//Validation of fields
//Space
bool NotValidSpace(string& string);
//***************************************************
//ID
bool CheckIfIdIsDigits(string&);
bool validateId(string &);
string EnterIdTillValid(string& id);
//***************************************************
//Password
int numOfCharacters(string&);
void PasswordDifficulty(int num);
string DoYouWantChangePassword(string& password);
bool validPassword(string &password);
string EnterPasswordTillValid(string& password);
//***************************************************
//Name
bool CheckIfNameIsLetters(string&);
bool validateName(string &);
string EnterNameTillValid(string& name);
//***************************************************
//Age
bool validateAge(string &);
string EnterAgeTillValid(string& age);
//***************************************************
//FreeText
bool validFreeText(string &);
string EnterFreeTextTillValid(string& freeText);
//***************************************************
//Question
string selectQuestion(string &, string &);
//***************************************************
//Role
string SelectCandidateOrEmployer(string& role);
//***************************************************

//ForgotPassword
//***************************************************
bool CheckUserAnswer(Database&, string&);
void ChangePassword(Database& , string&);
void ForgotPassword(Database&);
//***************************************************

//Register and Login
//***************************************************
void Register(Database&);
string Login(Database& db, string& name);
//***************************************************
string GetUserRole(Database&,string&);




#endif //MAIN_CPP_AUTHENTICATION_H