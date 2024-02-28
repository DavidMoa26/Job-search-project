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
#include "authentication.h"
#define WIDTH 45
#define BORDER_CHAR '|'
#define FILL_CHAR '-'
enum EditingSearchMenu{CHANGE_NAME = '1', CHANGE_AGE, CHANGE_PASSWORD,CHANGE_FREETEXT,CHANGE_QUESTION, BACK_TO_MENU};
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
                "4. Filter by your wanted profession.\n"
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
        cout << "Resume Submissions table does not exist.\n";
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
void ViewAllInterviewQuestions(Database& db, string& candidate_id) {
    string question,answer1,answer2,answer3,answer4,employer_id,employer_name,question_id , question_choice,answer_choice;
    bool flag_query1 = false;
    bool flag_query2 = false;
    try {
        Statement query1(db, "SELECT question, answer1, answer2, answer3, answer4, employer_id, id FROM tests WHERE candidate_id = ? AND grade IS NULL");
        query1.bind(1, stoi(candidate_id));
        while (query1.executeStep()) {
            flag_query1 = true;
            question = query1.getColumn(0).getText();
            answer1 = query1.getColumn(1).getText();
            answer2 = query1.getColumn(2).getText();
            answer3 = query1.getColumn(3).getText();
            answer4 = query1.getColumn(4).getText();
            employer_id = query1.getColumn(5).getText();
            question_id = query1.getColumn(6).getText();

            cout << "Question number : " <<question_id << "\n" << question <<"\n"
                 << "1. "<< answer1 << "\n"
                 << "2. "<< answer2 << "\n"
                 << "3. "<< answer3 << "\n"
                 << "4. "<< answer4 << "\n"
                 << " ===================================== \n \n";
        }
        if (!flag_query1) {
            cout << "No interview questions.\n";
            return;
        }

        cout << "Enter number of question you want answer : \n";
        cin >> question_choice;
        try {
            Statement query2(db,"SELECT id,correct_answer FROM tests WHERE id = ?");
            query2.bind(1, stoi(question_choice));
            while (query2.executeStep()) {
                flag_query2 = true;
                string storedCorrectAnswer = query2.getColumn(1).getText();
                cout << "Enter the answer 1-4 : \n";
                cin >> answer_choice;
                if(storedCorrectAnswer == answer_choice) {
                    int grade = 100;
                    Statement query3(db,"UPDATE tests SET grade = ? WHERE id = ?");
                    query3.bind(1, grade);
                    query3.bind(2, stoi(question_choice));
                    query3.exec();
                    cout << "Correct\n";
                }
                else {
                    int grade = 0;
                    Statement query3(db,"UPDATE tests SET grade = ? WHERE id = ?");
                    query3.bind(1, grade);
                    query3.bind(2, stoi(question_choice));
                    query3.exec();
                    cout << "Wrong\n";
                }
            }
            if(!flag_query2) {
                cout << "Wrong question choice\n";
            }
        } catch(exception & e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }

    }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
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

        cout << "select a job id you want to accept/reject the invitation or press B to go back\n";
        string job_id;
        cin >> job_id;
        if(job_id == "B" || job_id == "b")
            return;
        char choice;
        cout << "1. Accept" << endl;
        cout << "2. Reject" << endl;
        cout << "3. Go Back" << endl;
        cin >> choice;
        choice = tolower(choice); // Convert choice to lowercase

        string status;
        if (choice == '1')
            status = "accept";
        else if (choice == '2')
            status = "rejected";
        else if (choice == '3')
            break;
        else
        {
            cout << "Invalid choice. Please enter '1' or '2' or '3'." << endl;
            continue; // Restart the loop
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
void editName(Database& db,  string& id) {
    string newName;
    cout << "Please enter your name (must contain only letters and not more than 50 letters):\n";
    cin >> newName;
    while (!validateName(newName)) {
        cout << "You entered an invalid name. Name must contain only letters and not exceed 50 characters. Please try again.\n";
        cout << "If you want to return to the main menu, press '0'. Otherwise, press any other character to enter the name again:\n";
        string get_out;
        cin >> get_out;
        if (get_out == "0")
            return;
        cout << "Please enter your name (must contain only letters and not more than 50 letters):\n";
        cin >> newName;
    }
    try {
        db.exec("UPDATE users SET name = '" + newName + "' WHERE id = '" + id + "'");
        cout << "Name updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editAge(Database& db,  string& id) {
    string newAge;
    cout << "Please enter your age (18 - 99):\n";
    cin >> newAge;
    while (!validateAge(newAge)) {
        cout << "You entered an invalid age. Age must be between 18 - 99. Please try again.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter age again.\n";
        string get_out;
        cin >> get_out;
        if (get_out == "0")
            return;
        cout << "Please enter your age (18 - 99):\n";
        cin >> newAge;
    }
    try {
        db.exec("UPDATE users SET age = " + newAge + " WHERE id = '" + id + "'");
        cout << "Age updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editPassword(Database& db,  string& id,string&get_out)
{
    string newPassword;
    cout << "Please enter a password (6 to 12 characters, no spaces).\n"
            "Feedback on password difficulty:\n"
            "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
            "- Medium: exactly two types of characters.\n"
            "- Weak: exactly one type of character.\n";
    cin >> newPassword;
    while (!validPassword(newPassword))
    {
        if (get_out == "1")
            return;
        cout << "Please enter a password (6 to 12 characters, no spaces).\n"
                "Feedback on password difficulty:\n"
                "- Strong: at least three of lowercase, uppercase, digits, special characters.\n"
                "- Medium: exactly two types of characters.\n"
                "- Weak: exactly one type of character.\n";
        cin >> newPassword;
    }
    try {
        db.exec("UPDATE users SET password = '" + newPassword + "' WHERE id = '" + id + "'");
        cout << "Password updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editFreeText(Database& db,  string& id) {
    string freetext;
    cout << "Please tell about yourself\n";
    cin.ignore();
    getline(cin, freetext);
    while (!validFreeText(freetext)) {
        cout << "You entered an invalid free text. Text must contain not more than 200 characters.\n";
        cout << "Press '0' to return to the main menu.\n"
             << "Press any other character to enter free text again.\n";
        string get_out;
        cin >> get_out;
        if (get_out == "0")
            return;
        cout << "Please tell about yourself\n";
        cin.ignore();
        getline(cin, freetext);
    }
    try {
        // Update the database with the new free text
        db.exec("UPDATE users SET freetext = '" + freetext + "' WHERE id = '" + id + "'");
        cout << "Free text updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editQuestion(Database& db,  string& id,string &get_out)
{
    string question, answer;
    while (!selectQuestion(question, answer, get_out))
    {
        if (get_out == "0")
            return;
    }
    string query = "UPDATE forgot_password SET question = '" + question + "', answer = '" + answer + "' WHERE id = '" + id + "'";
    try {
        db.exec(query);
        cout << "Question and answer updated successfully.\n";
    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void editProfile(Database& db, string& id) {
    string option;
    string get_out;
    while (true)
    {
        cout << "Choose an option to edit your profile:\n"
                "1. Edit name.\n"
                "2. Edit age.\n"
                "3. Edit password.\n"
                "4. Edit Freetext.\n"
                "5. Edit questions.\n"
                "6. Back.\n";
        cin >> option;
        if(option.length() > 1)
            option = "10";
        switch (option[0])
        {
            case CHANGE_NAME:
                editName(db, id);
                break;
            case CHANGE_AGE:
                editAge(db, id);
                break;
            case CHANGE_PASSWORD:
                editPassword(db, id,get_out);
                break;
            case CHANGE_FREETEXT:
                editFreeText(db, id);
                break;
            case CHANGE_QUESTION:
                editQuestion(db, id,get_out);
                break;
            case BACK_TO_MENU:
                return;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}
