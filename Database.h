#include <sqlite3.h>
#include <string>
#include "Macros.h"
#include "Net.h"

class Database {
    private:
     sqlite3 *db;
    public:
     char *ErrMsg;


    Database(std::string name);
    ~Database();
    char insertNet(Net * net);
    Net retrieveNet(int id, std::string name);
};