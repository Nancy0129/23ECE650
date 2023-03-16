from sqlalchemy import MetaData, text
from sqlalchemy.orm import sessionmaker
# from urllib.parse import quote_plus
from sqlalchemy.sql import select
def add_player(C,
               team_id,
               jersey_num,
               first_name,
               last_name,
               mpg,
               ppg,
               rpg,
               apg,
               spg,
               bpg):
    metadata = MetaData()
    metadata.reflect(bind=C)
    player_table = metadata.tables.get("PLAYER")
    W = sessionmaker(bind=C)()
    W.execute(player_table.insert().values(TEAM_ID=team_id,
                                            UNIFORM_NUM=jersey_num,
                                            FIRST_NAME=first_name,
                                            LAST_NAME=last_name,
                                            MPG=mpg,
                                            PPG=ppg,
                                            RPG=rpg,
                                            APG=apg,
                                            SPG=spg,
                                            BPG=bpg))
    W.commit()
    
def add_team(C,name,state_id,color_id,wins,losses):
    metadata = MetaData()
    metadata.reflect(bind=C)
    team_table = metadata.tables.get("TEAM")
    W = sessionmaker(bind=C)()
    W.execute(team_table.insert().values(NAME=name,
                                         STATE_ID=state_id,
                                         COLOR_ID=color_id,
                                         WINS=wins,
                                         LOSSES=losses))
    W.commit()
    
def add_state(C,name):
    metadata = MetaData()
    metadata.reflect(bind=C)
    state_table = metadata.tables.get("STATE")
    W = sessionmaker(bind=C)()
    W.execute(state_table.insert().values(NAME=name))
    W.commit()

def add_color(C,name):
    metadata = MetaData()
    metadata.reflect(bind=C)
    color_table = metadata.tables.get("COLOR")
    W = sessionmaker(bind=C)()
    W.execute(color_table.insert().values(NAME=name))
    W.commit()

def query1(C,
           use_mpg,
           min_mpg,
           max_mpg,
           use_ppg,
           min_ppg,
           max_ppg,
           use_rpg,
           min_rpg,
           max_rpg,
           use_apg,
           min_apg,
           max_apg,
           use_spg,
           min_spg,
           max_spg,
           use_bpg,
           min_bpg,
           max_bpg):
    
    metadata = MetaData()
    metadata.reflect(bind=C)
    player_table = metadata.tables.get("PLAYER")
    W = sessionmaker(bind=C)()
    query = player_table.select()
    if(use_mpg==1):
        query=query.where(player_table.c.MPG.between(min_mpg,max_mpg))
    if(use_ppg==1):
        query=query.where(player_table.c.PPG.between(min_ppg,max_ppg))
    if(use_rpg==1):
        query=query.where(player_table.c.RPG.between(min_rpg,max_rpg))
    if(use_apg==1):
        query=query.where(player_table.c.APG.between(min_apg,max_apg))
    if(use_spg==1):
        query=query.where(player_table.c.SPG.between(min_spg,max_spg))
    if(use_bpg==1):
        query=query.where(player_table.c.BPG.between(min_bpg,max_bpg))
        
    res = W.execute(query)
    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
    for r in res:
        print(r[0],
              r[1],
              r[2],
              r[3],
              r[4],
              r[5],
              r[6],
              r[7],
              r[8],
              '%.1f'%r[9],
              '%.1f'%r[10])

def query2(C,  team_color):
    metadata = MetaData()
    metadata.reflect(bind=C)
    team = metadata.tables.get("TEAM")
    color = metadata.tables.get("COLOR")
    W = sessionmaker(bind=C)()
    query=team.join(color).select().where(color.c.NAME==team_color)
    query=select(query.c.NAME)
    
    res = W.execute(query)
    
    print("NAME")
    for r in res:
        print(r[0])

def query3(C, team_name):
    metadata = MetaData()
    metadata.reflect(bind=C)
    player = metadata.tables.get("PLAYER")
    team = metadata.tables.get("TEAM")
    W = sessionmaker(bind=C)()
    query = player.join(team).select().where(team.c.NAME==team_name)
    query=query.order_by(player.c.PPG.desc())
    query=select(query.c.FIRST_NAME,query.c.LAST_NAME)
    res = W.execute(query)
    print("FIRST_NAME LAST_NAME")
    for r in res:
        print(r[0],r[1])

def query4(C, team_state, team_color):
    metadata = MetaData()
    metadata.reflect(bind=C)
    player = metadata.tables.get("PLAYER")
    team = metadata.tables.get("TEAM")
    state= metadata.tables.get("STATE")
    color = metadata.tables.get("COLOR")
    W = sessionmaker(bind=C)()
    query = player.join(team).join(color).join(state).select()
    query=query.where(state.c.NAME ==(team_state),
                      color.c.NAME ==(team_color))    
    query=select(query.c.UNIFORM_NUM,query.c.FIRST_NAME,query.c.LAST_NAME)
    res = W.execute(query)
    print("UNIFORM_NUM FIRST_NAME LAST_NAME")
    for r in res:
        print(r[0],r[1],r[2])
    
def query5( C,  num_wins):
    metadata = MetaData()
    metadata.reflect(bind=C)
    player = metadata.tables.get("PLAYER")
    team = metadata.tables.get("TEAM")
    W = sessionmaker(bind=C)()
    query = player.join(team).select().where(team.c.WINS>num_wins)
    query=select(query.c.FIRST_NAME,query.c.LAST_NAME,query.c.NAME,query.c.WINS)
    res = W.execute(query)
    print("FIRST_NAME LAST_NAME NAME WINS")
    for r in res:
        print(r[0],r[1],r[2],r[3])
    

    
    
    