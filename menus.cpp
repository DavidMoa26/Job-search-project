#include "menus.h"
#include "authentication.h"
#include "employer.h"
#include "candidate.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include "iostream"
using namespace SQLite;
using namespace std;

enum SearchMenu{VIEW_ALL_JOBS = '1', SEARCH_BY_CATEGORY, BACK_TO_CANDIDATE_MENU};
enum CandidateMenu{LOOK_FOR_JOBS = '1', CREATE_RESUME, VIEW_JOBS_SUBMITTED, VIEW_INTERVIEW_INVITATIONS, EDIT_PROFILE, LOG_OUT_C};
enum EmployerMenu{PUBLISH_JOB = '1',DELETE_JOB,EDIT_JOB, VIEW_ALL_JOBS_YOU_PUBLISHED, SEND_QUESTION,FILTER,SEND_INVITATION,VIEW_INVITATION, LOG_OUT_E};
enum MainMenu{REGISTER = '1', LOGIN,FORGOT_PASSWORD,EXIT};


void CandidateMenu(Database& db , string& id)
{
    char option;
    while (true) {

        bool flagForContinue = false;
        while (!flagForContinue) {
            cout << "1. Look for jobs.\n"
                    "2. Create your resume.\n"
                    "3. View all the jobs you submitted your resume.\n"
                    "4. Edit your profile.\n"
                    "5. View all the interview invitations you got.\n"
                    "6. Log out from the system.\n"
                    "Please enter your choice : \n";
            cin >> option;
            switch (option) {
                case LOOK_FOR_JOBS:
                    LookForJobsMenu(db, id);
                    break;
                case CREATE_RESUME:
                    CreateResume(db, id);
                    break;
                case VIEW_JOBS_SUBMITTED:
                    ViewAllSubmittedJobs(db, id);
                    break;
                case EDIT_PROFILE:
                    break;
                case VIEW_INTERVIEW_INVITATIONS:
                    break;
                case LOG_OUT_C:
                    return;
                default:
                    cout << "You enter an illegal option, please try again!" << endl;
            }
        }
    }
}
void EmployerMenu(Database& db, string& id) {
    char option;
    bool flagForContinue = false;
    while (!flagForContinue)
    {
        cout << "1. Publish a job.\n"
                "2. Delete a job.\n"
                "3. Edit a job.\n"
                "4. View all the jobs you have already published.\n"
                "5. Send question for candidate to test him .\n"
                "6. Filter job for minimum years of experience..\n"
                "7. Send an invitation to a candidate the submitted their resume\n"
                "8. View all the interview invitations the employer has send\n"
                "9. Log out. \n"
                "Please enter your choice!\n";

        switch (option)
        {
            case PUBLISH_JOB:
                PostJob(db,id);
                break;
            case DELETE_JOB:
                DeleteJob(db,id);
                break;
            case EDIT_JOB:
                EditJob(db,id);
                break;
            case VIEW_ALL_JOBS_YOU_PUBLISHED:
                FetchAllJobs(db,id);
                break;
            case SEND_QUESTION:
                break;
            case FILTER:
                FillterCandidateResume(db,id);
                break;
            case SEND_INVITATION:
                SendInterviewInvitation(db,id);
                break;
            case VIEW_INVITATION:
                ViewAllInterviewInvitation(db,id);
                break;
            case LOG_OUT_E:
                flagForContinue = true;
                break;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }
}
void MainMenu(Database& db)
{
    char option;
    bool illegalOption = false;
    while (!illegalOption)
    {
        cout << "1. Register.\n"
                "2. Login.\n"
                "3. Forgot password.\n"
                "4. Exit.\n"
                "Please enter your choice:\n";
        cin >> option;
        switch (option)
        {

            case REGISTER:
            {
                Register(db);
                break;
            }

            case LOGIN:
            {
                string result = Login(db);

                if (result == "ERROR")

                while(result == "ERROR")
                    result = Login(db);
                if(result == "RETURN")
                    break;
                string role = GetUserRole(db,result);
                if(role == "employer")
                    EmployerMenu(db,result);
                else
                    CandidateMenu(db,result);
                break;
            }
            case FORGOT_PASSWORD:
            {
                ForgotPassword(db);
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
void LookForJobsMenu(Database& db, string& id)
{
    char option;
    while (true) {
        cout << "1. View all the jobs.\n"
                "2. Search by category.\n"
                "3. Back.\n";
        cin >> option;
        switch (option)
        {
            case VIEW_ALL_JOBS:
            {
                ViewAllJobs(db);
                string jobId = SelectJob(db, id);
                if (jobId != "ERROR" && jobId != "b")
                    SubmitResume(db, id, jobId);
                break;
            }
            case SEARCH_BY_CATEGORY:
                SearchByCategory(db);
                break;
            case BACK_TO_CANDIDATE_MENU:
                return;
            default:
                cout << "You enter an illegal option, please try again!" << endl;
        }
    }

}