from query_funcs import *
def exercise(C):
    print("-----------Case 1.0-----------") 
    query1(C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    print("-----------Case 1.1-----------")
    query1(C, 1, 35, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    print("-----------Case 1.2-----------")
    query1(C, 1, 35, 40, 1, 15, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    print("-----------Case 1.3-----------")
    query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    print("-----------Case 1.4-----------")
    query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 0, 0, 0, 0, 0, 0)
    print("-----------Case 1.5-----------")
    query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 1, 1, 2, 0, 0, 0)
    print("-----------Case 1.6-----------")
    query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 1, 1, 2, 1, 0.41, 0.51)
    print("-----------Case 1.7-----------")
    query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 1, 1, 2, 1, 0.6, 0.7)  
    
    print("-----------Case 2.1-----------")
    query2(C, "Orange")
    print("-----------Case 2.2-----------")
    query2(C, "Black")
    
    print("-----------Case 3.1-----------")
    query3(C, "BostonCollege")
    print("-----------Case 3.2-----------")
    query3(C, "Duke")
    
    print("-----------Case 4.1-----------")
    query4(C, "NC","Orange")
    print("-----------Case 4.2-----------")
    query4(C, "GA","Gold")
    print("-----------Case 4.3-----------")
    query4(C, "NC","LightBlue")
    
    print("-----------Case 5.1-----------")
    query5(C, 0)
    print("-----------Case 5.2-----------")
    query5(C, 5)
    print("-----------Case 5.3-----------")
    query5(C, 10)
    print("-----------Case 5.4-----------")
    query5(C, 11)
    print("-----------Case 5.5-----------")
    query5(C, 14)