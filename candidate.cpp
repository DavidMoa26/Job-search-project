//
// Created by david on 22/02/2024.
//
#include <windows.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <string>
#include "employer.h"
#include "candidate.h"
#include "authentication.h"
#include "menus.h"
#include "TableCreation.h"

#define WIDTH 45
#define BORDER_CHAR '|'
#define FILL_CHAR '-'
#define BACK "b"
#define CONTINUE "0"
#define ALL_GOOD "1"
enum DegreesMenu{DOES_NOT_HAVE = '0', BA, MA, PHD};
enum EditingSearchMenu{CHANGE_NAME = '1', CHANGE_AGE, CHANGE_PASSWORD,CHANGE_FREETEXT,CHANGE_QUESTION, BACK_TO_MENU};
enum CategoriesSearchMenu{SEARCH_BY_LOCATION = '1', SEARCH_BY_SCOPE, SEARCH_BY_EXPERIENCE_YEARS, SEARCH_BY_PROFESSION, BACK_TO_LOOK_FOR_JOBS_MENU};
using namespace std;

// Designing
void ChangeColor(int textColor,int backgroundColor)
{
    HANDLE console_color;//Declaring the variable console_color from HANDLE type.
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);//Using a function from the library windows.h that allows access to perform operations on the output.
    SetConsoleTextAttribute(console_color, (textColor + (backgroundColor * 16)));
//Using a function from the library windows.h which changes the output below it.
}
void printLine(int width, char borderChar, char fillChar) {
    cout << setfill(borderChar) << setw(width) << borderChar << endl;
}
void printRow(string text, int width, char borderChar)
{
    int textLength = text.length();
    int fillerLength = width - 4 - textLength; // 4 is for the spaces and borders
    cout << borderChar << "   " << text << setw(fillerLength) << setfill(' ') << " " << borderChar << endl;
}

