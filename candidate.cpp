//
// Created by david on 22/02/2024.
//
#include <windows.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <iomanip>
#include <string>
#include "employer.h"
#include "candidate.h"
#define WIDTH 45
#define BORDER_CHAR '|'
#define FILL_CHAR '-'
enum DegreesMenu{DOES_NOT_HAVE = '0', BA, MA, PHD};
enum CategoriesSearchMenu{SEARCH_BY_LOCATION = '1', SEARCH_BY_SCOPE, SEARCH_BY_EXPERIENCE_YEARS, SEARCH_BY_PROFESSION, BACK_TO_LOOK_FOR_JOBS_MENU};
using namespace std;

// Designing
void ChangeColor(int color)
{
    HANDLE console_color;//Declaring the variable console_color from HANDLE type.
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);//Using a function from the library windows.h that allows access to perform operations on the output.
    SetConsoleTextAttribute(console_color, color);//Using a function from the library windows.h which changes the output below it.
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
                PrintJob(jobId, companyName, location, position, scope, experience);
            }
        }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
string SelectJob(Database& db, string& id)
{
    string choice;
    cout << "Select a job - press its number.   |   Back - press 'b'." << endl;
    cin.ignore();
    getline(cin, choice);
    if (choice == "b")
        return "b";
    try {
        Statement query(db, "SELECT * FROM jobs_list WHERE id = ?");
        query.bind(1, stoi(choice));
        if (query.executeStep())
        {
            string jobId = query.getColumn(0);
            if (jobId == choice)
            {
                string companyName = query.getColumn(2).getText();
                string location = query.getColumn(3).getText();
                string position = query.getColumn(4).getText();
                string description = query.getColumn(5).getText();
                string scope = query.getColumn(6).getText();
                string experience = query.getColumn(7).getText();
                string salary = query.getColumn(8).getText();
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
                return jobId;
            }
        } else {
            cout << "No jobs found.\n";
            return "ERROR";
        }
    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return "ERROR";
}
string* SelectFilter(Database& db)
{
    while (true) {
        cout << "Please select the filter you want to activate:\n";
        cout << "1. Filter by location.\n"
             << "2. Filter by scope.\n"
             << "3. Filter by experience years.\n"
                "4  Filter by your wanted profession.\n"
                "5  Back.\n";
        char option;
        cin >> option;
        switch (option)
        {
            case SEARCH_BY_LOCATION:
            {
                string* filterAndChoice;
                filterAndChoice = new string[2];
                filterAndChoice[0] = "1";
                cout << "Please enter the location:" << endl;
                cin >> filterAndChoice[1];
                return filterAndChoice;
            }
            case SEARCH_BY_SCOPE:
            {
                string* filterAndChoice;
                filterAndChoice = new string[2];
                filterAndChoice[0] = "2";
                string filterScope = SelectScope();
                filterAndChoice[1] = filterScope;
                return filterAndChoice;
            }
            case SEARCH_BY_EXPERIENCE_YEARS:
            {
                string* filterAndChoice;
                filterAndChoice = new string[3];
                filterAndChoice[0] = "3";
                cout << "Please enter the min of experience years:" << endl;
                cin >> filterAndChoice[1];
                cout << "Please enter the max of experience years:" << endl;
                cin >> filterAndChoice[2];
                return filterAndChoice;
            }
            case SEARCH_BY_PROFESSION:
            {
                string* filterAndChoice;
                filterAndChoice = new string[2];
                filterAndChoice[0] = "4";
                cout << "Please enter the profession:" << endl;
                cin >> filterAndChoice[1];
                return filterAndChoice;
            }
            case BACK_TO_LOOK_FOR_JOBS_MENU:
                string* filterAndChoice;
                filterAndChoice = new string[1];
                filterAndChoice[0] = "5";
                return filterAndChoice;
            default:
                cout << "You entered an illegal option. Please try again!\n";
                continue;
        }
    }
}
void SearchByCategory(Database& db)
{

    if (!JobsListExists(db))
    {
        cout << "JobsList table does not exist.\n";
        return;
    }
    try {
        string* filterAndChoice = SelectFilter(db);
        if (filterAndChoice[0] == "5")
            return;
        Statement query(db, "SELECT * FROM jobs_list");
        if(query.executeStep())
        {
            bool flagIfAnyJobWasPrinted = false;
            while (query.executeStep())
            {
                string jobId = query.getColumn(0).getText();
                string companyName = query.getColumn(2).getText();
                string location = query.getColumn(3).getText();
                string position = query.getColumn(4).getText();
                string scope = query.getColumn(6).getText();
                string experience = query.getColumn(7).getText();
                if (filterAndChoice[0] == "1")
                {
                    if (location == filterAndChoice[1])
                    {
                        PrintJob(jobId, companyName, location, position, scope, experience);
                        flagIfAnyJobWasPrinted = true;
                    }
                }
                if (filterAndChoice[0] == "2")
                {
                    if (scope == filterAndChoice[1])
                    {
                        PrintJob(jobId, companyName, location, position, scope, experience);
                        flagIfAnyJobWasPrinted = true;
                    }                }
                if (filterAndChoice[0] == "3")
                {
                    int experienceInt = stoi(experience);
                    if (experienceInt >= stoi(filterAndChoice[1]) && experienceInt <= stoi(filterAndChoice[2]))
                    {
                        PrintJob(jobId, companyName, location, position, scope, experience);
                        flagIfAnyJobWasPrinted = true;
                    }                }
                if (filterAndChoice[0] == "4")
                {

                    if (position == filterAndChoice[1])
                    {
                        PrintJob(jobId, companyName, location, position, scope, experience);
                        flagIfAnyJobWasPrinted = true;
                    }
                }
                if (!flagIfAnyJobWasPrinted)
                {
                    cout << "No jobs found.\n";
                    return;
                }
            }
        }
        else
        {
            cout << "No jobs found.\n";
            return;
        }
    }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void PrintJob(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience)
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

// Resumes
bool ResumesTableExists(Database& db)
{
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='resumes';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return false;
}
void CreateResumeTable(Database& db)
{
    try {
        db.exec("CREATE TABLE IF NOT EXISTS resumes ("
                "candidate_id INTEGER PRIMARY KEY UNIQUE,"
                "full_name NOT NULL,"
                "age INTEGER NOT NULL,"
                "degree1,"
                "degree2,"
                "degree3,"
                "work_experience NOT NULL,"
                "years_of_experience INTEGER NOT NULL);");
        cout << "resumes table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void InsertResumeToTable(Database& db, string& id, string& full_name, string& age, string& degree1, string& degree2, string& degree3, string& work_experience, string& years_of_experience)
{
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO resumes (candidate_id, full_name, age, degree1, degree2, degree3, work_experience, years_of_experience) VALUES (?, ?, ?, ?, ?, ?, ?, ?);");

        // Bind values to the statement
        query.bind(1, stoi(id));
        query.bind(2, full_name);
        query.bind(3, age);
        query.bind(4, degree1);
        query.bind(5, degree2);
        query.bind(6, degree3);
        query.bind(7, work_experience);
        query.bind(8, years_of_experience);

        // Execute the statement
        query.exec();
        cout <<"resume successfully added to the database.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateResume(Database& db, string& id)
{
    string choice;
    cout << "Create your resume - press 'r'.   |   Back - press 'b'." << endl;
    cin >> choice;
    if (choice == "b")
        return;
    string full_name, age, degree1, degree2, degree3, work_experience, years_of_experience;
    cout << "Enter your full name : \n";
    cin >> full_name;
    while(full_name.empty())
    {
        cout << "Full name cannot be empty. Please try again:\n";
        cin >> full_name;
    }
    cout << "Enter your age : \n";
    cin >> age;
    while(age.empty()) {
        cout << "Age cannot be empty. Please try again:\n";
        cin >> age;
    }
    //degrees
    int degreesNumber = SelectDegree();
    if (degreesNumber >= 1 && degreesNumber <=3)
    {
        do{
            cout << "Enter the name of your Bachelor's degree (B.A.):" << endl;
            cin >> degree1;
        } while (degree1.empty());
    }
    if (degreesNumber >= 2 && degreesNumber <=3)
    {
        do {
            cout << "Enter the name of your  Master's degree (M.A.):" << endl;
            cin >> degree2;
        } while (degree2.empty());
    }
    if (degreesNumber == 3)
    {
        do{
            cout << "Enter the name of your Ph.D degree:" << endl;
            cin >> degree3;
        } while (degree3.empty());
    }
    cout << "Enter your work experience : \n";
    cin >> work_experience;
    while(work_experience.empty()) {
        cout << "Work experience cannot be empty. Please try again:\n";
        cin >> work_experience;
    }
    cout << "Enter the years of experience you have in the work : \n";
    cin >> years_of_experience;
    while(years_of_experience.empty()) {
        cout << "Years of experience cannot be empty. Please try again:\n";
        cin >> years_of_experience;
    }
    string confirm;
    do {
        cout << "Confirm - press 'c'   |   Back - press 'b'." << endl;
        cin >> confirm;
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
        char option;
        cin >> option;
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
bool ResumeSubmissionsTableExists(Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='submission';");
        // Execute the query
        if (query.executeStep()) {
            return true;
        } else {
            return false;
        }
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return false;
}
void CreateResumeSubmissionsTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS submission ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "job_id INTEGER,"
                "candidate_id INTEGER NOT NULL,"
                "status TEXT DEFAULT 'pending',"
                "FOREIGN KEY(job_id) REFERENCES jobs_list(id),"
                "FOREIGN KEY(candidate_id) REFERENCES users(id));");
        cout << "resume_submissions table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void InsertSubmitToTable(Database& db, string& candidate_id, string& jobId)
{
    if(!ResumeSubmissionsTableExists(db)) {
        CreateResumeSubmissionsTable(db);
    }
    try {
        // Prepare a SQL insert statement
        Statement query1(db, "SELECT * FROM jobs_list WHERE id = ?" );
        query1.bind(1, stoi(jobId));
        Statement query2(db, "SELECT * FROM resumes WHERE candidate_id = ?");
        query2.bind(1, stoi(candidate_id));
        if (query2.executeStep())
        {
            if (query1.executeStep())
            {
                string employerId = query1.getColumn(1).getText();
                Statement query3(db, "INSERT INTO submission (job_id, candidate_id) VALUES (?,?);");

                // Bind values to the statement
                query3.bind(1, stoi(jobId));
                query3.bind(2, stoi(candidate_id));

                // Execute the statement
                query3.exec();
                cout <<"resume successfully added to the database.\n";
            }
        }
        else
        {
            string choice;
            cout << "You didn't create a resume!" << endl;
            CreateResume(db, candidate_id);
        }

    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void SubmitResume(Database& db, string& id, string& jobId)
{
    string choice;
    do {
        cout << "Submit your resume - press 's'   |   Back - press 'b'." << endl;
        cin >> choice;
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
        cout << "ResumeSubmissions table does not exist.\n";
        return;
    }
    try {
        Statement query1(db, "SELECT * FROM resume_submissions WHERE candidate_id = ?");
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
                    PrintJob(jobId, companyName, location, position, scope, experience);
                }
            }
            if (!flagIfAnyJobWasPrinted)
                cout << "No jobs found.\n";
        }
    }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}