#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <vector>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void drop_all(connection * C) {
  string sql;
  string names[4] = {"PLAYER", "TEAM", "STATE", "COLOR"};
  for (int i = 0; i < 4; i++) {
    work W(*C);
    sql = "DROP TABLE IF EXISTS " + names[i] + ";";
    W.exec(sql);
    W.commit();
  }
}
void create_table(connection * C, string sql) {
  work W(*C);
  W.exec(sql);
  W.commit();
}

vector<string> extractor(const string & line) {
  stringstream ss(line);
  string entry;
  vector<string> res;
  while (getline(ss, entry, ' ')) {
    res.push_back(entry);
  }
  return res;
}

void parser_add_player(connection * C, const string & line) {
  vector<string> entries = extractor(line);
  add_player(C,
             stoi(entries[1]),
             stoi(entries[2]),
             entries[3],
             entries[4],
             stoi(entries[5]),
             stoi(entries[6]),
             stoi(entries[7]),
             stoi(entries[8]),
             stod(entries[9]),
             stod(entries[10]));
}
void parser_add_team(connection * C, const string & line) {
  vector<string> entries = extractor(line);
  add_team(C,
           entries[1],
           stoi(entries[2]),
           stoi(entries[3]),
           stoi(entries[4]),
           stoi(entries[5]));
}
void parser_add_state(connection * C, const string & line) {
  vector<string> entries = extractor(line);
  add_state(C, entries[1]);
}

void parser_add_color(connection * C, const string & line) {
  vector<string> entries = extractor(line);
  add_color(C, entries[1]);
}

void addAll_to_db(connection * C,
                  const string & file_name,
                  void (&func)(connection *, const string &)) {
  ifstream data_file(file_name);
  if (data_file.is_open()) {
    string line;
    while (getline(data_file, line)) {
      try {
        func(C, line);
      }
      catch (const std::exception & e) {
        cout << "Invalid input: " << e.what() << std::endl;
      }
    }
    data_file.close();
  }
  else {
    cout << "Cannot open the file " << file_name << endl;
  }
}

int main(int argc, char * argv[]) {
  //Allocate & initialize a Postgres connection object
  connection * C;
  string sql;
  try {
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    }
    else {
      cout << "Can't open database" << endl;
      return 1;
    }
  }
  catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  drop_all(C);
  sql = "CREATE TABLE PLAYER("
        "PLAYER_ID     SERIAL  PRIMARY KEY,"
        "TEAM_ID       INT,"
        "UNIFORM_NUM   INT,"
        "FIRST_NAME    TEXT,"
        "LAST_NAME     TEXT,"
        "MPG           INT,"
        "PPG           INT,"
        "RPG           INT,"
        "APG           INT,"
        "SPG           DOUBLE PRECISION,"
        "BPG           DOUBLE PRECISION);";
  create_table(C, sql);
  sql = "CREATE TABLE TEAM("
        "TEAM_ID    SERIAL  PRIMARY KEY,"
        "NAME       TEXT,"
        "STATE_ID   INT,"
        "COLOR_ID   INT,"
        "WINS       INT,"
        "LOSSES     INT);";
  create_table(C, sql);
  sql = "CREATE TABLE STATE("
        "STATE_ID   SERIAL  PRIMARY KEY,"
        "NAME       TEXT);";
  create_table(C, sql);
  sql = "CREATE TABLE COLOR("
        "COLOR_ID   SERIAL  PRIMARY KEY,"
        "NAME       TEXT);";
  create_table(C, sql);
  addAll_to_db(C, "player.txt", parser_add_player);
  addAll_to_db(C, "team.txt", parser_add_team);
  addAll_to_db(C, "state.txt", parser_add_state);
  addAll_to_db(C, "color.txt", parser_add_color);
  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}
