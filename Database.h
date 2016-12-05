#include <sqlite3.h>
#include <string>
#include <map>
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
    void retrieveNet(int id, Net * emptyNet );
    void GetNets(std::vector<std::string> * id_names);
    void nextId(std::string table, int * id);

};