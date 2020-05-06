//Floor 1
#include <string>
#include <time.h>
#include "floor.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fstream>
#include <iostream>
using namespace std;
/* symbol used on floor 1
#: wall_l4, space between wall_l4s is the door connecting different floor
H: HP_box, increase 200 HP
A: ATK_box, increase 5 ATK
D: DEF_box, increase 5 DEF
@: door    ^: key for @
M: monsters with fixed positions
m: small monsters with random positions
?: guidance
S: store
*/

//global variables
const int width_l4 = 20, height_l4 = 10, num_fixed_monster_l4 = 3, num_fixed_door_l4 = 1, num_random_monster_l4= 15,
num_guidance_l4 = 3,num_special_door_l4 =1;
int printable_num_fixed_door_l4 = 1, printable_num_fixed_monster_l4 = 3, printable_num_random_monster_l4 = 15,
printable_num_special_door_l4 = 1, printable_num_guidance_l4 = 3,m_x_l4,m_y_l4;

//store everything with position into catagory and into this struct
struct Fixed_sth_coordinate{
	int x;
	int y;
	int ATK;
	int DEF;
	int HP;
	char type;
};

//use struct to store fixed and random monster's attribute, treasure's/door's/boxes' position,
Fixed_sth_coordinate * monster_l4 = new Fixed_sth_coordinate [num_fixed_monster_l4];
Fixed_sth_coordinate * door_l4 = new Fixed_sth_coordinate [num_fixed_door_l4];
Fixed_sth_coordinate * random_monster_l4 = new Fixed_sth_coordinate [num_random_monster_l4];
Fixed_sth_coordinate * guidance_l4 = new Fixed_sth_coordinate[num_guidance_l4];
Fixed_sth_coordinate * special_door_l4 = new Fixed_sth_coordinate[num_special_door_l4];
int x_l4, y_l4;
int small_monster_x_l4[num_random_monster_l4], small_monster_y_l4[num_random_monster_l4];

int count_Setup_l4_l4 = 0;
bool save_l4, IsH_l4, IsA_l4, IsD_l4, Is_s_l4ur_l4, Is_Info_l4, Is_guidance_l4, Is_m_l4,Is_s_l4= false;
bool wall_l4[12][22];
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir_l4;

