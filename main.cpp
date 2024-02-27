#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "menus.h"
#include "employer.h"

using namespace std;
using namespace SQLite;

int main()
{
    Database db("db.db", OPEN_READWRITE|OPEN_CREATE);
    MainMenu(db);
    cout << "GOODBYE\n";
    return 0;
}

