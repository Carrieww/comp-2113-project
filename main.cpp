#include <iostream>
#include <fstream>
#include "level.h"
using namespace std;

int main(){
  ifstream fin;
  int player_status[12] = {1,1,1,1,0,100,10,5,0,0,0,0};
  // [0]: level of tower
  // [1]: x-coordinate
  // [2]: y-coordinate
  // [3]: player's Level
  // [4]: player's EXP
  // [5]: player's HP
  // [6]: player's ATK
  // [7]: player's DEF
  // [8]: player's Gold
  // [9]: player's '@' key
  // [10]: player's '$' key
  // [11]: player's '%' key

  fin.open("TofP_status.txt");
  if (fin.is_open()){
    for (int i=0;i<12;i++){
      fin >> player_status[i];
    }
  }
  // read data from 'TofP_status.txt'

  if (player_status[0] == 1){level_1(player_status);}
  else if (player_status[0] == 2){level_2(player_status);}
  else if (player_status[0] == 3){level_3(player_status);}
  else if (player_status[0] == 4){level_4(player_status);};
  // call functions of different levels

  fin.close();
  return 0;
}
