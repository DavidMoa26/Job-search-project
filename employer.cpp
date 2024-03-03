#include "employer.h"
#include "authentication.h"
#include "candidate.h"
#include "TableCreation.h"
#include "menus.h"
enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,QUESTION_6,QUESTION_7};
#define BACK "b"
#define CONTINUE "0"
#define ALL_GOOD "1"
string SelectScope(){
    while (true) {
        cout << "Please select scope of position:\n";
        cout << "1. Full-Time\n"
             << "2. Part-Time\n"
             << "3. Remote\n";
        char option;
        option = UserChoice();
        switch (option) {
            case QUESTION_1:
                return "Full-Time";
            case QUESTION_2:
                return "Part-Time";
            case QUESTION_3:
                return "Remote";
            default:
                cout << "You entered an illegal option. Please try again!\n";
                continue;
        }
    }
}
string SelectFieldToEdit(){
    while (true) {
        cout << "Please select field to edit:\n";
        cout << "1. Company\n"
             << "2. Location\n"
             << "3. Position\n"
             << "4. Description\n"
             << "5. Scope\n"
             << "6. Experience\n"
             << "7. Salary\n"
                ;
        char option;
        cin >> option;
        switch (option) {
            case QUESTION_1:
                return "Company";
            case QUESTION_2:
                return "Location";
            case QUESTION_3:
                return "Position";
            case QUESTION_4:
                return "Description";
            case QUESTION_5:
                return "Scope";
            case QUESTION_6:
                return "Experience";
            case QUESTION_7:
                return "Salary";
            default:
                cout << "You entered an illegal option. Please try again!\n";
                continue;
        }
    }
}
string EnterCompanyNameTillValid(string& freeText)
{
    do {
        cout << "Please enter company name about yourself   |   Back - 'b':\n";
        getline(cin >> ws, freeText);
        if (freeText == BACK)
            return BACK;
    } while (!validFreeText(freeText));
    return ALL_GOOD;
}
string EnterCompanyLocationTillValid(string& freeText)
{
    do {
        cout << "Please enter company location    |   Back - 'b':\n";
        getline(cin >> ws, freeText);
        if (freeText == BACK)
            return BACK;
    } while (!validFreeText(freeText));
    return ALL_GOOD;
}
string EnterJobDescriptionTillValid(string& freeText)
{
    do {
        cout << "Please enter job description    |   Back - 'b':\n";
        getline(cin >> ws, freeText);
        if (freeText == BACK)
            return BACK;
    } while (!validFreeText(freeText));
    return ALL_GOOD;
}
string EnterJobPositionTillValid(string& freeText)
{
    do {
        cout << "Please Enter position    |   Back - 'b':\n";
        getline(cin >> ws, freeText);
        if (freeText == BACK)
            return BACK;
    } while (!validFreeText(freeText));
    return ALL_GOOD;
}
string EnterYearsOfExperienceTillValids(string& years_of_experience)
{
    do {
        cout << "Please Enter experience for the position (Must be between 0 - 50)    |   Back - 'b':\n";
        getline(cin >> ws, years_of_experience);
        if (years_of_experience == BACK)
            return BACK;
    } while (!validateYearsOfExperiences(years_of_experience));
    return ALL_GOOD;
}
string EnterSalaryTillValid(string& years_of_experience)
{
    do {
        cout << "Please Enter salary (Must be between 0 - 1000000000)    |   Back - 'b':\n";
        getline(cin >> ws, years_of_experience);
        if (years_of_experience == BACK)
            return BACK;
    } while (!validateSalary(years_of_experience));
    return ALL_GOOD;
}
bool validateYearsOfExperiences(string & years_of_experience)
{
    if(!CheckIfIdIsDigits(years_of_experience))
    {
        cout << "You entered an invalid experience for the position(Must be between 0 - 50) Please try again.\n";
        return false;
    }
    int ageNumber = stoi(years_of_experience);
    if(ageNumber < 0 || ageNumber > 50 || !NotValidSpace(years_of_experience))
    {
        cout << "You entered an invalid experience for the position(Must be between 0 - 50) Please try again.\n";
        return false;
    }
    return true;
}
bool validateSalary(string & years_of_experience)
{
    if(!CheckIfIdIsDigits(years_of_experience))
    {
        cout << "You entered an invalid salary(Must be between 0 - 50) Please try again.\n";
        return false;
    }
    int ageNumber = stoi(years_of_experience);
    if(ageNumber < 0 || ageNumber > 1000000000 || !NotValidSpace(years_of_experience))
    {
        cout << "You entered an invalid salary(Must be between 0 - 50) Please try again.\n";
        return false;
    }
    return true;
}




