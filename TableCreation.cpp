//
// Created by User on 28/02/2024.
//
#include <SQLiteCpp/SQLiteCpp.h>
#include "employer.h"
#include "authentication.h"
#include "candidate.h"
#include "TableCreation.h"

// Validation
bool TestTableExists(Database& db)
{
    try {
        Statement query(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='tests';");
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


// Creation

void CreateTestTable(Database& db) {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS tests ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "employer_id INTEGER NOT NULL,"
                "candidate_id INTEGER NOT NULL,"
                "question TEXT NOT NULL,"
                "answer1 TEXT NOT NULL,"
                "answer2 TEXT NOT NULL,"
                "answer3 TEXT NOT NULL,"
                "answer4 TEXT NOT NULL,"
                "correct_answer INTEGER NOT NULL,"
                "grade INTEGER,"
                "FOREIGN KEY(employer_id) REFERENCES users(id),"
                "FOREIGN KEY(candidate_id) REFERENCES users(id),"
                "UNIQUE(employer_id, candidate_id)"
                ");");
        cout << "tests table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void CreateTables(Database& db)
{
    if(!InterviewInvitationsExist(db))
        CreateInterviewInvitationTable(db);
    if(!ForgotPasswordTableExists(db))
        CreateForgotPasswordTable(db);
    if(!UsersTableExists(db))
        CreateUsersTable(db);
    if(!JobsListExists(db))
        CreateJobsListTable(db);
    if(!ResumeSubmissionsTableExists(db))
        CreateResumeSubmissionsTable(db);
    if(!ResumesTableExists(db))
        CreateResumeTable(db);
    if(!TestTableExists(db)) {
        CreateTestTable(db);
    }
}