// Jobs
void ViewAllJobs(Database& db)
{
    if (!JobsListExists(db))
    {
        cout << "JobsList table does not exist.\n";
        return;
    }
    try {
        Statement query(db, "SELECT * FROM jobs_list");
        while (query.executeStep())
        {
            string jobId = query.getColumn(0).getText();
            string companyName = query.getColumn(2).getText();
            string location = query.getColumn(3).getText();
            string position = query.getColumn(4).getText();
            string scope = query.getColumn(6).getText();
            string experience = query.getColumn(7).getText();
            PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
        }
    }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
string SelectJob(Database& db, string& id, vector<string>& filteredJobs)
{
    string jobIdChoice;
    cout << "Select a job - press its number.   |   Back - press 'b'." << endl;
    bool firstIter = true;

    do {
        if (!firstIter)
            cout << "You entered an invalid selection, try again   |   Back - 'b'." << endl;
        firstIter = false;
        getline(cin >> ws, jobIdChoice);
        if (jobIdChoice == "b")
            return "b";
    } while (!CheckIfIdIsDigits(jobIdChoice));

    try {
        if (!filteredJobs.empty())
        {
            for (string &job: filteredJobs)
                if (jobIdChoice == job)
                    break;
            cout << "No jobs found.\n";
            return "ERROR";
        }
        Statement query(db, "SELECT * FROM jobs_list WHERE id = ?");
        query.bind(1, stoi(jobIdChoice));
        if (query.executeStep()) {
            string jobId = query.getColumn(0);
            if (jobId == jobIdChoice) {
                string companyName = query.getColumn(2).getText();
                string location = query.getColumn(3).getText();
                string position = query.getColumn(4).getText();
                string description = query.getColumn(5).getText();
                string scope = query.getColumn(6).getText();
                string experience = query.getColumn(7).getText();
                string salary = query.getColumn(8).getText();
                PrintJobMaxDetails(jobId, companyName, location, position, scope, experience, description, salary);
                return jobId;
            }
        }
        else
        {
            cout << "No jobs found.\n";
            return "ERROR";
        }
    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return "ERROR";
}
char SelectFilter()
{
    while (true) {
        cout << "Please select the filter you want to activate:\n";
        cout << "1. Filter by location.\n"
             << "2. Filter by scope.\n"
             << "3. Filter by experience years.\n"
                "4  Filter by your wanted profession.\n"
                "5  Back.\n";
        char option = UserChoice();
        switch (option)
        {
            case SEARCH_BY_LOCATION:
                return SEARCH_BY_LOCATION;
            case SEARCH_BY_SCOPE:
                return SEARCH_BY_SCOPE;
            case SEARCH_BY_EXPERIENCE_YEARS:
                return SEARCH_BY_EXPERIENCE_YEARS;
            case SEARCH_BY_PROFESSION:
                return SEARCH_BY_PROFESSION;
            case BACK_TO_LOOK_FOR_JOBS_MENU:
                return BACK_TO_LOOK_FOR_JOBS_MENU;
            default:
                cout << "You entered an illegal option. Please try again!\n";
        }
    }
}
char AnswerTheFilter(vector<string>& answerForFilter)
{
    char option = SelectFilter();
    switch (option)
    {
        case SEARCH_BY_LOCATION: {
            string locationChoice;
            if (EnterTillValid(locationChoice, "location") == BACK)
                return BACK_TO_LOOK_FOR_JOBS_MENU;
            answerForFilter.push_back(locationChoice);
            return SEARCH_BY_LOCATION;
        }
        case SEARCH_BY_SCOPE: {
            string scopeChoice = SelectScope();
            answerForFilter.push_back(scopeChoice);
            return SEARCH_BY_SCOPE;
        }
        case SEARCH_BY_EXPERIENCE_YEARS: {
            string minYearsOfExperience, maxYearsOfExperience;
            bool firstIter = true;
            do {
                if (!firstIter)
                    cout << "The min of years can't be bigger than the max of year!" << endl;
                firstIter = false;
                cout << "Please enter the min of experience years (0 - 50)   |   Back - 'b':\n";
                if (EnterYearsOfExperienceTillValid(minYearsOfExperience) == BACK)
                    return BACK_TO_LOOK_FOR_JOBS_MENU;
                cout << "Please enter the max of experience years (0 - 50)   |   Back - 'b':\n";
                if (EnterYearsOfExperienceTillValid(maxYearsOfExperience) == BACK)
                    return BACK_TO_LOOK_FOR_JOBS_MENU;
            } while (minYearsOfExperience > maxYearsOfExperience);
            answerForFilter.push_back(minYearsOfExperience);
            answerForFilter.push_back(maxYearsOfExperience);
            return SEARCH_BY_EXPERIENCE_YEARS;
        }
        case SEARCH_BY_PROFESSION: {
            string professionChoice;
            if (EnterTillValid(professionChoice, "profession") == BACK)
                return BACK_TO_LOOK_FOR_JOBS_MENU;
            answerForFilter.push_back(professionChoice);
            return SEARCH_BY_PROFESSION;
        }
        case BACK_TO_LOOK_FOR_JOBS_MENU:
            return BACK_TO_LOOK_FOR_JOBS_MENU;
        default:
            cout << "You entered an illegal option. Please try again!\n";
    }
}
string SearchByCategory(Database& db, vector<string>& filteredJobs)
{
    if (!JobsListExists(db))
    {
        cout << "JobsList table does not exist.\n";
        return "ERROR";
    }
    try {
        vector<string> answerForFilter;
        char choice = AnswerTheFilter(answerForFilter);;
        if (choice == BACK_TO_LOOK_FOR_JOBS_MENU)
            return "b";
        Statement query(db, "SELECT * FROM jobs_list");
        bool flagIfAnyJobWasPrinted = false;
        while (query.executeStep()) {
            string jobId = query.getColumn(0).getText();
            string companyName = query.getColumn(2).getText();
            string location = query.getColumn(3).getText();
            string position = query.getColumn(4).getText();
            string scope = query.getColumn(6).getText();
            string experience = query.getColumn(7).getText();
            if (choice == SEARCH_BY_LOCATION)
                if (location == answerForFilter[0])
                {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
            if (choice == SEARCH_BY_SCOPE)
                if (scope == answerForFilter[0])
                {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
            if (choice == SEARCH_BY_EXPERIENCE_YEARS)
            {
                int experienceInt = stoi(experience);
                if (experienceInt >= stoi(answerForFilter[0]) && experienceInt <= stoi(answerForFilter[1]))
                {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
            }
            if (choice == SEARCH_BY_PROFESSION)
                if (position == answerForFilter[0]) {
                    filteredJobs.push_back(jobId);
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                    flagIfAnyJobWasPrinted = true;
                }
        }
        if (!flagIfAnyJobWasPrinted)
        {
            cout << "No jobs found.\n";
            return "ERROR";
        }
    }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return "ALL_GOOD";
}
void PrintJobMinDetails(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience)
{
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow(jobId, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow("Position: " + position, WIDTH, BORDER_CHAR);
    printRow("Location: " + location, WIDTH, BORDER_CHAR);
    printRow("Company: " + companyName, WIDTH, BORDER_CHAR);
    printRow("Scope: " + scope, WIDTH, BORDER_CHAR);
    printRow("Experience years: " + experience, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
}
void PrintJobMaxDetails(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience, string& description, string& salary)
{
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow(jobId, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
    printRow("Position: " + position, WIDTH, BORDER_CHAR);
    printRow("Location: " + location, WIDTH, BORDER_CHAR);
    printRow("Company: " + companyName, WIDTH, BORDER_CHAR);
    printRow("Scope: " + scope, WIDTH, BORDER_CHAR);
    printRow("Experience years: " + experience, WIDTH, BORDER_CHAR);
    printRow("Description : " + description, WIDTH, BORDER_CHAR);
    printRow("Salary: " + salary, WIDTH, BORDER_CHAR);
    printLine(WIDTH, FILL_CHAR, BORDER_CHAR);
}
bool CheckIfOnlyLettersOrSpace(string& name)
{
    for (char c : name)
    {   if(c == ' ')
        {
            continue;
        }
        else if (!isalpha(c) )
            return false;
    }
    return true;
}
bool strIsValid(string &str, const string& strMessage) {
    if(str.length() == 0 || str.length() >= 50 ||   !CheckIfOnlyLettersOrSpace(str))
    {
        cout << "You entered an invalid " << strMessage << "(Must contain only letters and not exceed 50 characters).\n"
             << "Please try again   |   Back - 'b'.\n";
        return false;
    }
    return true;
}

string EnterTillValid(string& str, const string& strMessage)
{
    cout << "Please enter your " << strMessage << " (must contain only letters and not more than 50 letters)   |   Back - 'b':\n";
    do {
        getline(cin >> ws, str);
        if (str == BACK)
            return BACK;
    } while (!strIsValid(str, strMessage));
    return ALL_GOOD;
}
bool validateYearsOfExperience(string & years_of_experience)
{
    if(!CheckIfIdIsDigits(years_of_experience))
    {
        cout << "You entered an invalid years of experience(Must be between 0 - 50) Please try again.\n";
        return false;
    }
    int ageNumber = stoi(years_of_experience);
    if(ageNumber < 0 || ageNumber > 50 || !NotValidSpace(years_of_experience))
    {
        cout << "You entered an invalid years of experience(Must be between 0 - 50) Please try again.\n";
        return false;
    }
    return true;
}
string EnterYearsOfExperienceTillValid(string& years_of_experience)
{
    do {
        getline(cin >> ws, years_of_experience);
        if (years_of_experience == BACK)
            return BACK;
    } while (!validateYearsOfExperience(years_of_experience));
    return ALL_GOOD;
}
//***********

// Resumes
void CreateResume(Database& db, string& id)
{
    string choice;
    cout << "Create your resume - press 'r'.   |   Back - press 'b'." << endl;
    getline(cin >> ws,choice);
    if (choice == "b")
        return;
    string full_name, age, degree1, degree2, degree3, work_experience, years_of_experience;
    //FullName
    if(EnterTillValid(full_name, "full name") == BACK)
        return;

    //Age
    cout << "Please enter your experience years (0 - 50)   |   Back - 'b':\n";
    if (EnterAgeTillValid(age) == BACK)
        return;
    //Degrees
    int degreesNumber = SelectDegree();
    if (degreesNumber >= 1 && degreesNumber <= 3)
        if(EnterTillValid(full_name, "Bachelor's degree (B.A.)") == BACK)
            return;
    if (degreesNumber >= 2 && degreesNumber <= 3)
        if(EnterTillValid(full_name, "Master's degree (M.A.)") == BACK)
            return;
    if (degreesNumber == 3)
        if(EnterTillValid(full_name, "Ph.D degree") == BACK)
            return;
    //WorkExperience
    if (EnterFreeTextTillValid(work_experience) == BACK)
        return;
    //YearsOfExperience
    if(EnterYearsOfExperienceTillValid(years_of_experience) == BACK)
        return;

    string confirm;
    do {
        cout << "Confirm - press 'c'   |   Back - press 'b'." << endl;
        getline(cin >> ws,confirm);
        if (confirm == "b")
            return;
    } while (confirm != "c");

    if (!ResumesTableExists(db))
        CreateResumeTable(db);
    InsertResumeToTable(db, id, full_name, age, degree1, degree2, degree3, work_experience, years_of_experience);
}
int SelectDegree(){
    while (true) {
        cout << "Please select which degree you have, if any:\n";
        cout << "0. You don't have any degree.\n"
             << "1. Bachelor's degree (B.A.).\n"
             << "2. Master's degree (M.A.).\n"
             << "3. PhD.\n";
        char option = UserChoice();
        switch (option)
        {
            case DOES_NOT_HAVE:
                return 0;
            case BA:
                return 1;
            case MA:
                return 2;
            case PHD:
                return 3;
            default:
                cout << "You entered an illegal option. Please try again!\n";
                continue;
        }
    }
}


// Resume submissions
void SubmitResume(Database& db, string& id, string& jobId)
{
    string choice;
    do {
        cout << "Submit your resume - press 's'   |   Back - press 'b'." << endl;
        getline(cin >> ws,choice);
        if (choice == "b")
            return;
    } while (choice != "s");

    if (!ResumeSubmissionsTableExists(db))
        CreateResumeSubmissionsTable(db);
    InsertSubmitToTable(db, id, jobId);
}
void ViewAllSubmittedJobs(Database& db, string& candidate_id)
{
    if (!ResumeSubmissionsTableExists(db))
    {
        cout << "Submissions table does not exist.\n";
        return;
    }
    try {
        Statement query1(db, "SELECT * FROM submission WHERE candidate_id = ?");
        query1.bind(1, stoi(candidate_id));
        bool flagIfAnyJobWasPrinted = false;
        while (query1.executeStep())
        {
            string submittedJobId = query1.getColumn(0).getText();
            Statement query2(db, "SELECT * FROM jobs_list");
            while (query2.executeStep())
            {
                string jobId = query2.getColumn(0).getText();
                if (jobId == submittedJobId)
                {
                    flagIfAnyJobWasPrinted = true;
                    string companyName = query2.getColumn(2).getText();
                    string location = query2.getColumn(3).getText();
                    string position = query2.getColumn(4).getText();
                    string scope = query2.getColumn(6).getText();
                    string experience = query2.getColumn(7).getText();
                    PrintJobMinDetails(jobId, companyName, location, position, scope, experience);
                }
            }
        }
        if (!flagIfAnyJobWasPrinted)
            cout << "No jobs found.\n";
    }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}

//Edit profile
void editName(Database& db,  string& id)
{
    string newName;
    if (EnterNameTillValid(newName) == "b")
        return;
    try {
        db.exec("UPDATE users SET name = '" + newName + "' WHERE id = '" + id + "'");
        cout << "Name updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editAge(Database& db,  string& id) {
    string newAge;
    if (EnterAgeTillValid(newAge) == "b")
        return;
    try {
        db.exec("UPDATE users SET age = " + newAge + " WHERE id = '" + id + "'");
        cout << "Age updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editPassword(Database& db,  string& id)
{
    string newPassword;
    if (EnterPasswordTillValid(newPassword) == "b")
        return;
    try {
        db.exec("UPDATE users SET password = '" + newPassword + "' WHERE id = '" + id + "'");
        cout << "Password updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editFreeText(Database& db,  string& id) {
    string newFreetext;
    if (EnterFreeTextTillValid(newFreetext) == "b")
        return;
    try {
        // Update the database with the new free text
        db.exec("UPDATE users SET freetext = '" + newFreetext + "' WHERE id = '" + id + "'");
        cout << "Free text updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editQuestion(Database& db,  string& id)
{
    string newQuestion, newAnswer;
    if (selectQuestion(newQuestion, newAnswer) == "b")
        return;
    string query = "UPDATE forgot_password SET question = '" + newQuestion + "', answer = '" + newAnswer + "' WHERE id = '" + id + "'";
    try {
        db.exec(query);
        cout << "Question and answer updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editProfile(Database& db, string& id) {
    char option;
    while (true)
    {
        cout << "Choose an option to edit your profile:\n"
                "1. Edit name.\n"
                "2. Edit age.\n"
                "3. Edit password.\n"
                "4. Edit Freetext.\n"
                "5. Edit questions.\n"
                "6. Back.\n";
        option = UserChoice();
        switch (option)
        {
            case CHANGE_NAME:
                editName(db, id);
                break;
            case CHANGE_AGE:
                editAge(db, id);
                break;
            case CHANGE_PASSWORD:
                editPassword(db, id);
                break;
            case CHANGE_FREETEXT:
                editFreeText(db, id);
                break;
            case CHANGE_QUESTION:
                editQuestion(db, id);
                break;
            case BACK_TO_MENU:
                return;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}
//Invitations
void RejectAcceptInterviewInvitation(Database&db,string&id)
{
    while (true)
    {
        try {
            // Select data from submission table where status is accepted and candidate_id matches the provided id
            Statement selectQuery(db, "SELECT job_id FROM submission WHERE status = 'accepted' AND candidate_id = ?;");

            // Bind the candidate_id parameter
            selectQuery.bind(1, id);

            // Execute the query
            while (selectQuery.executeStep()) {
                string job_id = selectQuery.getColumn(0).getText();
                cout << "You have an interview invitation for job id:" << job_id << endl;
            }
        } catch (const exception &e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }
        string job_id;
        cout << "select a job id you want to accept/reject the invitation   |    Back - 'b'.\n";
        do{
            getline(cin >> ws, job_id);
            if (job_id == BACK)
                return;
        } while(!CheckIfIdIsDigits(job_id));
        char choice;
        cout << "1. Accept" << endl;
        cout << "2. Reject" << endl;
        cout << "3. Go Back" << endl;
        choice = UserChoice();
        string status;
        bool flagToContinue = true;
        while (flagToContinue) {
            switch (choice) {
                case '1':
                    status = "accept";
                    flagToContinue = false;
                    break;
                case '2':
                    status = "rejected";
                    flagToContinue = false;
                    break;
                case '3':
                    return;
                default:
                    cout << "Invalid choice. Please enter '1' or '2' or '3'." << endl;
            }
        }

        try
        {
            // Update submission status based on choice
            db.exec("UPDATE submission SET status = '" + status + "' WHERE job_id = '" + job_id + "' AND candidate_id = '" + id + "'");

            cout << "Invitation " << status << endl;

        } catch(exception& e)
        {
            cerr << "SQLite exception: " << e.what() << endl;
        }
    }
}