void PostJob(Database& db, string& id) {
    string companyName, location, description,position,scope ,experience, salary;
    string choice;
    cout << "Create your job - press 'j'.   |   Back - press 'b'." << endl;
    bool firstIter = true;
    do
    {
        if (!firstIter)
            cout << "You entered an invalid choice, please try again   |   Back - 'b'." << endl;
        firstIter = false;
        getline(cin >> ws,choice);
        if (choice == BACK)
            return;
    } while (choice!="j");
    if (EnterCompanyNameTillValid(companyName) == BACK)
        return;
    if (EnterCompanyLocationTillValid(location) == BACK)
        return;
    if (EnterJobDescriptionTillValid(description) == BACK)
        return;
    if (EnterJobPositionTillValid(position) == BACK)
        return;
    scope = SelectScope();
    if (EnterYearsOfExperienceTillValids(experience) == BACK)
        return;
    if (EnterSalaryTillValid(salary) == BACK)
        return;
    if(!JobsListExists(db))
        CreateJobsListTable(db);
    InsertJobToDatabase(db,id,companyName,location,position,description,scope,experience,salary);
}
bool FetchAllJobs(Database& db, string& id) {
    if (!JobsListExists(db)) {
        cout << "jobs_list table does not exist.\n";
        return " ";
    }

    bool foundJobs = false;

    try {
        Statement query(db, "SELECT * FROM jobs_list WHERE employer_id = ?");
        query.bind(1, stoi(id));

        // Loop through each row in the result set
        while (query.executeStep()) {
            foundJobs = true;
            int jobId = query.getColumn(0).getInt();
            string companyName = query.getColumn(2).getText();
            string position = query.getColumn(4).getText();
            string description = query.getColumn(5).getText();
            cout << "Job ID: " << jobId << ", Company: " << companyName <<  endl;
        }

        if (!foundJobs) {
            cout << "No jobs found.\n";
            return false;
        }

    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return true;
}
void DeleteJob(Database& db,  string& employer_id) {
    if (!JobsListExists(db)) {
        cout << "jobs_list table does not exist.\n";
        return;
    }
    if (!FetchAllJobs(db, employer_id))
        return; // Exit if FetchAllJobs returns false
    while (true)
    {
        try {
            // Display jobs first

            string job_id;
            cout << "Enter Job id you want to delete|  or press 'b' - for going back " << endl;;
            getline(cin >> ws, job_id); // Assuming job_id is the ID of the job to be deleted
            if(job_id == "b")
                return;
            // Check if the job ID exists in the table for the specified employer ID
            Statement checkQuery(db, "SELECT COUNT(*) FROM jobs_list WHERE id = ? AND employer_id = ?");
            checkQuery.bind(1, stoi(job_id)); // Bind the entered job ID
            checkQuery.bind(2, stoi(employer_id)); // Bind the employer ID
            checkQuery.executeStep();

            int count = checkQuery.getColumn(0); // Get the count of rows with the specified job ID and employer ID

            if (count > 0) {
                // If count is greater than 0, it means the job ID exists in the table for the specified employer
                Statement deleteQuery(db, "DELETE FROM jobs_list WHERE id = ? AND employer_id = ?");
                deleteQuery.bind(1, stoi(job_id)); // Convert job_id to integer and bind it
                deleteQuery.bind(2, stoi(employer_id)); // Bind the employer ID
                deleteQuery.executeStep(); // Execute the DELETE operation

                cout << "Job ID: " << job_id << " deleted." << endl;
            } else {
                // If count is 0, it means the job ID was not found in the table for the specified employer
                cout << "Job ID: " << job_id << " not found in the table for the specified employer." << endl;
            }

        } catch (const exception &e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }
    }
}

bool isValidChoice(const string& choice) {
    // Check if choice is a single character and between '1' and '8'
    return choice.size() == 1 && choice[0] >= '1' && choice[0] <= '8';
}

bool checkIfJobExist(Database& db, string& id) {
    try {
        // Prepare a SELECT statement to check if the ID exists
        Statement query(db, "SELECT COUNT(1) FROM jobs_list WHERE id = ?");

        query.bind(1, id);

        if (query.executeStep()) {
            return query.getColumn(0).getInt() > 0;
        }
    } catch (const std::exception& e) {
        cerr << "Error checking job ID in database: " << e.what() << endl;
    }

    return false;
}

bool checkIfCandidateExist(Database& db, string& id) {
    try {
        Statement query(db, "SELECT COUNT(1) FROM users WHERE id = ? AND role = 'candidate'");

        query.bind(1, id);

        if (query.executeStep()) {
            return query.getColumn(0).getInt() > 0;
        }
    } catch (exception& e) {
        cerr << "Error checking job ID in database: " << e.what() << endl;
    }

    return false;
}
void EditJob(Database& db, string& employer_id) {
    string job_id,companyName,location ,position,description,scope,experience,salary, choice;
    if (!JobsListExists(db)) {
        cout << "jobs_list table does not exist.\n";
        return;
    }

    try {
        if(!FetchAllJobs(db, employer_id))
            return; // Display jobs first
        cout << "Enter Job id - or press 0 to back :\n ";
        while(true) {
            getline(cin >> ws, job_id);
            if(checkIfJobExist(db,job_id)) {
                break;
            }
            else if (job_id == "0") {
                break;
            }
            else {
                cout << "Invalid Job id, try again - press 0 to back :\n ";
            }
        }

        if (job_id == "0")
            return;



        Statement query(db, "SELECT * FROM jobs_list WHERE id = ?");
        query.bind(1, stoi(job_id)); // Convert job_id to integer and bind it

        while(query.executeStep()) {
            companyName = query.getColumn(2).getText();
            location = query.getColumn(3).getText();
            position = query.getColumn(4).getText();
            description = query.getColumn(5).getText();
            scope = query.getColumn(6).getText();
            experience = query.getColumn(7).getText();
            salary = query.getColumn(8).getText();
        }
        cout << "1. Company : " << companyName << endl
             << "2. Location : " << location << endl
             << "3. Position : " << position << endl
             << "4. Description : " << description << endl
             << "5. Scope : " << scope << endl
             << "6. Experience : " << experience << endl
             << "7. Salary : " << salary << endl
             << "8. Go Back " << endl;
        cout << "Please select field to edit:\n";
        while(true) {
            getline(cin, choice);
            if (isValidChoice(choice)) {
                break;
            }
            else {
                cout << "Invalid input. Please enter a number between 1 and 8.\n";
            }
        }

        if (choice == "8")
            return;


        string newValue;
        string sqlUpdateQuery = "UPDATE jobs_list SET ";

        if (choice == "7") {
            cout << "Enter new salary :\n";
            cin >> newValue;
            sqlUpdateQuery += "salary = ?";
        } else if (choice == "6") {
            cout << "Enter new experience requirement:\n";
            cin >> newValue;
            sqlUpdateQuery += "experience = ? ";
        } else if (choice == "5") {
            newValue = SelectScope();
            sqlUpdateQuery += "scope = ? ";
        }
        else if (choice == "4") {
            cout << "Enter new description :\n";
            cin >> newValue;
            sqlUpdateQuery += "description = ? ";
        } else if (choice == "3") {
            cout << "Enter new position :\n";
            cin >> newValue;
            sqlUpdateQuery += "position = ? ";
        } else if (choice == "2") {
            cout << "Enter new location :\n";
            cin >> newValue;
            sqlUpdateQuery += "location = ? ";
        } else if (choice == "1") {
            cout << "Enter new company name :\n";
            cin >> newValue;
            sqlUpdateQuery += "company_name = ? ";
        }

        sqlUpdateQuery += "WHERE id = ?";

        try {
            Statement editQuery(db, sqlUpdateQuery); // Correct variable name
            if (choice == "7" || choice == "6") {
                // For numeric fields, ensure the conversion and binding are correct
                editQuery.bind(1, stoi(newValue)); // Convert and bind newValue for numeric fields
            } else {
                editQuery.bind(1, newValue); // Bind newValue directly for text fields
            }
            editQuery.bind(2, stoi(job_id)); // Ensure job_id is converted to int and bound correctly
            editQuery.exec(); // Execute the update statement
            cout << "Job updated successfully.\n";
        } catch (exception& e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }

    } catch(const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void printCandidateResume(Database&db ,string&selected_candidate_id)
{
    // Query the resume table for the selected candidate's information
    Statement resumeQuery(db, "SELECT full_name, age, degree1, degree2, degree3, work_experience, years_of_experience FROM resumes WHERE candidate_id = ?;");
    resumeQuery.bind(1, selected_candidate_id);

    if (resumeQuery.executeStep()) {
        // Fetch and print resume information
        string full_name = resumeQuery.getColumn(0).getText();
        int age = resumeQuery.getColumn(1).getInt();
        string degree1 = resumeQuery.getColumn(2).getText();
        string degree2 = resumeQuery.getColumn(3).getText();
        string degree3 = resumeQuery.getColumn(4).getText();
        string work_experience = resumeQuery.getColumn(5).getText();
        int years_of_experience = resumeQuery.getColumn(6).getInt();

        cout << "Resume Information for Candidate ID: " << selected_candidate_id << endl;
        cout << "---------------------------------------" << endl;
        cout << "Full Name:         " <<  full_name << endl;
        cout << "Age:               " <<  age << endl;
        cout << "Degrees:           " <<  degree1 << ", " << degree2 << ", " << degree3 << endl;
        cout << "Work Experience:   " <<  work_experience << endl;
        cout << "Years of Experience:" <<  years_of_experience << endl;
        cout << "---------------------------------------" << endl;
    } else {
        cout << "No resume found for Candidate ID: " << selected_candidate_id << endl;
    }
}
void SendInterviewInvitation(Database&db,string&id)
{
    string job_idSelected,grade,status;
    if(!ResumeSubmissionsTableExists(db)) {
        CreateResumeSubmissionsTable(db);
    }
    while (true)
    {
        job_idSelected = FetchJobsEmployee(db, id);
        if(!printPendingCandidates(db,job_idSelected,id))
            return;
        string select_id;
        cout << "Select the candidate ID you want to send interview invitation, or enter 'B' to go back:\n ";
        while (true) {
            cin >> select_id;
            if(checkIfCandidateExist(db,select_id)) {
                break;
            }
            else if(select_id == "B" || select_id == "b"){
                break;
            }
            else {
                cout << "Invalid candidate ID, try again or enter 'B' to go back:\n ";
            }
        }


        if (select_id == "B" || select_id == "b")
        {
            break;
        }
        printCandidateResume(db,select_id);
        try {
            Statement query(db,"SELECT grade FROM tests WHERE employer_id = ? AND candidate_id = ?;");
            query.bind(1,id);
            query.bind(2,select_id);
            while (query.executeStep()) {
                grade = query.getColumn(0).getText();
            }
        } catch(exception& e)
        {
            cerr << "SQLite exception: " << e.what() << endl;
        }

        char choice;

        if(grade.empty()) {
            cout << "1. Accept" << endl;
            cout << "2. Reject" << endl;
            cout << "3. Send question for test" << endl;
            cout << "4. Go Back" << endl;
            cin >> choice;

            if (choice == '1')
            {
                status = "accepted";
                if(!InterviewInvitationsExist(db))
                    CreateInterviewInvitationTable(db);
                insertToInterviewInvitationtable(db,select_id,job_idSelected,id);
            }
            else if(choice == '2')
                status = "reject";
            else if(choice == '3')
            {
                status = "pending";
                cin.ignore();
                string question, answer1,answer2,answer3, answer4,correct_answer;
                cout << "Please enter your question : \n";
                cin >> question;
                cout << "Please enter your answer 1 : \n";
                cin >> answer1;
                cout << "Please enter your answer 2 : \n";
                cin >> answer2;
                cout << "Please enter your answer 3 : \n";
                cin >> answer3;
                cout << "Please enter your answer 4 : \n";
                cin >> answer4;
                cout << "Please enter the correct answer (1-4) : \n";
                cin >> correct_answer;
                insertDataToTestsTable(db,id,select_id,question,answer1,answer2,answer3,answer4,correct_answer);
            }
            else if (choice == '4')
            {
                break;
            }
            else
            {
                cout << "Invalid choice. Please enter '1' or '2'." << endl;
                continue; // Restart the loop
            }
        }

        else {
            cout << "Grade is : " << grade << endl;
            cout << "1. Accept" << endl;
            cout << "2. Reject" << endl;
            cout << "3. Go Back" << endl;
            cin >> choice;
            choice = tolower(choice); // Convert choice to lowercase

            if (choice == '1')
            {
                status = "accepted";
                if(!InterviewInvitationsExist(db))
                    CreateInterviewInvitationTable(db);
                insertToInterviewInvitationtable(db,select_id,job_idSelected,id);
            }
            else if(choice == '2')
                status = "reject";

            else if (choice == '3')
            {
                break;
            }
            else
            {
                cout << "Invalid choice. Please enter '1' or '2'." << endl;
                continue; // Restart the loop
            }
        }

        try
        {
            // Update submission status based on choice
            db.exec("UPDATE submission SET status = '" + status + "' WHERE job_id = '" + job_idSelected + "' AND candidate_id = '" + select_id + "'");

            cout << "Invitation " << status << " for Candidate ID: " << select_id << endl;

        } catch(exception& e)
        {
            cerr << "SQLite exception: " << e.what() << endl;
        }
    }

}
void ViewAllInterviewInvitation(Database& db, string&employer_id) {
    int count = 0;
    try {
        // Select data from Interview_invitations table where candidate_id matches the provided employee_id
        Statement selectQuery(db, "SELECT * FROM Interview_invitations WHERE employer_id = ?;");

        // Bind the employee_id parameter
        selectQuery.bind(1, employer_id);

        // Execute the query
        while (selectQuery.executeStep()) {
            string id_emp = selectQuery.getColumn(0);
            string candidate_id = selectQuery.getColumn(1);
            string job_id = selectQuery.getColumn(2).getText(); // Assuming job_id is the second column

            if(id_emp == employer_id)
            {
                cout << "Interview invitation for job id:" << job_id << " was send to candidate id:" << candidate_id
                     << endl;
                ++count;
            }
        }
        if(count == 0)
            cout << "No history found\n";
    } catch(const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
string FetchJobsEmployee(Database& db, string& id) {
    try {
        Statement query(db, "SELECT * FROM jobs_list WHERE employer_id = ?");
        query.bind(1, stoi(id));

        // Execute the query and check if there are any results
        bool jobFound = false;
        while (query.executeStep()) {
            jobFound = true;
            int jobId = query.getColumn(0).getInt();
            string companyName = query.getColumn(2).getText();
            cout << "Job ID: " << jobId << ",Company: " << companyName << endl;
        }

        if (!jobFound) {
            cout << "No jobs found for employer ID: " << id << endl;
            return ""; // Return an empty string to indicate no job found
        }

    } catch (exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        return ""; // Return an empty string on exception
    }

    string job_id;
    cout << "Enter Job id to see all the submissions to the job  or enter 'B' to go back:\n ";
    while(true) {
        cin >> job_id;
        if(checkIfJobExist(db,job_id)){
            break;
        } else if(job_id == "b" || job_id == "B") {
            break;
        } else {
            cout << "Invalid job id, try again or press b to back\n";
        }
    }
    if(job_id == "B")
    {
        return "B";
    }
    return job_id;
}
bool printPendingCandidates(Database&db,string &job_idSelected,string&employer_id)
{
    try {
        // Job ID exists, continue to count pending submissions
        int count = 0;
        Statement query(db, "SELECT id, candidate_id, status, job_id FROM submission WHERE job_id = ? AND status = 'pending'");
        query.bind(1, job_idSelected);

        while (query.executeStep())
        {
            string candidateId = query.getColumn(1).getText();
            string status = query.getColumn(2).getText();

            // Check if the status is "pending" before printing
            if (status == "pending")
            {
                cout << "Candidate ID: " << candidateId << endl;
                ++count;
            }
        }

        if (count == 0)
        {
            cout << "No pending submissions found for Job ID: " << job_idSelected << endl;
            return false;
        }

    } catch(exception& e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return true;
}
void FilterCandidateResume(Database& db, string& id) {

    while (true)
    {
        int count = 0;
        string job_idSelected;
        job_idSelected = FetchJobsEmployee(db, id); // Assuming FetchJobsEmployee returns the job_id
        if(job_idSelected == "B")
            return;
        if (!printPendingCandidates(db, job_idSelected,id))
            return;
        string MinYearsExp;
        cout << "Enter minimum years of experience you want to see in the candidate resume, or enter 'B' to go back:\n ";
        getline(cin >> ws,MinYearsExp);

        if (MinYearsExp == "B") {
            break;
        }

        try {
            // Select all resumes
            Statement selectQuery(db, "SELECT * FROM resumes;");

            // Execute the query
            while (selectQuery.executeStep()) {
                string candidate_id = selectQuery.getColumn(0).getText(); // Assuming candidate_id is the first column
                string full_name = selectQuery.getColumn(1).getText(); // Assuming full_name is the second column
                int age = selectQuery.getColumn(2).getInt(); // Assuming age is the third column
                string degree1 = selectQuery.getColumn(3).getText(); // Assuming degree1 is the fourth column
                string degree2 = selectQuery.getColumn(4).getText(); // Assuming degree2 is the fifth column
                string degree3 = selectQuery.getColumn(5).getText(); // Assuming degree3 is the sixth column
                string work_experience = selectQuery.getColumn(6).getText(); // Assuming work_experience is the seventh column
                int years_of_experience = selectQuery.getColumn(7).getInt(); // Assuming years_of_experience is the eighth column

                // Check if the candidate meets the minimum years of experience requirement
                if (years_of_experience >= stoi(MinYearsExp)) {
                    // Print the resume data
                    cout << "Candidate ID: " << candidate_id << endl;
                    cout << "Full Name: " << full_name << endl;
                    cout << "Age: " << age << endl;
                    cout << "Degrees: " << degree1 << " " << degree2 << "  " << degree3 << endl;
                    cout << "Work Experience: " << work_experience << endl;
                    cout << "Years of Experience: " << years_of_experience << endl;
                    cout << "---------------------------------------------" << endl;
                    ++count;
                }
            }
        } catch (const exception& e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }
        if(count == 0)
            cout << "no resume found with that given of minimum years of experience.\n";
    }
}