//
// Created by User on 28/02/2024.
//

#ifndef JOBSEARCHPROJECT_TABLECREATION_H
#define JOBSEARCHPROJECT_TABLECREATION_H
#include <SQLiteCpp/SQLiteCpp.h>
#include "employer.h"
#include "authentication.h"
#include "candidate.h"
using namespace std;

void CreateTables(Database&);
void CreateTestTable(Database&);


#endif //JOBSEARCHPROJECT_TABLECREATION_H