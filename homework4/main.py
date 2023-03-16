from sqlalchemy import create_engine,MetaData, Table
from sqlalchemy.orm import declarative_base,sessionmaker
from sqlalchemy import Column, Integer, String, Double


    
    

def main():
    C = create_engine('postgresql://postgres:passw0rd@localhost/ACC_BBALL')
    metadata = MetaData()
    base = declarative_base()
    metadata = MetaData()  
    metadata.reflect(bind=C)
    for table_name in ["PLAYER", "TEAM", "STATE", "COLOR"]:
        a_table = metadata.tables.get(table_name)
        if a_table is not None:
            base.metadata.drop_all(C, [a_table], checkfirst=True)
    metadata = MetaData()
    player_table = Table("PLAYER",
                         metadata,
                         Column("PLAYER_ID",Integer, primary_key=True, autoincrement=True),
                         Column("TEAM_ID",Integer),
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
                       Column("STATE_ID",Integer),
                       Column("COLOR_ID",Integer),
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
    W = sessionmaker(bind=C)()
    
    W.execute(color_table.insert().values(NAME = "A"))
    
    W.commit()
    

if __name__ == '__main__':
    main()
