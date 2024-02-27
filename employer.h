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
void ViewAllSubResumeProfiles(Database&);
void SendInterviewInvitation(Database&db,string&id);
void ViewAllInterviewInvitation(Database&);
void printCandidateResume(Database& ,string&);
bool InterviewInvitationsExist(Database&);
void CreateInterviewInvitationTable(Database& );
bool printPendingCandidates(Database&db,string &job_idSelected);
string FetchJobsEmployee(Database&,string&);
void insertToInterviewInvitationtable(Database&db,string&candidate_id,string&job_id);
#endif //xMAIN_CPP_EMPLOYER_H