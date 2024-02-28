//
// Created by User on 28/02/2024.
//
#include <SQLiteCpp/SQLiteCpp.h>
#include "employer.h"
#include "authentication.h"
#include "candidate.h"
#include "TableCreation.h"
void CreatTables(Database&db)
{
    if(!InterviewInvitationsExist(db))
    CreateInterviewInvitationTable(db);
    if(!ForgotPasswordTableExists(db))
    CreateForgotPasswordTable(db);
    if(!UsersTableExists(db))
    CreateUsersTable(db);
    if(!JobsListExists(db))
    CreateJobsListTable(db);
    if(!ResumeSubmissionsTableExists(db))
    CreateResumeSubmissionsTable(db);
    if(!ResumesTableExists(db))
    CreateResumeTable(db);
}