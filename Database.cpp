#include "Database.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <typeinfo>

static int callback(void *passed, int argc, char **argv, char **azColName) {
    return 0;
}

static int Net_callback(void *passed, int argc, char **argv, char **azColName) {
    Net * net = reinterpret_cast<Net *>(passed);
    net->id = atoi(argv[0]);
    net->size = atoi(argv[1]);
    return 0;
}

static int Layer_callback(void *passed, int argc, char **argv, char **azColName) {
    Net * net = reinterpret_cast<Net *>(passed);
    Layer::LayerType t;
    if(atoi(argv[2]) == 0) t = Layer::LayerType::Input;
    if(atoi(argv[2]) == 1) t = Layer::LayerType::Middle;
    if(atoi(argv[2]) == 2) t = Layer::LayerType::Output;
    net->layers.push_back(Layer(atoi(argv[0]), t, atoi(argv[3]), atoi(argv[4])));
    return 0;
}

static int Neuron_callback(void *passed, int argc, char **argv, char **azColName) {
    Net * net = reinterpret_cast<Net *>(passed);
    Neuron::NeuronType t;
    if(atoi(argv[3]) == 0) t = Neuron::NeuronType::Input;
    if(atoi(argv[3]) == 1) t = Neuron::NeuronType::Middle;
    if(atoi(argv[3]) == 2) t = Neuron::NeuronType::Output;
    net->layers[atoi(argv[7])].neurons.push_back(Neuron(atoi(argv[0]), t, atoi(argv[2]), atof(argv[5]), atof(argv[4]), atof(argv[6]), atoi(argv[7])));
    return 0;
}

static int Connection_callback(void *passed, int argc, char **argv, char **azColName) {
    Net * net = reinterpret_cast<Net *>(passed);
    Neuron::ConnectionType t;
    Neuron * curNeuron = net->get_Neuron(atoi(argv[0]));
    Neuron * connNeuron = net->get_Neuron(atoi(argv[1]));
    curNeuron->connect(connNeuron, atof(argv[3]));

    printf("tried id: %i to target: %i\n",atoi(argv[0]),atoi(argv[1]));

    return 0;
}

static int Nets_callback(void *passed, int argc, char **argv, char **azColName) {
    std::vector<std::string>* nets_m = static_cast<std::vector<std::string> * >(passed);
    //std::map<int, std::string> * nets_m = reinterpret_cast<map<int, std::string> *>(passed);
    char out[BUFFERSIZE];
    sprintf(out, "%s:%s", argv[0], argv[2]);
    nets_m->push_back(out);
    return 0;
}

static int Id_callback(void *passed, int argc, char **argv, char **azColName) {
    int* id = static_cast<int * >(passed);
    if(!argv[0]) {
        *id = 0;
        return 0;
    }
    *id = atoi(argv[0]) + 1;
    return 0;
}

