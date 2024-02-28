#ifndef MAIN_CPP_CANDIDATE_H
#define MAIN_CPP_CANDIDATE_H
#include <SQLiteCpp/SQLiteCpp.h>


using namespace SQLite;

// Designing
void ChangeColor(int color);
void printLine(int width, char borderChar, char fillChar);
void printRow(string text, int width, char borderChar);

// Jobs
void PrintJob(string& jobId, string& companyName, string& location, string& position, string& scope, string& experience);
void ViewAllJobs(Database& db);
string SelectJob(Database& db, string& id);
void SearchByCategory(Database& db);
string* SelectFilter(Database& db);

//Resumes
bool ResumesTableExists(Database& db);
void CreateResumeTable(Database& db);
void InsertResumeToTable(Database& db, string& id, string& full_name, string& age, string& degree1, string& degree2, string& degree3, string& work_experience, string& years_of_experience);
void CreateResume(Database& db, string& id);
int SelectDegree();

// Resume submissions
bool ResumeSubmissionsTableExists (Database& db);
void CreateResumeSubmissionsTable(Database& db);
void InsertSubmitToTable(Database& db, string& id, string& jobId);
void SubmitResume(Database& db, string& id, string& jobId);
void ViewAllSubmittedJobs(Database& db, string& candidate_id);


void RejectAcceptInterviewInvitation(Database&db,string&id);
#endif //MAIN_CPP_CANDIDATE_H