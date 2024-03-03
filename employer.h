#ifndef MAIN_CPP_EMPLOYER_H
#define MAIN_CPP_EMPLOYER_H
#include "iostream"
#include <string>
#include "SQLiteCpp/SQLiteCpp.h"
#include "TableCreation.h"
using namespace std;
using namespace SQLite;

string SelectScope();
string SelectFieldToEdit();

//Job functions

void PostJob(Database&, string&);
bool FetchAllJobs(Database& , string&);
void DeleteJob(Database& , string&);
void EditJob(Database& , string&);
bool validateYearsOfExperience(string & years_of_experience);
bool validateSalary(string & years_of_experience);
string EnterYearsOfExperienceTillValids(string& years_of_experience);
bool validateYearsOfExperiences(string & years_of_experience);
//interview invitation functions

void SendInterviewInvitation(Database&db,string&id);
void ViewAllInterviewInvitation(Database& db, string&employee_id);

//additional functions
string chooseCandidateId(Database&);
string FetchJobsEmployee(Database& db, string& id);
bool printPendingCandidates(Database&,string &,string&);
void FilterCandidateResume(Database&db,string&id);
void printCandidateResume(Database&db ,string&selected_candidate_id);
#endif //MAIN_CPP_EMPLOYER_H