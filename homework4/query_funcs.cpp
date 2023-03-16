#include "query_funcs.h"

#include <iomanip>

void add_player(connection * C,
                int team_id,
                int jersey_num,
                string first_name,
                string last_name,
                int mpg,
                int ppg,
                int rpg,
                int apg,
                double spg,
                double bpg) {
  work W(*C);
  string sql;
  sql = "INSERT INTO  PLAYER  ( TEAM_ID ,  UNIFORM_NUM , "
        " FIRST_NAME,  LAST_NAME,  MPG,  PPG,  RPG,  APG,  SPG, "
        " BPG ) ";
  sql += "VALUES (";
  sql += to_string(team_id) + ",";
  sql += to_string(jersey_num) + ",";
  sql += W.quote(first_name) + ",";
  sql += W.quote(last_name) + ",";
  sql += to_string(mpg) + ",";
  sql += to_string(ppg) + ",";
  sql += to_string(rpg) + ",";
  sql += to_string(apg) + ",";
  sql += to_string(spg) + ",";
  sql += to_string(bpg) + ");";
  W.exec(sql);
  W.commit();
  //cout<<sql<<endl;
}

void add_team(connection * C,
              string name,
              int state_id,
              int color_id,
              int wins,
              int losses) {
  work W(*C);
  string sql;
  sql = "INSERT INTO  TEAM  ( NAME,  STATE_ID,  COLOR_ID, "
        " WINS,  LOSSES ) ";
  sql += "VALUES (";
  sql += W.quote(name) + ",";
  sql += to_string(state_id) + ",";
  sql += to_string(color_id) + ",";
  sql += to_string(wins) + ",";
  sql += to_string(losses) + ");";
  W.exec(sql);
  W.commit();
  //cout<<sql<<endl;
}

void add_state(connection * C, string name) {
  work W(*C);
  string sql;
  sql = "INSERT INTO STATE (NAME) ";
  sql += "VALUES (";
  sql += W.quote(name) + ");";
  W.exec(sql);
  W.commit();
  //cout<<sql<<endl;
}

void add_color(connection * C, string name) {
  work W(*C);
  string sql;
  sql = "INSERT INTO COLOR (NAME) ";
  sql += "VALUES (";
  sql += W.quote(name) + ");";
  W.exec(sql);
  W.commit();
  //cout<<sql<<endl;
}

void not_exist_add(string & sql) {
  if (sql.find("WHERE") == string::npos) {
    sql += "WHERE";
  }
  else {
    sql += " AND";
  }
}

/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */
void query1(connection * C,
            int use_mpg,
            int min_mpg,
            int max_mpg,
            int use_ppg,
            int min_ppg,
            int max_ppg,
            int use_rpg,
            int min_rpg,
            int max_rpg,
            int use_apg,
            int min_apg,
            int max_apg,
            int use_spg,
            double min_spg,
            double max_spg,
            int use_bpg,
            double min_bpg,
            double max_bpg) {
  work W(*C);
  string sql;
  sql = "SELECT * FROM PLAYER ";
  if (use_mpg == 1) {
    sql += "WHERE MPG BETWEEN " + to_string(min_mpg) + " AND " + to_string(max_mpg);
  }
  if (use_ppg == 1) {
    not_exist_add(sql);
    sql += " PPG BETWEEN " + to_string(min_ppg) + " AND " + to_string(max_ppg);
  }
  if (use_rpg == 1) {
    not_exist_add(sql);
    sql += " RPG BETWEEN " + to_string(min_rpg) + " AND " + to_string(max_rpg);
  }
  if (use_apg == 1) {
    not_exist_add(sql);
    sql += " APG BETWEEN " + to_string(min_apg) + " AND " + to_string(max_apg);
  }
  if (use_spg == 1) {
    not_exist_add(sql);
    sql += " SPG BETWEEN " + to_string(min_spg) + " AND " + to_string(max_spg);
  }
  if (use_bpg == 1) {
    not_exist_add(sql);
    sql += " BPG BETWEEN " + to_string(min_bpg) + " AND " + to_string(max_bpg);
  }
  sql += ";";
  result res = W.exec(sql);
  cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG\n";
  for (result::const_iterator r = res.begin(); r != res.end(); r++) {
    cout << r[0].as<int>() << " ";
    cout << r[1].as<int>() << " ";
    cout << r[2].as<int>() << " ";
    cout << r[3].as<string>() << " ";
    cout << r[4].as<string>() << " ";
    cout << r[5].as<int>() << " ";
    cout << r[6].as<int>() << " ";
    cout << r[7].as<int>() << " ";
    cout << r[8].as<int>() << " ";
    cout << std::fixed << std::setprecision(1) << r[9].as<double>() << " ";
    cout << std::fixed << std::setprecision(1) << r[10].as<double>() << endl;
  }
  W.commit();
}

void query2(connection * C, string team_color) {
  work W(*C);
  string sql = "SELECT TEAM.NAME FROM TEAM, COLOR "
               "WHERE TEAM.COLOR_ID = COLOR.COLOR_ID  "
               "AND COLOR.NAME = " +
               W.quote(team_color) + ";";
  result res = W.exec(sql);
  cout << "NAME\n";
  for (result::const_iterator r = res.begin(); r != res.end(); r++) {
    cout << r[0].as<string>() << endl;
  }
  W.commit();
}

void query3(connection * C, string team_name) {
  work W(*C);
  string sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM "
               "WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID "
               "AND TEAM.NAME = " +
               W.quote(team_name) +
               " "
               "ORDER BY PPG DESC;";
  result res = W.exec(sql);
  cout << "FIRST_NAME LAST_NAME\n";
  for (result::const_iterator r = res.begin(); r != res.end(); r++) {
    cout << r[0].as<string>() << " ";
    cout << r[1].as<string>() << endl;
  }
  W.commit();
}

void query4(connection * C, string team_state, string team_color) {
  work W(*C);
  string sql = "SELECT PLAYER.UNIFORM_NUM, PLAYER.FIRST_NAME, PLAYER.LAST_NAME "
               "FROM PLAYER, TEAM, STATE, COLOR "
               "WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID "
               "AND TEAM.STATE_ID = STATE.STATE_ID "
               "AND TEAM.COLOR_ID = COLOR.COLOR_ID "
               "AND STATE.NAME = " +
               W.quote(team_state) +
               " "
               "AND COLOR.NAME = " +
               W.quote(team_color) + ";";
  result res = W.exec(sql);
  cout << "UNIFORM_NUM FIRST_NAME LAST_NAME\n";
  for (result::const_iterator r = res.begin(); r != res.end(); r++) {
    cout << r[0].as<int>() << " ";
    cout << r[1].as<string>() << " ";
    cout << r[2].as<string>() << endl;
  }
  W.commit();
}

void query5(connection * C, int num_wins) {
  work W(*C);
  string sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS "
               "FROM PLAYER, TEAM "
               "WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID "
               "AND TEAM.WINS > " +
               to_string(num_wins) + ";";
  result res = W.exec(sql);
  cout << "FIRST_NAME LAST_NAME NAME WINS\n";
  for (result::const_iterator r = res.begin(); r != res.end(); r++) {
    cout << r[0].as<string>() << " ";
    cout << r[1].as<string>() << " ";
    cout << r[2].as<string>() << " ";
    cout << r[3].as<int>() << endl;
  }
  W.commit();
}
