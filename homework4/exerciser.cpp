#include "exerciser.h"

void exercise(connection * C) {
  //Testing query 1
  cout << "-----------Case 1.0-----------\n";
  query1(C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  cout << "-----------Case 1.1-----------\n";
  query1(C, 1, 35, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  cout << "-----------Case 1.2-----------\n";
  query1(C, 1, 35, 40, 1, 15, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  cout << "-----------Case 1.3-----------\n";
  query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  cout << "-----------Case 1.4-----------\n";
  query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 0, 0, 0, 0, 0, 0);
  cout << "-----------Case 1.5-----------\n";
  query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 1, 1, 2, 0, 0, 0);
  cout << "-----------Case 1.6-----------\n";
  query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 1, 1, 2, 1, 0.41, 0.51);
  cout << "-----------Case 1.7-----------\n";
  query1(C, 1, 35, 40, 1, 10, 20, 1, 5, 8, 1, 2, 10, 1, 1, 2, 1, 0.6, 0.7);
  cout << "-----------Case 2.1-----------\n";
  query2(C, "Orange");
  cout << "-----------Case 2.2-----------\n";
  query2(C, "Black");

  cout << "-----------Case 3.1-----------\n";
  query3(C, "BostonCollege");
  cout << "-----------Case 3.2-----------\n";
  query3(C, "Duke");

  cout << "-----------Case 4.1-----------\n";
  query4(C, "NC","Orange");
  cout << "-----------Case 4.2-----------\n";
  query4(C, "GA","Gold");
  cout << "-----------Case 4.3-----------\n";
  query4(C, "NC","LightBlue");

  cout << "-----------Case 5.1-----------\n";
  query5(C, 0);
  cout << "-----------Case 5.2-----------\n";
  query5(C, 5);
  cout << "-----------Case 5.3-----------\n";
  query5(C, 10);
  cout << "-----------Case 5.4-----------\n";
  query5(C, 11);
  cout << "-----------Case 5.5-----------\n";
  query5(C, 14);
}
