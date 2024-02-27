#ifndef MAIN_CPP_EMPLOYER_H
#define MAIN_CPP_EMPLOYER_H
#include "iostream"
#include <string>
#include "SQLiteCpp/SQLiteCpp.h"

using namespace std;
using namespace SQLite;

string SelectScope();
bool JobsListExists (Database&);
void CreateJobsListTable(Database&);
void PostJob(Database&, string&);
void InsertJobToDatabase(Database& , string& , string& ,string&, string&, string&, string& , string&, string&);
void FetchAllJobs(Database& , string&);
bool InterviewInvitationsExist(Database&db);
void CreateInterviewInvitationTable(Database& db);
void printCandidateResume(Database&db ,string&selected_candidate_id);
void SendInterviewInvitation(Database&db,string&id);
void ViewAllInterviewInvitation(Database& db, string&employee_id);
string FetchJobsEmployee(Database& db, string& id);
bool printPendingCandidates(Database&db,string &job_idSelected);
void insertToInterviewInvitationtable(Database&db,string&candidate_id,string&job_id,string&employee_id);
#endif //xMAIN_CPP_EMPLOYER_H
