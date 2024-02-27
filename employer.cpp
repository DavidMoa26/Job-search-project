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
        //query.bind(1, stoi(id));
        if (query.executeStep()) {
            int jobId = query.getColumn(0).getInt();
            string companyName = query.getColumn(2).getText();
            string position = query.getColumn(4).getText();
            string description = query.getColumn(5).getText();
            cout << "Job ID: " << jobId << ", Company: " << companyName << ", Position: " << "Description: " << description <<  endl;
        } else {
            cout << "No jobs found.\n";
        }
    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}