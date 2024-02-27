#ifndef MAIN_CPP_AUTHENTICATION_H
#define MAIN_CPP_AUTHENTICATION_H
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include "iostream"

using namespace SQLite;
using namespace std;


//Validation of fields
bool CheckIdLength(string&);
bool CheckIfIdIsDigits(string&);
bool ValidatePassword(string&);
<<<<<<< HEAD
bool ValidateFreeText(string &freeText);
=======
bool validateId(string &);
bool validateName(string &);
bool validateAge(string &);
bool validFreeText(string &);
bool passwordDifficulty(string&);
bool selectQuestion(string &, string &);
>>>>>>> b33e9a15ab5f96340d8258c7333df68ec46262d4

//Validation of tables
bool UsersTableExists(Database&);
bool ForgotPasswordTableExists(Database&);



//Logic
string SelectForgotPasswordQuestion();
string SelectCandidateOrEmployer();
void CreateUsersTable(Database&);
bool InsertUserToDatabase(Database&, string&, string&, string&, string&, string&,string&);
bool InsertForgotPasswordDetailsToDatabase(Database&, string&,string&,string&);
void Register(Database&);
string Login(Database&);
void CreateForgotPasswordTable(Database&);
bool CheckUserAnswer(Database&, string&);
void ChangePassword(Database& , string&);
void ForgotPassword(Database&);
string GetUserRole(Database&,string&);




#endif //MAIN_CPP_AUTHENTICATION_H
