#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "authentication.h"
#include "menus.h"

using namespace std;
using namespace SQLite;

int main()
{
    Database db("db.db", OPEN_READWRITE|OPEN_CREATE);
    MainMenu(db);
    return 0;
}

