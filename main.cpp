#include <iostream>
#include <string>
#include "floor.h"
#include <fstream>
using namespace std;

int main(){
  ifstream fin;
  int player_status[9] = {1,1000,10,10,20,50,10,0,0};
  // [0]: player's Level
  // [1]: player's EXP
  // [2]: player's HP
  // [3]: player's ATK
  // [4]: player's DEF
  // [5]: player's Gold
  // [6]: player's '@' key
  // [7]: player's '$' key
  // [8]: diamond
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
    int Floor;
    if (fin.is_open()){
      fin >> Floor;
      for (int i=0;i<9;i++){
        fin >> player_status[i];
      }
      // call functions of different levels
      if (Floor == 1){floor_1_main(player_status,user_name, GameOver);}
      else if (Floor  == 2){floor_2_main(player_status,user_name, GameOver);}
      else if (Floor  == 3){floor_3_main(player_status,user_name, GameOver);}
      //else if (Floor == 4){floor_4_main(player_status,user_name);};
      fin.close();
    }else{
      //floor_1_main(player_status,user_name, GameOver);
    }
  }

  return 0;
}