Database::Database(std::string name) {
    int rc = sqlite3_open(name.c_str(), &db);
    ErrMsg = 0;
    if(rc) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Opened Databse\n");
        //return;
    }
    std::string sql = "CREATE TABLE nets(" \
        "id int PRIMARY KEY NOT NULL," \
        "size int NOT NULL," \
        "name text);";
    int response;
    response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
    sql = "CREATE TABLE layers(" \
        "id int PRIMARY KEY NOT NULL," \
        "net int NOT NULL," \
        "type int NOT NULL," \
        "size int NOT NULL,"\
        "number_in_net int NOT NULL);";
    response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
    sql = "CREATE TABLE neurons(" \
        "id int PRIMARY KEY NOT NULL," \
        "net int NOT NULL," \
        "layer int NOT NULL," \
        "type int NOT NULL," \
        "threshold float NOT NULL," \
        "bias float NOT NULL," \
        "fadetime float NOT NULL," \
        "layer_in_net int NOT NULL);";
    response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
    sql = "CREATE TABLE connections(" \
        "source int NOT NULL," \
        "target int NOT NULL," \
        "type int NOT NULL," \
        "weight int NOT NULL);";
    response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::retrieveNet(int id, Net * emptyNet) {
    std::string sql = "SELECT * FROM nets ";
    sql += "WHERE id = " + std::to_string(id) + ";";
    int response = sqlite3_exec(db, sql.c_str(), Net_callback, emptyNet, &ErrMsg);
    sql = "SELECT * FROM layers WHERE net = " + std::to_string(id) + " ORDER BY id;";
    response = sqlite3_exec(db, sql.c_str(), Layer_callback, emptyNet, &ErrMsg);
    sql = "SELECT * FROM neurons WHERE net = " + std::to_string(id) + " ORDER BY layer, id;";
    response = sqlite3_exec(db, sql.c_str(), Neuron_callback, emptyNet, &ErrMsg);

    for(int layer=0; layer < emptyNet->size; ++layer){
        Layer curLayer = emptyNet->layers[layer];
        for(int neuron=0; neuron< curLayer.size; ++neuron ){
            Neuron curNeuron = curLayer.neurons[neuron];
            sql = "SELECT * FROM connections WHERE source = " + std::to_string(curNeuron.id) + ";";
            response = sqlite3_exec(db, sql.c_str(), Connection_callback, emptyNet, &ErrMsg);
        }
    }

}
void Database::GetNets(std::vector<std::string> * id_names) {
    std::string sql = "SELECT * FROM nets ORDER BY id;";
    int response = sqlite3_exec(db, sql.c_str(), Nets_callback, id_names, &ErrMsg);

}

char Database::insertNet(Net * net) {
    std::string sql = "INSERT INTO nets(id, size) VALUES(" + std::to_string(net->id) + ", " + std::to_string(net->size) + ");";
    int response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
    for(int layer = 0; layer < net->size; ++layer) {
        std::string sql = "INSERT INTO layers VALUES(" + std::to_string(net->layers[layer].id) \
            + ", " + std::to_string(net->id) \
            + ", " + std::to_string(net->layers[layer].type)  \
            + ", " + std::to_string(net->layers[layer].size)  \
            + ", " + std::to_string(net->layers[layer].number_in_net) + ");";

        int response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
        for(int neuron = 0; neuron < net->layers[layer].size; ++neuron) {
            std::string sql = "INSERT INTO neurons VALUES(" + std::to_string(net->layers[layer].neurons[neuron].id) \
                + ", " + std::to_string(net->id) + ", " + std::to_string(net->layers[layer].id) \
                + ", " + std::to_string(net->layers[layer].neurons[neuron].type) \
                + ", " + std::to_string(net->layers[layer].neurons[neuron].threshold) \
                + ", " + std::to_string(net->layers[layer].neurons[neuron].bias) \
                + ", " + std::to_string(net->layers[layer].neurons[neuron].fadetime)  \
                + ", " + std::to_string(net->layers[layer].neurons[neuron].layer_in_net) + ");";
            int response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
            for(int conn = 0; conn < net->layers[layer].neurons[neuron].connections.size(); ++conn) {
                Neuron::Connection * curConn = &net->layers[layer].neurons[neuron].connections[conn];
                if(curConn->type == Neuron::ConnectionType::In) continue;
                std::string sql = "INSERT INTO connections VALUES(" + std::to_string(net->layers[layer].neurons[neuron].id) \
                    + ", " + std::to_string(curConn->partner->id) \
                    + ", " + std::to_string(curConn->type) \
                    + ", " + std::to_string(curConn->weight) + ");";
                int response = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
            }

        }
    }

}

void Database::nextId(std::string table, int * id) {
    char sql[BUFFERSIZE];
    sprintf(sql,"SELECT Max(id) FROM %s;", table.c_str());
    int response = sqlite3_exec(db, sql, Id_callback, id, &ErrMsg);
}