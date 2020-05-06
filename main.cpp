#include <iostream>
#include <string>
#include "floor.h"
#include <fstream>
using namespace std;

int main(){
  ifstream fin;
  int player_status[10] = {1,1,500,20,10,0,0,0,0,0};
  // [0]: floor of tower
  // [1]: player's Level
  // [2]: player's HP
  // [3]: player's ATK
  // [4]: player's DEF
  // [5]: player's EXP
  // [6]: player's GOLD
  // [7]: player's '^' key
  // [8]: player's '&' key
  // [9]: player's diamond

  bool GameOver = false;
  string user_name;
  cout << "Please input your name (<10 characters):";
  cin >> user_name;
  char ch;
  cout << "a: Start a new game\nb: Continues where you left off? \n('a'/'b'): ";
  cin >> ch;
  if(ch == 'a'){
    cout << "No records of this user name."<<endl;
    floor_1_main(player_status,user_name, GameOver);
  }else if (ch == 'b'){
    string filename = "output/"+user_name+".txt";
    fin.open(filename.c_str());
    if (fin.is_open()){
      for (int i=0;i<9;i++){
        fin >> player_status[i];
      }
      // call functions of different levels
      if (player_status[0] == 1){floor_1_main(player_status,user_name, GameOver);}
      else if (player_status[0] == 2){floor_2_main(player_status,user_name, GameOver);}
      else if (player_status[0] == 3){floor_3_main(player_status,user_name, GameOver);}
      else if (player_status[0] == 4){floor_4_main(player_status,user_name, GameOver);};
      fin.close();
    }else{
      floor_1_main(player_status,user_name, GameOver);
    }
  }
  return 0;
}
