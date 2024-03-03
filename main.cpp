#include <iostream>
#include "SQLiteCpp/SQLiteCpp.h"
#include "authentication.h"
#include "menus.h"
#include "candidate.h"
#include "employer.h"
#include "sqlite3.h"
#include "TableCreation.h"

using namespace std;
using namespace SQLite;

int main()
{
    Database db("db.db", OPEN_READWRITE|OPEN_CREATE);
    CreatTables(db);
    MainMenu(db);
    cout << "GOODBYE\n";
    return 0;
}

