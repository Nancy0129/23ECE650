#include "query_funcs.h"

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
}

void query2(connection * C, string team_color) {
}

void query3(connection * C, string team_name) {
}

void query4(connection * C, string team_state, string team_color) {
}

void query5(connection * C, int num_wins) {
}
