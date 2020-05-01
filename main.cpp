#include <iostream>
#include "floor.h"
#include <fstream>
using namespace std;

int main(){
  ifstream fin;
  int player_status[9] = {2,1000,10,10,20,50,10,0,0};
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
  fin.close();

  // read data from 'TofP_status.txt'
  char user_name[10];
  cout << "please input your name (<10 characters):";
  cin >> user_name;
  if (player_status[0] == 1){floor_1_main(player_status,user_name);}
  else if (player_status[0] == 2){floor_2_main(player_status,user_name);}
  else if (player_status[0] == 3){floor_3_main(player_status,user_name);}
  else if (player_status[0] == 4){floor_4_main(player_status,user_name);};
  // call functions of different levels

  return 0;
}
