#ifndef MAIN_CPP_CANDIDATE_H
#define MAIN_CPP_CANDIDATE_H
#include <SQLiteCpp/SQLiteCpp.h>


using namespace SQLite;

bool ViewAllJobs(Database& db, string& id);
void SelectJob(Database& db);


#endif //MAIN_CPP_CANDIDATE_H