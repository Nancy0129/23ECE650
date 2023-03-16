from sqlalchemy import create_engine,MetaData, Table
from sqlalchemy.orm import declarative_base,sessionmaker
from sqlalchemy import Column, Integer, String, Double,ForeignKey
from exerciser import *
def dropAll(C):
    base = declarative_base()
    metadata = MetaData()  
    metadata.reflect(bind=C)
    for table_name in ["PLAYER", "TEAM", "STATE", "COLOR"]:
        a_table = metadata.tables.get(table_name)
        if a_table is not None:
            base.metadata.drop_all(C, [a_table], checkfirst=True)
def parse_add_player(C, entries):
    add_player(C,
             int(entries[1]),
             int(entries[2]),
             entries[3],
             entries[4],
             int(entries[5]),
             int(entries[6]),
             int(entries[7]),
             int(entries[8]),
             float(entries[9]),
             float(entries[10])) 
      
def parse_add_team(C, entries):
    add_team(C,
           entries[1],
           int(entries[2]),
           int(entries[3]),
           int(entries[4]),
           int(entries[5]))
    
def parse_add_state(C, entries):
    add_state(C, entries[1])

def parse_add_color(C, entries):
    add_color(C, entries[1])

def parser_add_all(C, file_name, parse_add):
    with open(file_name, 'r') as data_file:
        for line in data_file:
            parse_add(C,line.replace('\n','').split(' '))
    
def main():
    C = create_engine('postgresql://postgres:passw0rd@localhost/ACC_BBALL')
    print("Opened database successfully: ACC_BBALL")
    dropAll(C)
    metadata = MetaData()
    player_table = Table("PLAYER",
                         metadata,
                         Column("PLAYER_ID",Integer, primary_key=True, autoincrement=True),
                         Column("TEAM_ID",Integer,ForeignKey("TEAM.TEAM_ID")),
                         Column("UNIFORM_NUM",Integer),
                         Column("FIRST_NAME",String),
                         Column("LAST_NAME",String),
                         Column("MPG",Integer),
                         Column("PPG",Integer),
                         Column("RPG",Integer),
                         Column("APG",Integer),
                         Column("SPG",Double),
                         Column("BPG",Double)
                         )
    team_table = Table("TEAM",
                       metadata,
                       Column("TEAM_ID",Integer, primary_key=True, autoincrement=True),
                       Column("NAME",String),
                       Column("STATE_ID",Integer,ForeignKey("STATE.STATE_ID")),
                       Column("COLOR_ID",Integer,ForeignKey("COLOR.COLOR_ID")),
                       Column("WINS",Integer),
                       Column("LOSSES",Integer)
                       )
    state_table = Table("STATE",
                        metadata,
                        Column("STATE_ID",Integer, primary_key=True, autoincrement=True),
                        Column("NAME",String)
                        )
    color_table = Table("COLOR",
                        metadata,
                        Column("COLOR_ID",Integer, primary_key=True, autoincrement=True),
                        Column("NAME",String)
                        )
    metadata.create_all(C)
    parser_add_all(C, "color.txt",parse_add_color)
    parser_add_all(C, "state.txt",parse_add_state)
    parser_add_all(C, "team.txt",parse_add_team)
    parser_add_all(C, "player.txt",parse_add_player)
    # query2(C,"O'); DROP TABLE COLOR;--")
    # query2(C,"Gold")
    exercise(C)  
    
if __name__ == '__main__':
    main()
