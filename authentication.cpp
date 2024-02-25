#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <string>

enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,GO_BACK};
#define MAX_PASSWORD_SIZE 12
#define MIN_PASSWORD_SIZE 6
#define SPACE ' '
enum {SIGN_UP = '1', LOGIN,FORGOT_PASSWORD,EXIT};
enum CandidateMenu{LOOK_FOR_JOBS = '1', CREATE_RESUME, VIEW_JOBS_SUBMITTED, VIEW_INTERVIEW_INVITATIONS, EDIT_PROFILE, LOG_OUT};
enum {COUNT_FEEDBACK = 0,COUNT_FEEDBACK1,COUNT_FEEDBACK2,COUNT_FEEDBACK3};
#define ID_VALID 9


using namespace SQLite;
using namespace std;

//Validation of fields
bool CheckIdLength(string &id) {
    return id.length() == 9;
}
bool CheckIfIdIsDigits(string &id) {
    for (char ch : id) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}
bool validPassword(string &password)
{
    char count = '1';
    if(password.length() < MIN_PASSWORD_SIZE || password.length() > MAX_PASSWORD_SIZE)
        count = '0';
    for (int i = 0; i < password.length(); ++i)
    {
        if(password[i] == ' ')
        {
            count = '0';
            break;
        }

    }

    if (count == '0')
    {
        cout << "you entered not valid password please try again:\n";
        return false;
    }
    return true;
}
bool validateId(string &id) {
        if (id.length() == ID_VALID)
        {
            bool valid = true;
            for (char c : id)
            {
                if (c < '0' || c > '9')
                {
                    return false;
                }
            }

        } else
            return false;
    return true;
}
bool validateName(string &name) {

        for (char c : name)
        {
            if (!isalpha(c))
            {
                return false;
            }
        }
        if (name.length() >= 50)
        {
            return false;
        }

    return true;
}
bool validateAge(string &age) {

    int ageNumber = stoi(age);
    if (ageNumber < 18 || ageNumber > 99)
    {
        return false;
    }
    return true;
}
bool validFreeText(string &freeText)
{

        if (freeText.length() > 200)
        {
            return false;
        }

        return true;

}
bool passwordDifficulty(string&password,char &get_out)
{
        string feedback;
        bool special_character = false;
        bool lowercase = false;
        bool uppercase = false;
        int counter = 0;

        for (int i=0;i < password.length();++i)
        {
            if((password[i] >= 33 && password[i] <= 47)||(password[i] >=58 && password[i] <= 64)||(password[i] >=91 && password[i] <= 96) )
            {
                if(!special_character)
                    ++counter;
                special_character = true;
            }
            if(password[i]>=97 && password[i] <= 122)
            {
                if(!lowercase)
                    ++counter;
                lowercase = true;
            }
            if(password[i] >=65 && password[i] <=90)
            {
                if(!uppercase)
                    ++counter;
                uppercase = true;

            }

        }


        if (counter == COUNT_FEEDBACK || counter == COUNT_FEEDBACK1)
            cout << "The password is weak\n";
        if (counter == COUNT_FEEDBACK2)
            cout << "The password is medium\n";
        if (counter == COUNT_FEEDBACK3)
            cout << "The password is strong\n";
        cout << "Do you want to change your password?(press 0 for yes/any other character for no/to return to main menu press 1)\n";
        cin >> get_out;
        if(get_out == '0')
            return false;
        if(get_out == '1')
            return false;
    return true;

}
bool selectQuestion(string &question, string &answer ,char &get_out)
{


        cout << "Please select a number of question:\n";
        cout << "1. What is the name of your father?\n"
             << "2. What is the name of your mother?\n"
             << "3. What is the name of the school you attended?\n"
             << "4. What year were you born?\n"
             << "5. What is your favorite food?\n"
             << "6. Go back to the login/register menu\n";
        char option;
        cin >> option;
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
            {
                get_out = '0';
                return false;
            }

            default:
                cout << "You entered an illegal option. Please try again!\n";
                return false;
        }
        cout << "Please enter your answer for \"" << question << "\":\n";
        cin >> answer;
         return true;


}

