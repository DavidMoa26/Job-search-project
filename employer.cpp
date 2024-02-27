#include "employer.h"
#include "authentication.h"
#include "candidate.h"

enum SecurityQuestions{QUESTION_1 = '1', QUESTION_2,QUESTION_3,QUESTION_4,QUESTION_5,QUESTION_6,QUESTION_7};

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
void FetchAllJobs(Database& db, string& id) {
    if (!JobsListExists(db)) {
        cout << "jobs_list table does not exist.\n";
        return;
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
        }

    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        }
}
void DeleteJob(Database& db, string& id) {
    if (!JobsListExists(db)) {
        cout << "jobs_list table does not exist.\n";
        return;
    }
    try {
        FetchAllJobs(db, id); // Display jobs first
        string job_id;
        cout << "Enter Job id:\n ";
        cin >> job_id; // Assuming job_id is the ID of the job to be deleted

        Statement query(db, "DELETE FROM jobs_list WHERE id = ?");
        query.bind(1, stoi(job_id)); // Convert job_id to integer and bind it

        query.executeStep(); // Execute the DELETE operation, no need to loop through results

        // If you reach this point, the DELETE operation was executed
        // However, without additional context or a way to check, we don't know if a row was actually deleted.
        cout << "Job ID: " << job_id << " deleted." << endl;

    } catch(const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void EditJob(Database& db, string& id) {
    string job_id,companyName,location ,position,description,scope,experience,salary, choice;
    if (!JobsListExists(db)) {
        cout << "jobs_list table does not exist.\n";
        return;
    }
    try {
        FetchAllJobs(db, id);
        cout << "Enter Job id - or press 0 to back :\n ";
        cin >> job_id; // Assuming job_id is the ID of the job to be deleted
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
        cin >> choice;

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
                "employer_id TEXT NOT NULL,"
                "candidate_id TEXT NOT NULL,"
                "job_id TEXT NOT NULL,"
                "UNIQUE(candidate_id, job_id)"
                ");");
        cout << "Interview_invitations table created successfully.\n";
    } catch (const exception& e) {
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
    if(!ResumeSubmissionsTableExists(db)) {
        CreateResumeSubmissionsTable(db);
    }
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
        cout << "1. Accept" << endl;
        cout << "2. Back" << endl;
        cin >> choice;
        choice = tolower(choice); // Convert choice to lowercase

        string status;
        if (choice == '1')
        {
            status = "accepted";
            if(!InterviewInvitationsExist(db))
                CreateInterviewInvitationTable(db);
            insertToInterviewInvitationtable(db,select_id,job_idSelected,id);
        }
        else if (choice == '2')
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter '1' or '2'." << endl;
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
    cout << "Enter Job id to see all the submissions to the job  or enter 'B' to go back:\n ";
    cin >> job_id;
    if(job_id == "B")
    {
        return "B";
    }
    return job_id;
}
bool printPendingCandidates(Database&db,string &job_idSelected)
{
    int count = 0;
    try {
        // Check if job_id Selected exists
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
    if(!InterviewInvitationsExist(db)) {
        CreateInterviewInvitationTable(db);
    }
        try {
            Statement query(db, "INSERT INTO Interview_invitations (employee_id, candidate_id, job_id) VALUES (? ,?, ?)");
            query.bind(1, employee_id);
            query.bind(2, candidate_id);
            query.bind(3, job_id);
            query.exec();
            cout << "Interview invitation inserted successfully.\n";
        } catch (const exception& e) {
            cerr << "SQLite exception: " << e.what() << endl;
        }
}
void FillterCandidateResume(Database& db, string& id) {

    while (true)
    {
        int count = 0;
        string job_idSelected;
        job_idSelected = FetchJobsEmployee(db, id); // Assuming FetchJobsEmployee returns the job_id
        if(job_idSelected == "B")
            return;
        if (!printPendingCandidates(db, job_idSelected))
            return;
        string MinYearsExp;
        cout << "Enter minimum years of experience you want to see in the candidate resume, or enter 'B' to go back:\n ";
        cin >> MinYearsExp;

        if (MinYearsExp == "B" || MinYearsExp == "b") {
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

