#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <string>
#include <limits>
#include "menus.h"
#include "TableCreation.h"

#define MAX_PASSWORD_SIZE 12
#define MIN_PASSWORD_SIZE 6
#define BACK "b"
#define CONTINUE "0"
#define ALL_GOOD "1"
#define SPACE ' '
enum {SIGN_UP = '1', LOGIN,FORGOT_PASSWORD,EXIT};
enum CandidateMenu{LOOK_FOR_JOBS = '1', CREATE_RESUME, VIEW_JOBS_SUBMITTED, VIEW_INTERVIEW_INVITATIONS, EDIT_PROFILE, LOG_OUT};
enum {COUNT_FEEDBACK = 0,COUNT_FEEDBACK1,COUNT_FEEDBACK2,COUNT_FEEDBACK3};
enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,GO_BACK};
enum SelectRole{CANDIDATE = '1', EMPLOYER, GET_BACK};
#define ID_VALID 9


using namespace SQLite;
using namespace std;

//Validation of fields
//Space
//***************************************************
bool NotValidSpace(string& string)
{
    for(char i : string)
        if(i == ' ')
        {
            cout << "You can't enter a apace!\n";
            return false;
        }
    return true;
}
//***************************************************

//ID
//***************************************************
bool CheckIfIdIsDigits(string &id)
{
    for (char ch : id)
        if (!isdigit(ch))
            return false;
    return true;
}
bool validateId(string &id) {
    if (id.length() == ID_VALID && CheckIfIdIsDigits(id))
        return true;
    else
    {
        cout << "You entered an invalid ID - ID must contain exactly 9 digits.\n";
        return false;
    }
}
string EnterIdTillValid(string& id)
{
    do {
        cout << "Please enter your ID (ID must contain 9 digits).   |   Back - 'b'.:\n";
        getline(cin >> ws, id);
        if (id == BACK)
            return BACK;
    } while (!validateId(id));
    return ALL_GOOD;
}
//***************************************************

//Password
//***************************************************
int numOfCharacters(string&password)
{
    string feedback;
    bool special_character = false, lowercase = false, uppercase = false, digits = false;
    int counter = 0;
    for (char i : password)
    {
        if (isdigit(i) && !digits)
        {
            digits = true;
            ++counter;
        }
        if (islower(i) && !lowercase)
        {
            lowercase = true;
            ++counter;
        }
        if (isupper(i) && !uppercase)
        {
            uppercase = true;
            ++counter;
        }
        if (ispunct(i) && !special_character)
        {
            special_character = true;
            ++counter;
        }
    }
    return counter;
}
void PasswordDifficulty(int num)
{
    if (num == COUNT_FEEDBACK || num == COUNT_FEEDBACK1)
        cout << "The password is weak\n";
    if (num == COUNT_FEEDBACK2)
        cout << "The password is medium\n";
    if (num == COUNT_FEEDBACK3)
        cout << "The password is strong\n";
}
string DoYouWantChangePassword(string& password)
{
    string choice;
    do {
        cout << "Do you want to change your password?\n"
                "For yes - press '0'.\n"
                "For no  - press '1' \n"
                "Back - press 'b'." << endl;
        getline(cin >> ws, choice);
    } while (choice != CONTINUE && choice != ALL_GOOD && choice != BACK);
    return choice;
}
bool validPassword(string &password)
{
    if(password.length() < MIN_PASSWORD_SIZE || password.length() > MAX_PASSWORD_SIZE)
    {
        cout << "You entered invalid password - password must contain 6 to 12 characters!\n";
        return false;
    }
    return NotValidSpace(password);
}
string EnterPasswordTillValid(string& password)
{
    string choice;
    do {
        do {
            cout << "Please enter a password (6 to 12 characters, no spaces).   |   Back - 'b'.\n"
                    "Feedback on password difficulty:\n"
                    "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
                    "- Medium: exactly two types of characters.\n"
                    "- Weak: exactly one type of character.\n";
            getline(cin >> ws, password);
            if (password == BACK)
                return BACK;
        } while (!validPassword(password));
        PasswordDifficulty(numOfCharacters(password));
        choice = DoYouWantChangePassword(password);
    } while (choice == CONTINUE);
    return choice;
}
//***************************************************