//Validation of tables
bool UsersTableExists(Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='users';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << std::endl;
    }
    return false;
}
bool ForgotPasswordTableExists(Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='forgot_password';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << std::endl;
    }
    return false;
}

//Logic
string SelectCandidateOrEmployer(){
    while (true) {
        cout << "Please select who are you:\n";
        cout << "1. candidate\n"
             << "2. employer\n";
        char option;
        cin >> option;
        switch (option) {
            case QUESTION_1:
                return "candidate";
            case QUESTION_2:
                return "employer";
            default:
                cout << "You entered an illegal option. Please try again!\n";
                continue;
        }
    }
}
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
void CreateUsersTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY UNIQUE,"
                "password TEXT NOT NULL,"
                "name TEXT NOT NULL,"
                "age INTEGER NOT NULL,"
                "freeText TEXT NOT NULL,"
                "role TEXT NOT NULL);");
        cout << "Users table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateForgotPasswordTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS forgot_password ("
                "id INTEGER PRIMARY KEY,"
                "question TEXT NOT NULL,"
                "answer TEXT NOT NULL,"
                "FOREIGN KEY(id) REFERENCES users(id)"
                ");");
        cout << "Forgot password table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
bool InsertForgotPasswordDetailsToDatabase(Database& db, string& id, string& question ,string& answer) {
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO forgot_password (id, question ,answer) VALUES (?, ?, ?);");
        // Bind values to the statement
        query.bind(1, stoi(id)); // Assuming ID is a numeric value stored as text
        query.bind(2, question);
        query.bind(3, answer);

        // Execute the statement
        query.exec();
        cout << "Password recovery details saved.\n";
        return true;
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        return false;
    }
}
bool InsertUserToDatabase(Database& db, string& id, string& password, string& name, string& age, string& role,string&freetext) {
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO users (id, password, name, age,freeText, role) VALUES (?, ?, ?,?, ?, ?);");

        // Bind values to the statement
        query.bind(1, stoi(id)); // Assuming ID is a numeric value stored as text
        query.bind(2, password);
        query.bind(3, name);
        query.bind(4, stoi(age)); // Convert age from string to integer
        query.bind(5, freetext);
        query.bind(6, role);

        // Execute the statement
        query.exec();
        cout << "User " << name << " successfully added to the database.\n";
        return true;
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        return false;
    }
}
void Register (Database& db) {
    string id, password, name, age,role,question, answer,freetext;
    char get_out;
    if(!UsersTableExists(db))
        CreateUsersTable(db);
    cout << "Dear user, please enter your details to sign up.\n";
    cout << "Please enter your ID (ID must contain 9 digits):\n";
    cin.ignore();
    getline(cin,id);
    while (!validateId(id))
    {
        cout << "You entered an invalid ID. ID must contain exactly 9 digits. Please try again.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter ID again.\n";

        cin >> get_out;
        if (get_out == '0')
            return;
        cout << "Please enter your ID (ID must contain 9 digits):\n";
        cin.ignore();
        getline(cin,id);
    }
    cout << "Please enter a password (6 to 12 characters, no spaces).\n"
            "Feedback on password difficulty:\n"
            "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
            "- Medium: exactly two types of characters.\n"
            "- Weak: exactly one type of character.\n";
    cin.ignore();
    getline(cin,id);
    while (!validPassword(password) || !passwordDifficulty(password ,get_out))
    {
        if (get_out == '1')
            return;
        cout << "Please enter a password (6 to 12 characters, no spaces).\n"
                "Feedback on password difficulty:\n"
                "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
                "- Medium: exactly two types of characters.\n"
                "- Weak: exactly one type of character.\n";
        getline(cin,id);
    }
    cout << "Please enter your name (must contain only letters and not more than 50 letters):\n";
    cin.ignore();
    getline(cin,id);
    while (!validateName(name))
    {
        cout << "You entered an invalid name. Name must contain only letters and not exceed 50 characters. Please try again.\n";
        cout << "If you want to return to the main menu, press '0'. Otherwise, press any other character to enter the name again:\n";
        cin >> get_out;
        if (get_out == '0')
            return;
        cout << "Please enter your name (must contain only letters and not more than 50 letters):\n";
        getline(cin,id);
    }
    cout << "Please enter your age (18 - 99):\n";
    cin.ignore();
    getline(cin,id);
    while (!validateAge(age))
    {
        cout << "You entered an invalid age. Age must be between 18 - 99. Please try again.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter age again.\n";
        cin >> get_out;
        if (get_out == '0')
            return;
        cout << "Please enter your age (18 - 99):\n";
        getline(cin,id);
    }
    cout << "please tell about yourself\n";
    cin.ignore();
    getline(cin, freetext);
    while (!validFreeText(freetext))
    {
        cout << "You entered an invalid free text. text must contain not more then 200 characters.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter free text again.\n";
        cin >> get_out;
        if (get_out == '0')
            return;
        cout << "please tell about yourself\n";
        cin.ignore();
        getline(cin, freetext);
    }
    while (!selectQuestion(question,answer,get_out))
    {

        if (get_out == '0')
            return;
    }

    role = SelectCandidateOrEmployer();
    if(InsertUserToDatabase(db,id,password,name,age,role,freetext))
    {
        return;
    }
    if(!ForgotPasswordTableExists(db))
        CreateForgotPasswordTable(db);
    InsertForgotPasswordDetailsToDatabase(db,id,question,answer);
}
string Login (Database& db) {

    char get_out;

    if (!UsersTableExists(db)) {
        cout << "users not exist.\n";
        return "ERROR";
    }
    try {
        string id, password;
        cout << "Enter Your ID (must be 9 digits).\n";
        cin >> id;

        cout << "Enter Your password.\n";
        cin >> password;

        Statement query(db, "SELECT password,role FROM users WHERE id = ?");
        query.bind(1, stoi(id));
        if (query.executeStep())
        {
            string dbPassword = query.getColumn(0).getText();
            string role = query.getColumn(1).getText();
            if (dbPassword == password)
            {
                return id;
            }
            else
            {
                cout << "Invalid password \n";
                cout << "Do you want to return to the main menu(press 0 for yes/no any other character to try login again):\n";
                cin >> get_out;
                if(get_out == '0')
                    return "RETURN";
                return "ERROR";
            }
        }
        else
        {
            cout << "User ID not found.\n";
            cout << "Do you want to return to the main menu(press 0 for yes/no any other character to try login again):\n";
            cin >> get_out;
            if(get_out == '0')
                return "RETURN";
            return "ERROR";
        }

    } catch(exception& e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
        return "ERROR";
    }
}
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
            cout << "For change your password answer the next question.:\n";
            cout << dbQuestion << "\n";
            cin >> answer;
            if (dbAnswer == answer) {
                cout << "Correct answer.\n";
                return true;
            } else {
                cout << "Invalid answer.\n";
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
    char get_out;
    cout << "Please enter a password (6 to 12 characters, no spaces).\n"
            "Feedback on password difficulty:\n"
            "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
            "- Medium: exactly two types of characters.\n"
            "- Weak: exactly one type of character.\n";
    cin >> newPassword;
    while (!validPassword(newPassword) || !passwordDifficulty(newPassword ,get_out))
    {
        if (get_out == '1')
            return;
        cout << "Please enter a password (6 to 12 characters, no spaces).\n"
                "Feedback on password difficulty:\n"
                "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
                "- Medium: exactly two types of characters.\n"
                "- Weak: exactly one type of character.\n";
        cin >> newPassword;
    }
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
    cout << "Enter Your ID (must be 9 digits).\n";
    cin >> id;
    while (!CheckIdLength(id) || !CheckIfIdIsDigits(id)) {
        cout << "Invalid ID. ID must be exactly 9 digits long and contains only numbers. Please try again:\n";
        cin >> id;
    }
    if(CheckUserAnswer(db,id))
        ChangePassword(db,id);
}

