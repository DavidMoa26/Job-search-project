#include "employer.h"
#include "authentication.h"

enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3};

string SelectScope(){
    while (true) {
        cout << "Please select scope of position:\n";
        cout << "1. Full-Time\n"
             << "2. Part-Time\n"
             << "3. Remote\n";
        char option;
        cin >> option;
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
bool JobsListExists (Database& db) {
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='jobs_list';");
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
void CreateJobsListTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS jobs_list ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                "employer_id INTEGER NOT NULL,"
                "company_name TEXT NOT NULL,"
                "location TEXT NOT NULL,"
                "position TEXT NOT NULL,"
                "description TEXT NOT NULL,"
                "scope TEXT NOT NULL,"
                "experience INTEGER NOT NULL,"
                "salary INTEGER NOT NULL,"
                "FOREIGN KEY (employer_id) REFERENCES users(id));");
        cout << "jobs_list table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void InsertJobToDatabase(Database& db, string& id, string& name, string& location,string& position, string& description,string& scope,string& experience, string& salary) {
    try {
        // Prepare a SQL insert statement
        Statement query(db, "INSERT INTO jobs_list (employer_id, company_name, location, description,position, scope,experience,salary) VALUES (?, ?, ?,?, ?,?,?,?);");

        // Bind values to the statement
        query.bind(1, stoi(id));
        query.bind(2, name);
        query.bind(3, location);
        query.bind(4, description);
        query.bind(5, position);
        query.bind(6, scope);
        query.bind(7, stoi(experience));
        query.bind(8, stoi(salary));

        // Execute the statement
        query.exec();
        cout <<"job successfully added to the database.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void PostJob(Database& db, string& id) {
    string companyName, location, description,position,scope ,experience, salary;
    cout << "Enter Your company name : \n";
    cin >> companyName;
    while(companyName.empty()) {
        cout << "Company name cannot be empty. Please enter your company name:\n";
        cin >> companyName;
    }
    cout << "Enter Your company's location : \n";
    cin >> location;
    while(location.empty()) {
        cout << "location cannot be empty. Please try again:\n";
        cin >> location;
    }
    cout << "Enter job description : \n";
    cin >> description;
    while(description.empty()) {
        cout << "description cannot be empty. Please try again:\n";
        cin >> description;
    }
    cout << "Enter position : \n";
    cin >> position;
    while(position.empty()) {
        cout << "position cannot be empty. Please try again:\n";
        cin >> position;
    }
    while(position.empty()) {
        cout << "position cannot be empty. Please try again:\n";
        cin >> position;
    }
    scope = SelectScope();
    cout << "Enter experience for the position (must be numbers).\n";
    cin >> experience;
    while (!CheckIfIdIsDigits(experience)) {
        cout << "Invalid experience.must contains only numbers. Please try again:\n";
        cin >> experience;
    }
    cout << "Enter salary for the position (must be numbers).\n";
    cin >> salary;
    while (!CheckIfIdIsDigits(salary)) {
        cout << "Invalid salary .must contains only numbers. Please try again:\n";
        cin >> salary;
    }
    if(!JobsListExists(db))
        CreateJobsListTable(db);
    InsertJobToDatabase(db,id,companyName,location,position,description,scope,experience,salary);
}
void FetchAllJobs(Database& db, string& id)
{
    if (!JobsListExists(db))
    {
        cout << "jobs_list table does not exist.\n";
        return;
    }
    try {
        Statement query(db, "SELECT * FROM jobs_list WHERE employer_id = ?");
<<<<<<< HEAD
        //query.bind(1, stoi(id));
        if (query.executeStep()) {
=======
        query.bind(1, stoi(id));

        // Loop through each row in the result set
        while (query.executeStep()) {
>>>>>>> b33e9a15ab5f96340d8258c7333df68ec46262d4
            int jobId = query.getColumn(0).getInt();
            string companyName = query.getColumn(2).getText();
            string position = query.getColumn(4).getText();
            string description = query.getColumn(5).getText();
            cout << "Job ID: " << jobId << ", Company: " << companyName << ", Position: " << position << ", Description: " << description <<  endl;
        }

        if (query.getColumn(0).getInt() == 0) {
            cout << "No jobs found.\n";
        }

    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
bool InterviewInvitationsExist(Database&db)
{
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='Interview_invitations';");
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
void CreateInterviewInvitationTable(Database& db)
{
    try {
        db.exec("CREATE TABLE IF NOT EXISTS Interview_invitations ("
                "employee_id TEXT NOT NULL,"
                "candidate_id TEXT NOT NULL,"
                "job_id TEXT NOT NULL"
                ");");
        cout << "Interview_invitations table created successfully.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void printCandidateResume(Database&db ,string&selected_candidate_id)
{
    // Query the resume table for the selected candidate's information
    Statement resumeQuery(db, "SELECT full_name, age, degree1, degree2, degree3, work_experience, years_of_experience FROM resume WHERE candidate_id = ?;");
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
    while (true)
    {
        string job_idSelected;
        job_idSelected = FetchJobsEmployee(db, id); // Assuming FetchJobsEmployee returns the job_id
        if(!printPendingCandidates(db,job_idSelected))
            return;
        string select_id;
        cout << "Select the candidate ID you want to send interview invitation, or enter 'B' to go back:\n ";
        cin >> select_id;

        if (select_id == "B" || select_id == "b")
        {
            break;
        }
        printCandidateResume(db,select_id);
        char choice;
        cout << "Do you want to accept (A/a) or reject (R/r) the invitation, or enter 'B' to go back?\n ";
        cin >> choice;
        choice = tolower(choice); // Convert choice to lowercase

        string status;
        if (choice == 'a')
        {
            status = "accepted";
            if(!InterviewInvitationsExist(db))
                CreateInterviewInvitationTable(db);
            insertToInterviewInvitationtable(db,select_id,job_idSelected,id);
        }
        else if (choice == 'r')
        {
            status = "rejected";
            if(!InterviewInvitationsExist(db))
                CreateInterviewInvitationTable(db);
            insertToInterviewInvitationtable(db,select_id,job_idSelected,id);
        }
        else if (choice == 'b')
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter 'A', 'R', or 'B'." << endl;
            continue; // Restart the loop
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
void ViewAllInterviewInvitation(Database& db, string&employee_id) {
    try {
        // Select data from Interview_invitations table where candidate_id matches the provided employee_id
        Statement selectQuery(db, "SELECT * FROM Interview_invitations WHERE candidate_id = ?;");

        // Bind the employee_id parameter
        selectQuery.bind(1, employee_id);

        // Execute the query
        while (selectQuery.executeStep()) {
            string job_id = selectQuery.getColumn(1).getText(); // Assuming job_id is the second column

            // Print the data
            cout << "Candidate ID: " << employee_id << ", Job_id: " << job_id << endl;
            // Add more columns as needed
        }
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
    cout << "Enter Job id to see all the submissions to the job:\n ";
    cin >> job_id;
    return job_id;
}
bool printPendingCandidates(Database&db,string &job_idSelected)
{
    int count = 0;
    try {
        // Check if job_idSelected exists
        Statement checkQuery(db, "SELECT COUNT(*) FROM submission WHERE job_id = ?");
        checkQuery.bind(1, job_idSelected);

        int jobCount = 0;
        if (checkQuery.executeStep())
        {
            jobCount = checkQuery.getColumn(0).getInt();
        } else
        {
            cerr << "Error occurred while checking if Job ID exists.\n";
        }

        if (jobCount == 0)
        {
            cout << "Job ID " << job_idSelected << " does not exist .\n";
            return false; // Exit the function if job ID does not exist
        }

        // Job ID exists, continue to count pending submissions
        int count = 0;
        Statement query(db, "SELECT id, candidate_id, status, job_id FROM submission WHERE job_id = ?");
        query.bind(1, job_idSelected);

        cout << "the candidate that submitted for Job ID: " << job_idSelected << endl;

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
        }

    } catch(exception& e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
    }
    return true;
}
void insertToInterviewInvitationtable(Database&db,string&candidate_id,string&job_id,string&employee_id)
{
        try {
            Statement query(db, "INSERT INTO Interview_invitations (employee_id candidate_id, job_id) VALUES (? ,?, ?)");
            query.bind(1, employee_id);
            query.bind(2, candidate_id);
            query.bind(3, job_id);
            query.exec();
            cout << "Interview invitation inserted successfully.\n";
        } catch (const exception& e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }
}