//Name
//***************************************************
bool CheckIfNameIsLetters(string& name)
{
    for (char c : name)
        if (!isalpha(c))
            return false;
    return true;
}
bool validateName(string &name) {
    if(name.length() == 0 || name.length() >= 50 || !NotValidSpace(name) || !CheckIfNameIsLetters(name))
    {
        cout << "You entered an invalid name. Name must contain only letters and not exceed 50 characters. Please try again.\n";
        return false;
    }
    return true;
}
string EnterNameTillValid(string& name)
{
    do {
        cout << "Please enter your name (must contain only letters and not more than 50 letters)   |   Back - 'b':\n";
        getline(cin >> ws, name);
        if (name == BACK)
            return BACK;
    } while (!validateName(name));
    return ALL_GOOD;
}
//***************************************************

//Age
//***************************************************
bool validateAge(string &age) {

    int ageNumber = stoi(age);
    if(age.length() == 0 || ageNumber < 18 || ageNumber > 99 || !NotValidSpace(age))
    {
        cout << "You entered an invalid age. Age must be between 18 - 99. Please try again.\n";
        return false;
    }
    return true;
}
string EnterAgeTillValid(string& age)
{
    do {
        cout << "Please enter your age (18 - 99)   |   Back - 'b':\n";
        getline(cin >> ws, age);
        if (age == BACK)
            return BACK;
    } while (!validateAge(age));
    return ALL_GOOD;
}
//***************************************************

//FreeText
//***************************************************
bool validFreeText(string &freeText)
{
    if (freeText.length() > 200)
    {
        cout << "You entered an invalid free text. text must contain not more then 200 characters.\n";
        return false;
    }
    return true;

}
string EnterFreeTextTillValid(string& freeText)
{
    do {
        cout << "Please some information about yourself   |   Back - 'b':\n";
        getline(cin >> ws, freeText);
        if (freeText == BACK)
            return BACK;
    } while (!validFreeText(freeText));
    return ALL_GOOD;
}
//***************************************************

//SelectQuestion
//***************************************************
string selectQuestion(string &question, string &answer)
{
        cout << "Please select a number of question:\n";
        cout << "1. What is the name of your father?\n"
             << "2. What is the name of your mother?\n"
             << "3. What is the name of the school you attended?\n"
             << "4. What year were you born?\n"
             << "5. What is your favorite food?\n"
             << "6. Back.\n";
        char option;
        option = UserChoice();
        switch (option) {
            case QUESTION_1:
                question = "What is the name of your father?";
                break;
            case QUESTION_2:
                question = "What is the name of your mother?";
                break;
            case QUESTION_3:
                question = "What is the name of the school you attended?";
                break;
            case QUESTION_4:
                question = "What year were you born?";
                break;
            case QUESTION_5:
                question = "What is your favorite food?";
                break;
            case GO_BACK:
                return BACK;
            default:
                cout << "You entered an illegal option. Please try again!\n";
        }
    cout << "Please enter your answer for \"" << question << "\":\n";
    getline(cin >> ws, answer);
    return ALL_GOOD;
}
//***************************************************

//Role
//***************************************************
string SelectCandidateOrEmployer(string& role){
    while (true) {
        cout << "Please select who are you:\n";
        cout << "1. Candidate\n"
             << "2. Employer\n"
                "3. Back\n";
        char option = UserChoice();
        switch (option) {
            case CANDIDATE:
                role = "candidate";
                return ALL_GOOD;
            case EMPLOYER:
                role = "employer";
                return ALL_GOOD;
            case GET_BACK:
                return BACK;
            default:
                cout << "You entered an illegal option. Please try again!\n";
        }
    }
}
//***************************************************

