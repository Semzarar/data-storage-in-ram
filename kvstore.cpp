#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class KVStore {
    unordered_map<string, string> store;

public:
    string set(const string& key, const string& value) {
        store[key] = value;
        return "OK";
    }

    string get(const string& key) {
        auto it = store.find(key);
        if (it != store.end())
            return it->second;
        return "(nil)";
    }

    string del(const string& key) {
        return store.erase(key) ? "1" : "0";
    }

    string exists(const string& key) {
        return store.count(key) ? "1" : "0";
    }

    string keys() {
        if (store.empty()) return "(empty)";
        string result;
        int i = 1;
        for (auto& [k, v] : store)
            result += to_string(i++) + ") \"" + k + "\"\n";
        return result;
    }

    string flushall() {
        store.clear();
        return "OK";
    }
};

vector<string> tokenize(const string& line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

int main() {
    KVStore db;
    string line;

    cout << "=== In-Memory KV Store ===\n";
    cout << "Commands: SET <key> <value> | GET <key> | DEL <key> | EXISTS <key> | KEYS | FLUSHALL | EXIT\n\n";

    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        string cmd = tokens[0];
        // uppercase the command
        for (auto& c : cmd) c = toupper(c);

        string response;

        if (cmd == "EXIT" || cmd == "QUIT") {
            cout << "Bye!\n";
            break;
        } else if (cmd == "SET") {
            if (tokens.size() < 3)
                response = "ERR: SET requires 2 arguments: SET <key> <value>";
            else
                response = db.set(tokens[1], tokens[2]);
        } else if (cmd == "GET") {
            if (tokens.size() < 2)
                response = "ERR: GET requires 1 argument: GET <key>";
            else
                response = db.get(tokens[1]);
        } else if (cmd == "DEL") {
            if (tokens.size() < 2)
                response = "ERR: DEL requires 1 argument: DEL <key>";
            else
                response = db.del(tokens[1]);
        } else if (cmd == "EXISTS") {
            if (tokens.size() < 2)
                response = "ERR: EXISTS requires 1 argument: EXISTS <key>";
            else
                response = db.exists(tokens[1]);
        } else if (cmd == "KEYS") {
            response = db.keys();
        } else if (cmd == "FLUSHALL") {
            response = db.flushall();
        } else {
            response = "ERR: Unknown command '" + tokens[0] + "'";
        }

        cout << response << "\n";
    }

    return 0;
}
