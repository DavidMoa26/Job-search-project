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

#endif //xMAIN_CPP_EMPLOYER_H