//ForgotPassword
//***************************************************
bool CheckUserAnswer(Database& db, string& id) {
    if (!ForgotPasswordTableExists(db)) {
        cout << "forgot_password table does not exist.\n";
        return false;
    }
    try {
        string answer;
        Statement query(db, "SELECT question,answer FROM forgot_password WHERE id = ?");
        query.bind(1, stoi(id));
        if (query.executeStep()) {
            string dbQuestion = query.getColumn(0).getText();
            string dbAnswer = query.getColumn(1).getText();
            cout << "For change your password answer the next question:\n";
            cout << dbQuestion << "\n";
            getline(cin >> ws, answer);
            if (dbAnswer == answer) {
                cout << "Correct answer.\n";
                return true;
            } else {
                cout << "Wrong answer.\n";
                return false;
            }
        } else {
            cout << "User ID not found.\n";
            return false;
        }
    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        return false;
    }
}
void ChangePassword (Database& db, string& id) {
    string newPassword;
    if (EnterPasswordTillValid(newPassword) == BACK)
        return;
    try {
        Statement query(db, "UPDATE users SET password = ? WHERE id = ?");
        query.bind(1, newPassword);
        query.bind(2, stoi(id));
        query.exec();
        cout << "Password updated successfully.\n";
    } catch(exception &e) {
        cerr << "SQLite exception: " << e.what() << "\n";
    }
}
void ForgotPassword(Database& db) {
    string id;
    do {
        cout << "Enter Your ID (must be 9 digits)   |   Back - 'b':\n";
        cin.ignore();
        getline(cin, id);
        if (id == BACK)
            return;
    } while (!validateId(id));
    if(CheckUserAnswer(db,id))
        ChangePassword(db,id);
}
//***************************************************

//Register and Login
//***************************************************
void Register (Database& db)
{
    string id, password, name, age,role,question, answer,freeText;

    if(!UsersTableExists(db))
        CreateUsersTable(db);

    cout << "Dear user, please enter your details to sign up.\n";

    if (EnterIdTillValid(id) == BACK || EnterPasswordTillValid(password) == BACK || EnterNameTillValid(name) == BACK
        || EnterAgeTillValid(age) == BACK || EnterFreeTextTillValid(freeText) == BACK || selectQuestion(question,answer) == BACK
        || SelectCandidateOrEmployer(role) == BACK || InsertUserToDatabase(db ,id ,password ,name ,age , role, freeText))
        return;

    if(!ForgotPasswordTableExists(db))
        CreateForgotPasswordTable(db);
    InsertForgotPasswordDetailsToDatabase(db,id,question,answer);
}
string Login(Database& db, string& name)
{
    if (!UsersTableExists(db))
    {
        cout << "Users table does not exist.\n";
        return "ERROR";
    }
    try {
        string id, password;
        if (EnterIdTillValid(id) == BACK)
            return BACK;

        cout << "Enter Your password (must be between 6-12 digits, without spaces):\n";
        do {
            getline(cin >> ws, password);
        } while (!validPassword(password));

        Statement query(db, "SELECT password,role FROM users WHERE id = ?");
        query.bind(1, stoi(id));
        if (query.executeStep())
        {
            string dbPassword = query.getColumn(0).getText();
            string role = query.getColumn(1).getText();
            if (dbPassword == password) {
                return id;
            } else {
                cout << "Invalid password \n";
                return "ERROR";
            }
        }
        else {
            cout << "User ID not found.\n";
            return "ERROR";
        }
    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        return "ERROR";
    }
}
//***************************************************
string GetUserRole(Database& db, string& id) {
    try {
        Statement query(db, "SELECT role FROM users WHERE id = ?");
        query.bind(1, stoi(id));
        if (query.executeStep()) {
            return query.getColumn(0).getText();
        }
        return "Error";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return "Error";
}

