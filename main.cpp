enum ConsoleColor
{
    FOREGROUND_BLUE      = 0x0001,
    FOREGROUND_GREEN     = 0x0002,
    FOREGROUND_RED       = 0x0004,
    FOREGROUND_INTENSITY = 0x0008,
    BACKGROUND_BLUE      = 0x0010,
    BACKGROUND_GREEN     = 0x0020,
    BACKGROUND_RED       = 0x0040,
    BACKGROUND_INTENSITY = 0x0080
};

#include <iostream>
#include "menus.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "TableCreation.h"


using namespace std;
using namespace SQLite;

int main()
{
    Database db("db.db", OPEN_READWRITE|OPEN_CREATE);
    CreateTables(db);
    MainMenu(db);
    cout << "GOODBYE\n";
    return 0;
}
