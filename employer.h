#ifndef MAIN_CPP_EMPLOYER_H
#define MAIN_CPP_EMPLOYER_H
#include "iostream"
#include <string>
#include "SQLiteCpp/SQLiteCpp.h"

using namespace std;
using namespace SQLite;

string SelectScope();
bool JobsListExists (Database& );
void CreateJobsListTable(Database& );
void InsertJobToDatabase(Database& , string& , string&, string& ,string& , string& ,string& ,string& , string& );
void PostJob(Database&, string& );
void FetchAllJobs(Database&, string&);

void printCandidateResume(Database& ,string&);
void SendInterviewInvitation(Database&,string&);
string FetchJobsEmployee(Database&, string&);
bool printPendingCandidates(Database&,string &);

bool InterviewInvitationsExist(Database&);
void CreateInterviewInvitationTable(Database&);
void insertToInterviewInvitationtable(Database&,string&,string&,string&);
void ViewAllInterviewInvitation(Database&,string&);
#endif //xMAIN_CPP_EMPLOYER_H
