#include <iostream>
#include <string>
#include "floor.h"
#include <fstream>
using namespace std;

int main(){
  ifstream fin;
  int player_status[9] = {1,1000,10,10,20,50,10,0,0};
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
  bool GameOver = false;
  string user_name;
  cout << "Please input your name (<10 characters):";
  cin >> user_name;
  char ch;
  cout << "a: Start a new game\nb: Continues where you left off? \n('a'/'b'): ";
  cin >> ch;
  if(ch == 'a'){
    cout << "No records of this user name"<<endl;
    floor_1_main(player_status,user_name, GameOver);
  }else if (ch == 'b'){
    string filename = user_name+".txt";
    fin.open(filename.c_str());
    if (fin.is_open()){
      for (int i=0;i<9;i++){
        fin >> player_status[i];
      }
      // call functions of different levels
      if (player_status[0] == 1){floor_1_main(player_status,user_name, GameOver);}
      else if (player_status[0] == 2){floor_2_main(player_status,user_name, GameOver);}
      else if (player_status[0] == 3){floor_3_main(player_status,user_name, GameOver);}
      else if (player_status[0] == 4){floor_4_main(player_status,user_name);};
      fin.close();
    }else{
      floor_1_main(player_status,user_name, GameOver);
    }
  }
  return 0;
}
