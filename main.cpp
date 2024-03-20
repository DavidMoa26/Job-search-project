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
void printGoodbye() {
    cout << " -------------------------------------------- \n";
    cout << "|                                            |\n";
    cout << "|         Thank you for your time.           |\n";
    cout << "|       Goodbye and have a wonderful day!    |\n";
    cout << "|                                            |\n";
    cout << " -------------------------------------------- \n";
}
int main()
{
    Database db("db.db", OPEN_READWRITE|OPEN_CREATE);
    CreatTables(db);
    MainMenu(db);
    printGoodbye();
    return 0;
}