void move_l4(int x,int y){
  printf("\033[%d;%dH", (x+1), (y+1));
}
void clear_l4(){
  printf("\033[2J");
}
int getch_l4(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
char mvinch_l4(int x, int y){
  if (wall_l4[x][y] == true){
    return ('#');
  }
  for(int k = 0; k < printable_num_fixed_monster_l4; ++k){
    if (monster_l4[k].x == x && monster_l4[k].y == y){
      return ('M');
    }
	}
	//print existing doors
	for(int k = 0; k < printable_num_fixed_door_l4; ++k){
		if(door_l4[k].x == x &&  door_l4[k].y== y){
      return ('@');
    }
	}
  //print special doors
  for(int k = 0; k < printable_num_special_door_l4; ++k){
		if(special_door_l4[k].x == x &&  special_door_l4[k].y== y){
      return ('$');
    }
	}
	//print exsiting random monsters
	for(int k = 0; k < printable_num_random_monster_l4; ++k){
		if(random_monster_l4[k].x == x && random_monster_l4[k].y == y){
      return ('m');
    }
	}

  //print guidance
	for(int k = 0; k < printable_num_guidance_l4; ++k){
		if(guidance_l4[k].x==x &&guidance_l4[k].y==y){
      return ('?');
    }
	}
  return (' ');
}
void saving_status_l4(int * role_attribute,string user_name, bool &GameOver){
	move_l4(12,0);
	string filename = "output/"+user_name + ".txt";
	ofstream fout;
	fout.open(filename.c_str());
	for(int i = 0; i < 9; i++){
		fout << role_attribute[i]<<" ";
	}
	fout << role_attribute[9]<< std::endl;
	fout.close();
	printf("game status already saved!\ngame over...\n");
	GameOver = true;
}
//Setup_l4 all positions in this floor
void Setup_l4(){
	dir_l4 = STOP;
	x_l4 = 1;
	y_l4 = 1;
	small_monster_x_l4[0] = 1;
  small_monster_y_l4[0] = 11;
  small_monster_x_l4[1] = 1;
  small_monster_y_l4[1] = 12;
  small_monster_x_l4[2] = 1;
  small_monster_y_l4[2] = 13;
  small_monster_x_l4[3] = 1;
  small_monster_y_l4[3] = 14;
  small_monster_x_l4[4] = 1;
  small_monster_y_l4[4] = 15;
  small_monster_x_l4[5] = 5;
  small_monster_y_l4[5] = 5;
  small_monster_x_l4[6] = 5;
  small_monster_y_l4[6] = 6;
  small_monster_x_l4[7] = 5;
  small_monster_y_l4[7] = 7;
  small_monster_x_l4[8] = 5;
  small_monster_y_l4[8] = 8;
  small_monster_x_l4[9] = 5;
  small_monster_y_l4[9] = 9;
  small_monster_x_l4[10] = 9 + rand()%2;
  small_monster_y_l4[10] = 9 + rand()%2;
  small_monster_x_l4[11] = 9 + rand()%2;
  small_monster_y_l4[11] = 11 + rand()%2;
  small_monster_x_l4[12] = 9 + rand()%2;
  small_monster_y_l4[12] = 13 + rand()%2;
  small_monster_x_l4[13] = 9 + rand()%2;
  small_monster_y_l4[13] = 15 + rand()%2;
  small_monster_x_l4[14] = 9 + rand()%2;
  small_monster_y_l4[14] = 17 + rand()%2;


	//store fixed monsters into array
	//store them into struct
	int monster_x_l4[num_fixed_monster_l4] = {3,5,7};
	int monster_y_l4[num_fixed_monster_l4] = {13,13,13};
	for(int i = 0; i < num_fixed_monster_l4; i++){
		monster_l4[i].x = monster_x_l4[i];
		monster_l4[i].y = monster_y_l4[i];
		monster_l4[i].HP = 1000;
		monster_l4[i].ATK = 100;
		monster_l4[i].DEF = 100;
    monster_l4[i].type = 'M';
	}

	//store fixed doors into array
	//store them into struct
	int door_x_l4[num_fixed_door_l4] = {8};
	int door_y_l4[num_fixed_door_l4] = {20};
	for(int i = 0; i < num_fixed_door_l4; i++){
		door_l4[i].x = door_x_l4[i];
		door_l4[i].y = door_y_l4[i];
		door_l4[i].type = '@';
	}
  //store special doors into array
	//store them into struct
	int special_door_x_l4[num_fixed_door_l4] = {2};
	int special_door_y_l4[num_fixed_door_l4] = {20};
	for(int i = 0; i < num_special_door_l4; i++){
		special_door_l4[i].x = special_door_x_l4[i];
		special_door_l4[i].y = special_door_y_l4[i];
		special_door_l4[i].type = '$';
	}

	//store random monsters into array
	//store them into struct
	for(int i = 0; i < num_random_monster_l4; i++){
		random_monster_l4[i].x = small_monster_x_l4[i];
		random_monster_l4[i].y = small_monster_y_l4[i];
		random_monster_l4[i].HP = 100;
		random_monster_l4[i].ATK = 30;
		random_monster_l4[i].DEF = 20;
    random_monster_l4[i].type = 'm';
	}

  //save the location of guidance
  int guidance_x_l4[num_guidance_l4] = {3,7,5};
  int guidance_y_l4[num_guidance_l4] = {3,3,20};
  for (int i=0;i<num_guidance_l4;i++){
    guidance_l4[i].x = guidance_x_l4[i];
    guidance_l4[i].y = guidance_y_l4[i];
    guidance_l4[i].type = '?';
  }

}
void upgrade_l4(int* role_attribute){
	//atk
	role_attribute[3] = role_attribute[3] + 5;
	//def
	role_attribute[4] = role_attribute[4] + 5;
	//HP
	role_attribute[2] = role_attribute[5] + 100;
}

void update_attribute_l4(int hp_value, int atk_value, int def_value,
	int add_exp, int add_gold, int* role_attribute){
	int hp = hp_value, atk = atk_value, def = def_value;
	hp = hp - (role_attribute[3]-def);
	while(hp > 0){
		if (atk > role_attribute[4]){
			role_attribute[2] = role_attribute[2] - (atk - role_attribute[4]);
		};
		hp = hp - (role_attribute[3]-def);
	}
	role_attribute[5] = role_attribute[5] + add_exp;
	role_attribute[6] = role_attribute[6] + add_gold;
	if(role_attribute[5] > 99){
		role_attribute[1]++;
		role_attribute[5] = role_attribute[5] - 99;
		upgrade_l4(role_attribute);
	}
}
bool mtest_l4(){
  move_l4(12,0);
  int a = rand()%3;
  if (a == 0){
    move_l4(12,0);
    printf("If we intend to open a file, f.fail() function can check if the file exists or not");
    move_l4(13,0);
    printf("T/F");
    char c = getch_l4();
    while (c != 't' && c != 'f'){
      c = getch_l4();
    }
    if (c == 't'){
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(14,0);
      printf("\033[K");
      move_l4(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(14,0);
      printf("\033[K");
      move_l4(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  if (a == 1){
    move_l4(12,0);
    printf("Setprecision() function can show a number with certain decimal places");
    move_l4(13,0);
    printf("T/F");
    char c = getch_l4();
    while (c != 't' && c != 'f'){
      c = getch_l4();
    }
    if (c == 't'){
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(14,0);
      printf("\033[K");
      move_l4(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(14,0);
      printf("\033[K");
      move_l4(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  if (a == 2){
    move_l4(12,0);
    printf("We can change the content of an array using '=' after declaration");
    move_l4(13,0);
    printf("T/F");
    char c = getch_l4();
    while (c != 't' && c != 'f'){
      c = getch_l4();
    }
    if (c == 't'){
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(14,0);
      printf("\033[K");
      move_l4(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(14,0);
      printf("\033[K");
      move_l4(12,0);
      printf("It is weakened!");
      return true;
    }
  }

  return true;
}
// guide function to give 3 guidances at the beginning
void guide(int x, int y, std::string user_name, int *role_attribute, bool &GameOver){
  if (x == 5 && y == 20){
    move_l4(12,0);
    cout << "Hi " << user_name << ", I AM Dr. L!";
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("I know you have come across many monsters, how do you feel:)");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Do you think you have known more about programming?");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("In fact, I am always waiting for you, young man.");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("For some reason, I cannot get out of the tower.");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("But the world needs programmer!");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("I can give you all my codes, and then you can deal with any problem in the world.");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("You are gonna become the most famous programmer in the world!");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Just say yes, your life will be totally changed!");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Are you ready for downloading all my codes?");
    printf(" press y to accept, n to reject");
    char cc = getch_l4();
    while (cc != 'y' && cc != 'n'){
      cc = getch_l4();
    }
    if (cc == 'y'){
      move_l4(12,0);
      printf("\033[K");
      printf("HAHHHHHHHHHH!");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("You really believe me??");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("There is never free lunch for you, young man.");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("I am not Dr.L! Just one of his programs!");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("Get out!! If you want to simply get codes without learning-");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("-then you don't deserve meeting the true Dr.L!");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("I will let you learn this lesson! GET OUT!");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      x_l4 = 1;
      y_l4 = 1;
      role_attribute[2] = role_attribute[2] / 2;
      role_attribute[3] = role_attribute[3] / 2;
      role_attribute[4] = role_attribute[4] / 2;
    } else {
      move_l4(12,0);
      printf("\033[K");
      printf("Wise move, young man.");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("In fact, I am not Dr. L. Just one of his programs.");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("True Dr.L will never let students simply get codes without learning.");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("Go find help with the man trapped below me.");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
      move_l4(12,0);
      printf("He will let you know how to find Dr. L");
      printf(" (press p to continue)\n");
      while (getch_l4() != 'p'){
      }
      move_l4(12,0);
      printf("\033[K");
      move_l4(13,0);
      printf("\033[K");
    }
  }
  if (x==7 && y==3){
    printf("Welcome, young man");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("I guess you want to seek for Dr.L");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("I am one of his teaching assitants");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("He has blocked himself in this tower for years");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("No one knows why but he seemed to be really upset");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("He told me if a great young man arrives -");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("- Give him or her this key");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("That's all I can help you");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Please help Dr. L and here is your key");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Dr. L is right above me.");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    role_attribute[8] = 1;
  }
  if (x==3 && y==3){
    printf("Finally, you are here. I am the true Dr. L");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("I have been waiting someone for years");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("10 years ago, I loved teaching and there were so many talents who learned from me");
		printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("However, with the salary of programmers rising rapidly");
		printf(" (press p to continue)\n");
		while (getch_l4() != 'p'){
		}
		move_l4(12,0);
		printf("\033[K");
		move_l4(13,0);
		printf("\033[K");
		move_l4(12,0);
		printf("They just come here to get my famous code template");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("They even don't want to learn about the algorithm behind");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Just want to get a well-paid job and have no interest in learning");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("I soon got disappointed and no longer wanted to teach");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("So I designed the tower of programmer and stayed here");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Anyone who is able to see me must be interested in programming");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("And more importantly, want to pay efforts into learning about it");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("I am so glad to see you here!");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("The tower is my first lesson for you, actually");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("You must learn from hard work and answer questions");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("But should never look for a free lunch");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    move_l4(12,0);
    printf("Come with me! Our journey just begins! Congratulation!");
    printf(" (press p to continue)\n");
    while (getch_l4() != 'p'){
    }
    move_l4(12,0);
    printf("\033[K");
    move_l4(13,0);
    printf("\033[K");
    GameOver = true;
    clear_l4();
    move_l4(0,0);
    printf("Producer: Kevin & Carrie\n");
    printf("Thank all teaching assitants and our two Dr. L for their teaching this semester\n");
    printf("This is the end of this game. Hope you will enjoy it!\n");
  }
}


void delete_sth_l4(int x, int y, char n){
	if(n == '@'){
		for(int i = 0; i < printable_num_fixed_door_l4; ++i){
			if (door_l4[i].x == x && door_l4[i].y == y){
				for(int j = i; j < printable_num_fixed_door_l4 - 1; ++j){
					door_l4[j].x = door_l4[j+1].x;
					door_l4[j].y = door_l4[j+1].y;
					door_l4[j].type = door_l4[j+1].type;
				}
			}
		}
		printable_num_fixed_door_l4--;
	}else if (n == 'M'){
		for(int i = 0; i < printable_num_fixed_monster_l4;++i){
			if(monster_l4[i].x == x && monster_l4[i].y == y){
				for(int j = i; j < printable_num_fixed_monster_l4 - 1; ++j){
					monster_l4[j].x = monster_l4[j+1].x;
					monster_l4[j].y = monster_l4[j+1].y;
				}
			}
		}
		printable_num_fixed_monster_l4--;
	}else if (n == 'm'){
		for(int i = 0; i < printable_num_random_monster_l4;++i){
			if(random_monster_l4[i].x == x && random_monster_l4[i].y == y){
				for(int j = i; j < printable_num_random_monster_l4 - 1; ++j){
					random_monster_l4[j].x = random_monster_l4[j+1].x;
					random_monster_l4[j].y = random_monster_l4[j+1].y;
				}
			}
		}
		printable_num_random_monster_l4--;
	}else if(n == '$'){
		for(int i = 0; i < printable_num_special_door_l4;++i){
			if(special_door_l4[i].x == x && special_door_l4[i].y == y){
				for(int j = i; j < printable_num_special_door_l4 - 1; ++j){
					special_door_l4[j].x = special_door_l4[j+1].x;
					special_door_l4[j].y = special_door_l4[j+1].y;
				}
			}
		}
		printable_num_special_door_l4--;
	}else if(n == '?'){
		for(int i = 0; i < printable_num_guidance_l4; ++i){
			if(guidance_l4[i].x == x && guidance_l4[i].y == y){
				for(int j = i; j < printable_num_guidance_l4 - 1; ++j){
					guidance_l4[j].x = guidance_l4[j+1].x;
					guidance_l4[j].y = guidance_l4[j+1].y;
				}
			}
		}
		printable_num_guidance_l4--;
	}
}
//function: calculate hp_needed to beat monster '0'
int hp_needed_beat_0_l4(int role_ATK, int role_DEF){
	int hp_needed_beat_0_l4;
	//when role_ATK <= rival_DEF, it means we can not attack the rival
	if(role_ATK <= random_monster_l4[0].DEF){
		hp_needed_beat_0_l4 = -1;
	//when role_ATK > rival_DEF, attack results in decreases in hp
	//if rival_ATK <= role_ATK, their attack can not hurt me
}else if (random_monster_l4[0].ATK <= role_DEF && role_ATK > random_monster_l4[0].DEF){
		hp_needed_beat_0_l4 = 0;
	}else if ((random_monster_l4[0].HP % (role_ATK-random_monster_l4[0].DEF)) == 0 && role_ATK > random_monster_l4[0].DEF){
		hp_needed_beat_0_l4 = (random_monster_l4[0].ATK-role_DEF)*((random_monster_l4[0].HP/(role_ATK-random_monster_l4[0].DEF))-1);
	}else if ((random_monster_l4[0].HP % (role_ATK-random_monster_l4[0].DEF)) != 0 && role_ATK > random_monster_l4[0].DEF){
		hp_needed_beat_0_l4 = (random_monster_l4[0].ATK-role_DEF)*(random_monster_l4[0].HP/(role_ATK-random_monster_l4[0].DEF));
	}
	return hp_needed_beat_0_l4;
}

//function: calculate hp_needed to beat monster 'M'
int hp_needed_beat_M_l4(int role_ATK, int role_DEF){
	int hp_needed_beat_M_l4;
	if(role_ATK <= monster_l4[0].DEF){
		hp_needed_beat_M_l4 = -1;
	}else if (monster_l4[0].ATK <= role_DEF && role_ATK > monster_l4[0].DEF){
		hp_needed_beat_M_l4 = 0;
	}else if (monster_l4[0].HP % (role_ATK-monster_l4[0].DEF) == 0 && role_ATK > monster_l4[0].DEF){
		hp_needed_beat_M_l4 = (monster_l4[0].ATK-role_DEF)*((monster_l4[0].HP/(role_ATK-monster_l4[0].DEF))-1);
	}else if (monster_l4[0].HP % (role_ATK-monster_l4[0].DEF) != 0 && role_ATK > monster_l4[0].DEF){
		hp_needed_beat_M_l4 = (monster_l4[0].ATK-role_DEF)*(monster_l4[0].HP/(role_ATK-monster_l4[0].DEF));
	}
	return hp_needed_beat_M_l4;
}

//print the map
void Draw_l4(int* role_attribute){
	clear_l4();
  move_l4(0,0);
	//print Floor 1 map wall_l4s
	for(int i = 0; i <= 21; ++i){
		if(i != 1){
      wall_l4[0][i] = true;
			printf("#");
		}else{
			printf(" ");
		}
  }
	printf("\n");

	for(int i = 1; i <= 11; ++i){
		for (int j = 0; j <= 21; ++j){
			if ((j == 0) || (j ==21) || (i == 11)
			|| (i == 2 && (j!= 1 &&j !=20))
			|| (i == 3 && j==2)
			|| (i == 4 && j!=1)
			|| (i == 6 && j != 1)
			|| (i == 7 && j == 2)
			|| (i == 8 && (j!=1 && j!=20)))
			{
        wall_l4[i][j] = true;
        printf("#");
      }else{
        printf(" ");
      }
		}
    printf("\n");
  }

	//print existing fixed monsters
	for(int k = 0; k < printable_num_fixed_monster_l4; ++k){
		move_l4(monster_l4[k].x, monster_l4[k].y);
		printf("M");
	}
	//print existing doors
	for(int k = 0; k < printable_num_fixed_door_l4; ++k){
		move_l4(door_l4[k].x, door_l4[k].y);
		printf("%c", door_l4[k].type);
	}

	//print exsiting random monsters
	for(int k = 0; k < printable_num_random_monster_l4; ++k){
		move_l4(random_monster_l4[k].x, random_monster_l4[k].y);
		printf("m");
	}
	//print special door
	for(int k = 0; k < printable_num_special_door_l4; ++k){
		move_l4(special_door_l4[k].x, special_door_l4[k].y);
		printf("$");
	}

  //print guidance
	for(int k = 0; k < printable_num_guidance_l4; ++k){
		move_l4(guidance_l4[k].x, guidance_l4[k].y);
		printf("?");
	}


	//print role position
	move_l4(x_l4,y_l4);
	printf("c");

	//print role_attribute
	move_l4(0,25);
	printf("Floor: %d", role_attribute[0]);
	move_l4(2,25);
	printf("Level: %d", role_attribute[1]);
	move_l4(3,25);
	printf("EXP: %d", role_attribute[5]);
	move_l4(4,25);
	printf("HP: %d", role_attribute[2]);
	move_l4(5,25);
	printf("ATK: %d",role_attribute[3]);
	move_l4(6,25);
	printf("DEF: %d",role_attribute[4]);
	move_l4(8,25);
	printf("GOLD: %d",role_attribute[6]);
	move_l4(9,25);
	printf("KEY(door): ^(@) %d",role_attribute[7]);
	move_l4(10,25);
	printf("           &($) %d",role_attribute[8]);
	move_l4(0,40);
	printf("'0' to save the game.");
	move_l4(1,40);
	printf("'q' to quit the game.");
	move_l4(12,0);
}


void show_info_l4(int* role_attribute){
	int hp_0 = hp_needed_beat_0_l4(role_attribute[3], role_attribute[4]);
	int hp_M = hp_needed_beat_M_l4(role_attribute[3], role_attribute[4]);
	//print all information of monsters in Floor 2
	//if we can not attack the monster
	//the attack can not hurt them or hp_needed_kill_them > role_HP
	//just show ???
	move_l4(12,0);
	printf("Here is some information about rivals in Floor %d:\n", role_attribute[0]);
	if (hp_0 < 0){
		printf("random monster 'm': HP:  %d ATK: %d DEF:  %d HP_needed: ???\n", random_monster_l4[0].HP, random_monster_l4[0].ATK, random_monster_l4[0].DEF);
	}else{
		printf("random monster 'm': HP:  %d ATK: %d DEF:  %d HP_needed: %d\n", random_monster_l4[0].HP, random_monster_l4[0].ATK, random_monster_l4[0].DEF, hp_0);
	}
	if (hp_M < 0){
		printf("fixed monster 'M': HP:  %d ATK: %d DEF:  %d HP_needed: ???\n", monster_l4[0].HP, monster_l4[0].ATK, monster_l4[0].DEF);
	}else{
		printf("fixed monster 'M': HP:  %d ATK: %d DEF:  %d HP_needed: %d\n", monster_l4[0].HP, monster_l4[0].ATK, monster_l4[0].DEF, hp_M);
	}
	printf("'?' is mysterious, try to talk to them.\n");

}

//use getch_l4() to read user input_l4 prumptly
void input_l4(int* role_attribute, std::string user_name, bool &GameOver){
	if(Is_Info_l4 == true){
		show_info_l4(role_attribute);
		Is_Info_l4 = false;
	}else if(save_l4 == true){
		saving_status_l4(role_attribute,user_name, GameOver);
		dir_l4 = STOP;
		return;
	}else if(Is_guidance_l4 == true){
    guide (x_l4,y_l4,user_name,role_attribute, GameOver);
    Is_guidance_l4 = false;
  } else if (Is_m_l4 == true){
    if (mtest_l4() == true){
      int a,b,c;
      a = monster_l4[0].ATK;
      b = monster_l4[0].DEF;
      c = monster_l4[0].HP;
      monster_l4[0].ATK = monster_l4[0].ATK / 2;
      monster_l4[0].DEF = monster_l4[0].DEF / 2;
      monster_l4[0].HP = monster_l4[0].HP / 2;
      int hp_M = hp_needed_beat_M_l4(role_attribute[3], role_attribute[4]);
      if (hp_M >= 0 && hp_M < role_attribute[2]){
        update_attribute_l4(50,12,5,9,10, role_attribute);
        delete_sth_l4(m_x_l4,m_y_l4,'M');
      }
      monster_l4[0].ATK = a;
      monster_l4[0].DEF = b;
      monster_l4[0].HP = c;
    } else {
      int hp_M = hp_needed_beat_M_l4(role_attribute[3], role_attribute[4]);
      if (hp_M >= 0 && hp_M < role_attribute[2]){
        update_attribute_l4(100,25,10,9,10, role_attribute);
      }
      delete_sth_l4(m_x_l4,m_y_l4,'M');
    }
    Is_m_l4 = false;
  }

	//user input_l4
	switch(getch_l4()){
		case 'w':
			dir_l4 = UP;
			break;
		case 's':
			dir_l4= DOWN;
			break;
		case 'a':
			dir_l4 = LEFT;
			break;
		case 'd':
			dir_l4 = RIGHT;
			break;
		case 'q':
			GameOver = true;
			dir_l4 = STOP;
			break;
		case '0':
		  save_l4 = true;
		  break;
		case 'i':
			Is_Info_l4 = true;
			break;
		default:
		//it is like a loop
			input_l4(role_attribute, user_name, GameOver);
			break;
	}
}


void logic_function_1_l4(int x, int y, int &change, int b, bool is_down,bool is_up, int* role_attribute, std::string user_name, bool &GameOver){
	//if the cell is wall_l4, then not move
	if(mvinch_l4(x, y) == '#'){

	//if the cell is the door to floor_3, then go to floor_3
}else if(x ==0 && y == 1 && is_up == true){
  role_attribute[0]--;
  floor_3_main(role_attribute,user_name, GameOver);

	//it is a door
}else if(mvinch_l4(x, y) == '@'){
		if (role_attribute[7] != 0){
			delete_sth_l4(x,y,'@');
			role_attribute[7]--;
		}
//it is a special door
}else if(mvinch_l4(x, y) == '$'){
  		if (role_attribute[8] != 0){
  			delete_sth_l4(x,y,'$');
  			role_attribute[8]--;
  		}
	//it is a guidance
}else if(mvinch_l4(x, y) == '?'){
    Is_guidance_l4 = true;
    delete_sth_l4(x,y,'?');
    change = change + b;
	//it is a fixed position monster
	//calculate the hp_needed_beat_M_l4, if we can kill it, then we can attack it
	//otherwise, we stay
}else if(mvinch_l4(x, y) == 'M'){
  m_x_l4 = x;
  m_y_l4 = y;
  Is_m_l4 = true;
	//it is a random_generated_monster, if we can kill it, then we can attack it
	//otherwise, we stay
}else if(mvinch_l4(x, y) == 'm'){
		int hp_0 = hp_needed_beat_0_l4(role_attribute[3], role_attribute[4]);
		if (hp_0 >= 0 && hp_0 < role_attribute[2]){
			update_attribute_l4(50,15,5,3,5,role_attribute);
			delete_sth_l4(x,y,'m');
		}else{
			x_l4 = x_l4;
		}

	}else{
    change = change + b;
  }
}

//move up and down to generate different reactions
void logic_l4(int* role_attribute, std::string user_name, bool &GameOver){
	switch (dir_l4) {
		case UP:
		logic_function_1_l4(x_l4-1,y_l4,x_l4,-1,false, true, role_attribute, user_name, GameOver);
			break;
		case DOWN:
		logic_function_1_l4(x_l4+1,y_l4,x_l4,1,true, false, role_attribute, user_name,GameOver);
			break;
		case LEFT:
			logic_function_1_l4(x_l4,y_l4-1,y_l4,-1, false, false, role_attribute, user_name,GameOver);
			break;
		case RIGHT:
			logic_function_1_l4(x_l4,y_l4+1,y_l4,1, false, false, role_attribute, user_name,GameOver);
			break;
		default:
			break;
	}
  dir_l4 = STOP;
}

void floor_4_main(int *role_attribute, std::string user_name, bool &GameOver){
  srand (time(NULL));
	if(GameOver == false){
		for (int i=0;i<=11;i++){
	    for (int j=0;j<=21;j++ ){
	      wall_l4[i][j] =false;
	    }
	  }
		//start a loop
		if (count_Setup_l4_l4 == 0){
			Setup_l4();
			count_Setup_l4_l4++;
		}
		while(!GameOver){
			Draw_l4(role_attribute);
			input_l4(role_attribute, user_name, GameOver);
			logic_l4(role_attribute, user_name, GameOver);
		}
	}
	delete [] monster_l4;
	delete [] door_l4;
  delete [] special_door_l4;
	delete [] random_monster_l4;
	delete [] guidance_l4;
}
