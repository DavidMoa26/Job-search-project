
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

#define MAX_PASSWORD_SIZE 12
#define MIN_PASSWORD_SIZE 6
#define SPACE ' '
#define ID_VALID 9

using namespace std;
using namespace SQLite;

enum MainMenu{SIGN_UP = '1', LOGIN,FORGOT_PASSWORD,EXIT};
enum CandidateMenu{LOOK_FOR_JOBS = '1', CREATE_RESUME, VIEW_JOBS_SUBMITTED, VIEW_INTERVIEW_INVITATIONS, EDIT_PROFILE, LOG_OUT_C};
enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,GO_BACK};
enum EmployerMenu{PUBLISH_JOB = '1', VIEW_ALL_JOBS, VIEW_CANDIDATES_PROFILES, LOG_OUT_E};
enum {NUMBER_PROBLEM1 = 1,NUMBER_PROBLEM2};
enum {COUNT_FEEDBACK = 0,COUNT_FEEDBACK1,COUNT_FEEDBACK2,COUNT_FEEDBACK3};
void mainMenu();
void secondMenuCandidate();
void secondMenuEmployer();
void signUp();
bool validateId(string &id);
bool validateName(string &name);
bool validateAge(string &age);
bool selectQuestion(string &question, string &answer);
bool validFreeText(string &freeText);
void login();
bool passwordDifficulty(string&);
bool EmployerOrCandidate(char&);
int main()
{
    mainMenu();
    return 0;
}
void mainMenu()
{
    char option;
    bool illegalOption = false;
    while (!illegalOption)
    {
        cout << "JobSearch\n"
                "1. Sign up.\n"
                "2. Login.\n"
                "3. forgot password.\n"
                "4. Exit.\n"
                "Please enter your choice:\n";
        cin >> option;
        switch (option)
        {

            case SIGN_UP:
            {
                signUp();
                break;
            }

            case LOGIN:
            {
                login();
                break;
            }
            case FORGOT_PASSWORD:
            {
                break;
            }
            case EXIT:
            {
                illegalOption = true;
                break;
            }

            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}
void secondMenuCandidate()
{
    char option;

    bool flagForContinue = false;
    while (!flagForContinue)
    {
        cout << "JobSearch\n"
                "1. Look for jobs.\n"
                "2. Create your resume\n"
                "3. View all the jobs you submitted your resume.\n"
                "4. View all the interview invitations you got.\n"
                "5. Edit your profile.\n"
                "6. Log out from the system.\n"
                "Please enter your choice!\n";
        cin >> option;
        switch (option)
        {
            case LOOK_FOR_JOBS:
                break;
            case CREATE_RESUME:
                break;
            case VIEW_JOBS_SUBMITTED:
                break;
            case VIEW_INTERVIEW_INVITATIONS:
                break;
            case EDIT_PROFILE:
                break;
            case LOG_OUT_C:
                flagForContinue = true;
                break;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}
void secondMenuEmployer()
{
    char option;
    cout << "JobSearch\n"
            "1. Publish a job.\n"
            "2. View all the jobs you have already published.\n"
            "3. View all the candidates who submitted their resumes for the jobs you published.\n"
            "4. Log out."
            "Please enter your choice!\n";
    bool flagForContinue = false;
    while (!flagForContinue)
    {
        cin >> option;
        switch (option)
        {
            case PUBLISH_JOB:
                break;
            case VIEW_ALL_JOBS:
                break;
            case VIEW_CANDIDATES_PROFILES:
                break;
            case LOG_OUT_E:
                flagForContinue = true;
                break;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}
void login()
{
    while(true)
    {
        string id, password;
        char exit;
        cout << "dear user please enter your ID:\n";
        cin >> id;
        cout << "please enter your password:\n";
        cin >> password;

//    db check validation
        if(id == "1" && password == "123")
        {
            //check if the user is candidate or emplyer
            //secondMenuCandidate();
            //secondMenuEmployer()
            return;
        }
        cout << "You entered a wrong detail\n";
        cout << "If you want to go back enter the number 1\n";
        cin >> exit;
        if (exit == '1')
            return;
    }
}
void signUp() {
    string id, password, name, question, answer, age,freetext;
    char UserCandidateEmployee;
    cout << "Dear user, please enter your details to sign up.\n";
    if (!validateId(id))
        return;
    if(!passwordDifficulty(password))
        return;
    if(!validateName(name))
        return;
    if(!validateAge(age))
        return;
    if (!selectQuestion(question, answer))
        return;
    if(!validFreeText(freetext))
        return;
    if(!EmployerOrCandidate(UserCandidateEmployee))
        return;
    //privacy policy
    // Perform further operations such as database validation here...add question to db and details to database
    cout << "congratulations you successfully registered!\n";
}
bool validateId(string &id) {
    char get_out;
    while (true) {
        cout << "Please enter your ID (ID must contain 9 digits):\n";
        cin >> id;
        if (id.length() == ID_VALID) {
            bool valid = true;
            for (char c : id) {
                if (c < '0' || c > '9') {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                break;
            }
        }
        cout << "You entered an invalid ID. ID must contain exactly 9 digits. Please try again.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter ID again.\n";
        cin >> get_out;
        if (get_out == '0')
            return false;
    }
    return true;
}
bool validateName(string &name) {
    char get_out;
    while (true) {
        cout << "Please enter your name (must contain only letters and not more than 50 letters):\n";
        cin >> name;
        bool validName = true;
        for (char c : name) {
            if (!isalpha(c)) {
                validName = false;
                break;
            }
        }
        if (validName && name.length() <= 50) {
            break;
        }
        cout << "You entered an invalid name. Name must contain only letters and not exceed 50 characters. Please try again.\n";
        cout << "If you want to return to the main menu, press '0'. Otherwise, press any other character to enter the name again:\n";
        cin >> get_out;
        if (get_out == '0')
            return false;
    }
    return true;
}
bool validateAge(string &age) {
    char get_out;
    while (true) {
        cout << "Please enter your age (18 - 99):\n";
        cin >> age;
        if (age.length() == 2 && age[0] >= '1' && age[0] <= '9' && age[1] >= '0' && age[1] <= '9') {
            int ageNumber = stoi(age);
            if (ageNumber >= 18 && ageNumber <= 99) {
                break;
            }
        }
        cout << "You entered an invalid age. Age must be between 18 - 99. Please try again.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter age again.\n";
        cin >> get_out;
        if (get_out == '0')
            return false;
    }
    return true;
}
bool selectQuestion(string &question, string &answer)
{
    char get_out;
    while (true) {
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
                return false;
            default:
                cout << "You entered an illegal option. Please try again!\n";
                continue;
        }
        cout << "Please enter your answer for \"" << question << "\":\n";
        cin >> answer;
        break;
    }
    return true;
}
bool validFreeText(string &freeText)
{
    cout << "please tell about yourself\n";
    char get_out;
    while(true)
    {
        cin.ignore();
        getline(cin, freeText);
        if (freeText.length() <= 200)
        {
            break;
        }
        cout << "You entered an invalid free text. text must contain not more then 200 characters.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter free text again.\n";
        cin >> get_out;
        if (get_out == '0')
            return false;
        return true;
    }
}
bool passwordDifficulty(string&password)
{
    cout << "Please enter a password (6 to 12 characters, no spaces).\n"
            "Feedback on password difficulty:\n"
            "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
            "- Medium: exactly two types of characters.\n"
            "- Weak: exactly one type of character.\n";
    char get_out = '0';
    while (get_out == '0')
    {
        string feedback;
        bool special_character = false;
        bool lowercase = false;
        bool uppercase = false;
        int count = 0;
        int numberOfproblem = 3;
        cout << "enter the password\n";
        cin >> password;
        if(password.length() < MIN_PASSWORD_SIZE || password.length() > MAX_PASSWORD_SIZE)
            numberOfproblem = NUMBER_PROBLEM2;
        for (int i=0;i < password.length();++i)
        {
            if((password[i] >= 33 && password[i] <= 47)||(password[i] >=58 && password[i] <= 64)||(password[i] >=91 && password[i] <= 96) )
            {
                if(!special_character)
                    ++count;
                special_character = true;
            }
            if(password[i]>=97 && password[i] <= 122)
            {
                if(!lowercase)
                    ++count;
                lowercase = true;
            }
            if(password[i] >=65 && password[i] <=90)
            {
                if(!uppercase)
                    ++count;
                uppercase = true;

            }
            if (password[i] == SPACE)
                numberOfproblem = NUMBER_PROBLEM1;
        }
        if(numberOfproblem == NUMBER_PROBLEM1)
        {
            cout << "you entered a space please enter password again\n ";
            count = -1;
        }
        if(numberOfproblem == NUMBER_PROBLEM2)
        {
            cout << "you entered a not valid password Length\n";
            count = -1;
        }
        if (count == COUNT_FEEDBACK || count == COUNT_FEEDBACK1)
            cout << "The password is weak\n";
        if (count == COUNT_FEEDBACK2)
            cout << "The password is medium\n";
        if (count == COUNT_FEEDBACK3)
            cout << "The password is strong\n";
        cout << "Do you want to change your password?(press 0 for yes/any other character for no/to return to main menu press 1)\n";
        cin >> get_out;
        if(get_out == '1')
        {
            password = "EXIT";
            return false;
        }
    }
    return true;
}
bool EmployerOrCandidate(char &EorC)
{
    char get_out;
    cout << "Please choose who you are: Candidate or Employee (for Employee press 'E', for Candidate press 'C')\n";
    cin >> EorC;
    cin.ignore(); // Clear the newline character from the buffer

    while (true) {
        if (EorC == 'C' || EorC == 'E')
        {
            break;
        }

        cout << "You entered an invalid choice. Please enter 'C' for Candidate or 'E' for Employee.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter who you are (Candidate or Employee) again.\n";

        cin >> get_out;
        cin.ignore();

        if (get_out == '0')
        {
            return false;
        } else
        {
            cout << "Please enter 'C' for Candidate or 'E' for Employee.\n";
            cin >> EorC;
            cin.ignore(); // Clear the newline character from the buffer
        }
    }

    return true;
}