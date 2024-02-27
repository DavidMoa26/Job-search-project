#include <SQLiteCpp/SQLiteCpp.h>
#include "iostream"
#include <string>
#include "employer.h"
#include "candidate.h"

using namespace std;
bool ViewAllJobs(Database& db, string& id)
{
    if (!JobsListExists(db))
    {
        cout << "Users table does not exist.\n";
        return false;
    }
    try {
        Statement query(db, "SELECT * FROM jobs_list");
        if(query.executeStep())
        {
            while (query.executeStep())
            {
                string jobId = query.getColumn(0).getText();
                string companyName = query.getColumn(2).getText();
                string location = query.getColumn(3).getText();
                string position = query.getColumn(4).getText();
                string scope = query.getColumn(6).getText();
                string experience = query.getColumn(7).getText();

                cout << " ---------------------------------" << endl;
                cout << jobId << "." << endl
                     << "   Company: " << companyName  << endl
                     << "   Position: " << position  << endl
                     << "   Location: " << location  << endl
                     << "   Scope: " << scope  << endl
                     << "   Experience years: " << experience  << endl;
            }
            cout << " ---------------------------------" << endl;
            return true;
        }
        else
        {
            cout << "No jobs found.\n";
            return false;
        }
    }
    catch(exception & e)
    {
        cerr << "SQLite exception: " << e.what() << endl;
        return false;
    }
}
void SelectJob(Database& db)
{
    string choice;
    cout << "Select a job - press its number   |   Back - press 'b'." << endl;
    cin >> choice;
    if (choice == "b")
        return;
    try {
        Statement query(db, "SELECT * FROM jobs_list WHERE id = ?");
        query.bind(1, stoi(choice));
        if (query.executeStep())
        {
            string jobId = query.getColumn(0);
            if (jobId == choice)
            {
                string companyName = query.getColumn(2).getText();
                string position = query.getColumn(4).getText();
                string description = query.getColumn(5).getText();
                cout << "Job ID: " << jobId << ", Company: " << companyName << ", Position: " << "Description: " << description <<  endl;
            }
        } else {
            cout << "No jobs found.\n";
        }
    } catch(exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
    }
}
void viewInterviewInvitations(Database& db, string id) {
    try {
        // Retrieve and display interview invitations for the specific candidate ID
        Statement selectQuery(db, "SELECT * FROM Submissions_table WHERE candidate_id = ?;");
        selectQuery.bind(1, id); // Bind the candidate ID parameter
        while (selectQuery.executeStep()) {
            // Display each submission and provide options for accept/reject
            // Extract data from the query result and present it to the candidate
            string job_id = selectQuery.getColumn(0).getText();
            string job_name = selectQuery.getColumn(2).getText();
            string status = selectQuery.getColumn(3).getText();

            cout << "Job ID: " << job_id << ", Job Name: " << job_name << ", Status: " << status << endl;
            cout << "Accept (a) / Reject (r) / Go back (g)" << endl;

            // Handle accept/reject options
            char option;
            cin >> option;
            if (option == 'A' || option == 'a')
            {
                // Update status to "accepted" in the database
                Statement updateQuery(db, "UPDATE Submissions_table SET status = 'accepted' WHERE candidate_id = ? AND job_id = ?;");
                updateQuery.bind(1, id); // Bind the candidate ID parameter
                updateQuery.bind(2, job_id);
                updateQuery.exec();
                cout << "Invitation accepted." << endl;
            }
            else if (option == 'R' || option == 'r')
            {
                // Update status to "rejected" in the database
                Statement updateQuery(db, "UPDATE Submissions_table SET status = 'rejected' WHERE candidate_id = ? AND job_id = ?;");
                updateQuery.bind(1, id); // Bind the candidate ID parameter
                updateQuery.bind(2, job_id);
                updateQuery.exec();
                cout << "Invitation rejected." << endl;
            }
            else if (option == 'g' || option == 'G') {
                return; // Go back
            }
            else {
                cout << "Invalid option." << endl;
            }
        }
    }
    catch(const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        }
}
void CreateResumeTable(Database& db)
{
    try {
        db.exec("CREATE TABLE IF NOT EXISTS resume ("
                "candidate_id INTEGER PRIMARY KEY UNIQUE,"
                "full_name NOT NULL,"
                "age INTEGER NOT NULL,"
                "degree1,"
                "degree2"
                "degree3"
                "work_experience NOT NULL,"
                "years_of_experience INTEGER NOT NULL);");
        cout << "resumes table created.\n";
    } catch (const exception& e) {
        cerr << "SQLite exception: " << e.what() << endl;
        }
}