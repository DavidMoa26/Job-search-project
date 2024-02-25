//
// Created by david on 22/02/2024.
//
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
