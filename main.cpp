#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

using namespace SQLite;
using namespace std;

enum MainMenu{SIGN_UP = '1', LOGIN,FORGOT_PASSWORD,EXIT};
enum CandidateMenu{LOOK_FOR_JOBS = '1', CREATE_RESUME, VIEW_JOBS_SUBMITTED, VIEW_INTERVIEW_INVITATIONS, EDIT_PROFILE, LOG_OUT_C};
enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,GO_BACK};
enum EmployerMenu{PUBLISH_JOB = '1', VIEW_ALL_JOBS, VIEW_CANDIDATES_PROFILES, LOG_OUT_E};
void mainMenu();
void secondMenuCandidate();
void secondMenuEmployer();
void signUp();
void login();
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
    bool Wrong = false;
    while(!Wrong)
    {
        string id, password;
        char exit;
        cout << "dear user please enter your ID:\n";
        cin >> id;
        cout << "please enter your password:\n";
        cin >> password;

//    db check validation
        if(id == "1" && password == "123") {
            secondMenuCandidate();
            //secondMenuEmployer()
            return;
        }
        cout << "You entered a wrong detail\n";
        cout << "If you want to go back enter the number 1\n";
        cin >> exit;
        if (exit == '1')
        {
            return;
        }
    }


}
void signUp() {
    string questions[] =
            {
                    "A",
                    "B",
                    "C"
            };


    string id, password, name, question, answer;
    cout << "dear user please enter your ID:\n";
    cin >> id;
    cout << "please enter your password:\n";
    cin >> password;
    cout << "please enter your name:\n";
    cin >> name;
    bool flagForContinue = false;
    while (!flagForContinue) {
        char option;
        cout << "please select number of question:\n";
        cout << "1.What is the name of your father?\n";
        cout << "2.What is the name of your mother?\n";
        cout << "3.What is the name of the school you attended.\n";
        cout << "4.What year were you born?\n";
        cout << "5.What is your favorite food?\n";
        cout << "6.Go back to the login/register menu\n";
        cin >> option;
        switch (option) {
            case QUESTION_1:
                break;
            case QUESTION_2:
                break;
            case QUESTION_3:
                break;
            case QUESTION_4:
                break;
            case QUESTION_5:
                break;
            case GO_BACK: {

                return;
            }
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }

    }
}