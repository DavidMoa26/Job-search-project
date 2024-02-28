#ifndef MAIN_CPP_EMPLOYER_H
#define MAIN_CPP_EMPLOYER_H
#include "iostream"
#include <string>
#include "SQLiteCpp/SQLiteCpp.h"

using namespace std;
using namespace SQLite;

string SelectScope();
string SelectFieldToEdit();

//Job functions
bool JobsListExists (Database&);
void CreateJobsListTable(Database&);
void PostJob(Database&, string&);
void InsertJobToDatabase(Database& , string& , string& ,string&, string&, string&, string& , string&, string&);
void FetchAllJobs(Database& , string&);
void DeleteJob(Database& , string&);
void EditJob(Database& , string&);


//interview invitation functions
bool InterviewInvitationsExist(Database&db);
void CreateInterviewInvitationTable(Database& db);
void SendInterviewInvitation(Database&db,string&id);
void ViewAllInterviewInvitation(Database& db, string&employee_id);
void insertToInterviewInvitationtable(Database&db,string&candidate_id,string&job_id,string&employee_id);
void insertDataToTestsTable(Database&,string&,string&,string&,string&,string&,string&,string&,string&);

//additional functions
string chooseCandidateId(Database&);
string FetchJobsEmployee(Database& db, string& id);
bool printPendingCandidates(Database&db,string &job_idSelected);
void FillterCandidateResume(Database&db,string&id);
void printCandidateResume(Database&db ,string&selected_candidate_id);
#endif //xMAIN_CPP_EMPLOYER_H