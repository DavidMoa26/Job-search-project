#include <iostream>
<<<<<<< HEAD
#include "SQLiteCpp/SQLiteCpp.h"
#include "authentication.h"
#include "menus.h"
#include "candidate.h"
#include "employer.h"
#include "sqlite3.h"
enum ConsoleColor {
    FOREGROUND_BLUE      = 0x0001,
    FOREGROUND_GREEN     = 0x0002,
    FOREGROUND_RED       = 0x0004,
    FOREGROUND_INTENSITY = 0x0008,
    BACKGROUND_BLUE      = 0x0010,
    BACKGROUND_GREEN     = 0x0020,
    BACKGROUND_RED       = 0x0040,
    BACKGROUND_INTENSITY = 0x0080
};
=======
#include <SQLiteCpp/SQLiteCpp.h>
#include "menus.h"
#include "employer.h"

>>>>>>> b33e9a15ab5f96340d8258c7333df68ec46262d4
using namespace std;
using namespace SQLite;

int main()
{
    Database db("db.db", OPEN_READWRITE|OPEN_CREATE);
    MainMenu(db);
    cout << "GOODBYE\n";
    return 0;
}

