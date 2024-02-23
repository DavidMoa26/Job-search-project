#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <string>
#include <tuple>

enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,GO_BACK};

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
bool ValidateName(string &name) {
    if (name.length() > 50) {
        return false;
    }
    for (char ch : name) {
        if (!isalpha(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true; // Return true if all checks pass
}
bool ValidateAge(string &age) {
    if (age.empty()) {
        return false;
    }
    // Check if all characters in the string are digits
    for (char ch : age) {
        if (!isdigit(static_cast<unsigned char>(ch))) {
            return false; // Found a non-digit character
        }
    }

    // Convert the string to an integer
    int ageInt = stoi(age);

    // Check if age is within the valid range
    return ageInt >= 18 && ageInt <= 99;
}
bool ValidatePassword(string &password) {
    if (password.length() < 6 || password.length() > 12) {
        return false;
    }

    int lowercaseCount = 0, uppercaseCount = 0, digitCount = 0, specialCharCount = 0;

    for (char ch : password) {
        if (islower(ch)) {
            ++lowercaseCount;
        } else if (isupper(ch)) {
            ++uppercaseCount;
        } else if (isdigit(ch)) {
            ++digitCount;
        } else {
            ++specialCharCount;
        }
    }

    int categoriesMet = 0;
    if (lowercaseCount > 0) ++categoriesMet;
    if (uppercaseCount > 0) ++categoriesMet;
    if (digitCount > 0) ++categoriesMet;
    if (specialCharCount > 0) ++categoriesMet;

    return categoriesMet >= 3;
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
string SelectForgotPasswordQuestion(){
    while (true) {
        cout << "Please select a number of question:\n";
        cout << "1. What is the name of your father?\n"
             << "2. What is the name of your mother?\n"
             << "3. What is the name of the school you attended?\n"
             << "4. What year were you born?\n"
             << "5. What is your favorite food?\n";
        char option;
        cin >> option;
        switch (option) {
            case QUESTION_1:
                return "What is the name of your father?";
            case QUESTION_2:
                return "What is the name of your mother?";
            case QUESTION_3:
                return "What is the name of the school you attended?";
            case QUESTION_4:
                return "What year were you born?";
            case QUESTION_5:
                return "What is your favorite food?";
            default:
                cout << "You entered an illegal option. Please try again!\n";
                continue;
        }
    }
}
void CreateUsersTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY UNIQUE,"
                "password TEXT NOT NULL,"
                "name TEXT NOT NULL,"
                "age INTEGER NOT NULL,"
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
bool InsertUserToDatabase(Database& db, string& id, string& password, string& name, string& age, string& role) {
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO users (id, password, name, age, role) VALUES (?, ?, ?, ?, ?);");

        // Bind values to the statement
        query.bind(1, stoi(id)); // Assuming ID is a numeric value stored as text
        query.bind(2, password);
        query.bind(3, name);
        query.bind(4, stoi(age)); // Convert age from string to integer
        query.bind(5, role);

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
    string id, password, name, age,role,question, answer;
    cout << "Enter Your ID (must be 9 digits).\n";
    cin >> id;
    while (!CheckIdLength(id) || !CheckIfIdIsDigits(id)) {
        cout << "Invalid ID. ID must be exactly 9 digits long and contains only numbers. Please try again:\n";
        cin >> id;
    }
    cout << "Enter Your password (must be between 6-12 digits,must contains lowercase,uppercase,numbers).\n";
    cin >> password;
    while (!ValidatePassword(password)) {
        cout << "Invalid Password. Must be 6-12 digits,must contains lowercase,uppercase,numbers. Please try again:\n";
        cin >> password;
    }
    cout << "Enter Your name (only letters).\n";
    cin >> name;
    while (!ValidateName(name)) {
        cout << "Invalid name - must contain only letters:\n";
        cin >> name;
    }
    cout << "Enter Your age (only numbers 18-99).\n";
    cin >> age;
    while (!ValidateAge(age)) {
        cout << "Invalid age - must contain numbers between 18-99:\n";
        cin >> age;
    }
    role = SelectCandidateOrEmployer();
    question = SelectForgotPasswordQuestion();
    cout << "Enter Your answer :\n";
    cin >> answer;
    if(!UsersTableExists(db))
        CreateUsersTable(db);
    InsertUserToDatabase(db,id,password,name,age,role);
    if(!ForgotPasswordTableExists(db))
        CreateForgotPasswordTable(db);
    InsertForgotPasswordDetailsToDatabase(db,id,question,answer);
}
string Login (Database& db) {
    if (!UsersTableExists(db)) {
        cout << "Users table does not exist.\n";
        return "ERROR";
    }
    try {
        string id, password;
        cout << "Enter Your ID (must be 9 digits).\n";
        cin >> id;
        while (!CheckIdLength(id) || !CheckIfIdIsDigits(id)) {
            cout << "Invalid ID. ID must be exactly 9 digits long and contains only numbers. Please try again:\n";
            cin >> id;
        }
        cout << "Enter Your password (must be between 6-12 digits,must contains lowercase,uppercase,numbers).\n";
        cin >> password;
        while (!ValidatePassword(password)) {
            cout << "Invalid Password. Must be 6-12 digits,must contains lowercase,uppercase,numbers. Please try again:\n";
            cin >> password;
        }
        Statement query(db, "SELECT password,role FROM users WHERE id = ?");
        query.bind(1, stoi(id));
        if (query.executeStep()) {
            string dbPassword = query.getColumn(0).getText();
            string role = query.getColumn(1).getText();
            if (dbPassword == password) {
                return id;
            } else {
                cout << "Invalid password \n";
                return "ERROR";
            }
        } else {
            cout << "User ID not found.\n";
            return "ERROR";
        }
    } catch(exception& e) {
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
    cout << "Enter your new password : \n";
    cin >> newPassword;
    while (!ValidatePassword(newPassword)) {
        cout << "Invalid Password. Must be 6-12 digits,must contains lowercase,uppercase,numbers. Please try again:\n";
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
