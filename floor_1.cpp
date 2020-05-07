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
#: wall_l1, space between wall_l1s is the door connecting different floor
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
const int width_l1 = 20, height_l1 = 10, num_fixed_monster_l1 = 6, num_fixed_door_l1 = 4, num_random_monster_l1= 10,
num_fixed_HP_box_l1 = 3, num_fixed_ATK_box_l1 = 3, num_fixed_DEF_box_l1 = 3, num_fixed_key_l1 = 7, num_store_l1 = 1, num_guidance_l1 = 3;
int printable_num_fixed_door_l1 = 4, printable_num_fixed_monster_l1 = 6, printable_num_random_monster_l1 = 10,
printable_num_fixed_HP_box_l1 = 3, printable_num_fixed_ATK_box_l1 = 3, printable_num_fixed_DEF_box_l1 = 3,
printable_num_store_l1 = 1, printable_num_fixed_key_l1 = 7, printable_num_guidance_l1 = 3,m_x,m_y;

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
Fixed_sth_coordinate * monster_l1 = new Fixed_sth_coordinate [num_fixed_monster_l1];
Fixed_sth_coordinate * door_l1 = new Fixed_sth_coordinate [num_fixed_door_l1];
Fixed_sth_coordinate * key_l1 = new Fixed_sth_coordinate [num_fixed_key_l1];
Fixed_sth_coordinate * random_monster_l1 = new Fixed_sth_coordinate [num_random_monster_l1];
Fixed_sth_coordinate * HP_box_l1 = new Fixed_sth_coordinate[num_fixed_HP_box_l1];
Fixed_sth_coordinate * ATK_box_l1 = new Fixed_sth_coordinate[num_fixed_ATK_box_l1];
Fixed_sth_coordinate * DEF_box_l1 = new Fixed_sth_coordinate[num_fixed_DEF_box_l1];
Fixed_sth_coordinate * store_l1 = new Fixed_sth_coordinate[num_store_l1];
Fixed_sth_coordinate * guidance_l1 = new Fixed_sth_coordinate[num_guidance_l1];

int x_l1, y_l1;
int small_monster_x_l1[10], small_monster_y_l1[10];

int count_Setup_l1_l1 = 0;
bool save_l1, IsH_l1, IsA_l1, IsD_l1, Is_Sur_l1, Is_Info_l1, Is_guidance_l1, Is_m,Is_s= false;
bool wall_l1[12][22];
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir_l1;

// move the cursor
void move_l1(int x,int y){
  printf("\033[%d;%dH", (x+1), (y+1));
}
// clear the screen
void clear_l1(){
  printf("\033[2J");
}
// return the symbol
int getch_l1(void)
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
char mvinch_l1(int x, int y){
  if (wall_l1[x][y] == true){
    return ('#');
  }
  for(int k = 0; k < printable_num_fixed_monster_l1; ++k){
    if (monster_l1[k].x == x && monster_l1[k].y == y){
      return ('M');
    }
	}
	//print existing doors
	for(int k = 0; k < printable_num_fixed_door_l1; ++k){
		if(door_l1[k].x == x &&  door_l1[k].y== y){
      return ('@');
    }
	}

	//print exsiting random monsters
	for(int k = 0; k < printable_num_random_monster_l1; ++k){
		if(random_monster_l1[k].x == x && random_monster_l1[k].y == y){
      return ('m');
    }
	}
	//print existing HP_box
	for(int k = 0; k < printable_num_fixed_HP_box_l1; ++k){
		if(HP_box_l1[k].x ==x &&  HP_box_l1[k].y == y){
      return ('H');
    }
	}

	//print existing ATK_box
	for(int k = 0; k < printable_num_fixed_ATK_box_l1; ++k){
		if(ATK_box_l1[k].x ==x &&ATK_box_l1[k].y==y){
      return ('A');
    }
	}

	//print existing DEF_box
	for(int k = 0; k < printable_num_fixed_DEF_box_l1; ++k){
		if(DEF_box_l1[k].x ==x &&DEF_box_l1[k].y==y){
      return ('D');
    }
	}

	//print existing keys
	for(int k = 0; k < printable_num_fixed_key_l1; ++k){
		if(key_l1[k].x ==x &&key_l1[k].y == y){
      return ('^');
    }
	}

  //print guidance
	for(int k = 0; k < printable_num_guidance_l1; ++k){
		if(guidance_l1[k].x==x &&guidance_l1[k].y==y){
      return ('?');
    }
	}

  //print store
	for(int k = 0; k < printable_num_store_l1; ++k){
		if(store_l1[k].x==x &&store_l1[k].y==y){
      return ('S');
    }
	}
  return (' ');
}
// save the game
void saving_status_l1(int * role_attribute,string user_name, bool &GameOver){
	move_l1(12,0);
	string filename = "output/"+user_name + ".txt";
	ofstream fout;
	fout.open(filename.c_str());
	for(int i = 0; i < 9; i++){
		fout << role_attribute[i]<<" ";
	}
	fout << role_attribute[9]<< std::endl;
	fout.close();
	printf("game status already saved!\ngame over...");
	GameOver = true;
}
//Setup_l1 all positions in this floor
void Setup_l1(){
	dir_l1 = STOP;
	x_l1 = 1;
	y_l1 = 1;
	small_monster_x_l1[0] = 7 + rand()%2;
  small_monster_y_l1[0] = 4 + rand()%2;
  small_monster_x_l1[1] = 7 + rand()%2;
  small_monster_y_l1[1] = 6 + rand()%2;
  small_monster_x_l1[2] = 3 + rand()%4;
  small_monster_y_l1[2] = 9 + rand()%2;
  small_monster_x_l1[3] = 8 + rand()%3;
  small_monster_y_l1[3] = 9 + rand()%2;
  small_monster_x_l1[4] = 8 + rand()%3;
  small_monster_y_l1[4] = 12;
  small_monster_x_l1[5] = 8 + rand()%3;
  small_monster_y_l1[5] = 13 + rand()%2;
  small_monster_x_l1[6] = 8 + rand()%3;
  small_monster_y_l1[6] = 15 + rand()%2;
  small_monster_x_l1[7] = 7 + rand()%3;
  small_monster_y_l1[7] = 18;
  small_monster_x_l1[8] = 7 + rand()%3;
  small_monster_y_l1[8] = 20;
  small_monster_x_l1[9] = 4;
  small_monster_y_l1[9] = 1;


	//store fixed monsters into array
	//store them into struct
	int monster_x_l1[num_fixed_monster_l1] = {8,3,5,5,5,5};
	int monster_y_l1[num_fixed_monster_l1] = {1,4,3,12,16,19};
	for(int i = 0; i < num_fixed_monster_l1; i++){
		monster_l1[i].x = monster_x_l1[i];
		monster_l1[i].y = monster_y_l1[i];
		monster_l1[i].HP = 100;
		monster_l1[i].ATK = 25;
		monster_l1[i].DEF = 10;
    monster_l1[i].type = 'M';
	}

	//store fixed doors into array
	//store them into struct
	int door_x_l1[num_fixed_door_l1] = {4,6,5,10};
	int door_y_l1[num_fixed_door_l1] = {3,3,8,17};
	for(int i = 0; i < num_fixed_door_l1; i++){
		door_l1[i].x = door_x_l1[i];
		door_l1[i].y = door_y_l1[i];
		door_l1[i].type = '@';
	}

	//store random monsters into array
	//store them into struct
	for(int i = 0; i < num_random_monster_l1; i++){
		random_monster_l1[i].x = small_monster_x_l1[i];
		random_monster_l1[i].y = small_monster_y_l1[i];
		random_monster_l1[i].HP = 50;
		random_monster_l1[i].ATK = 15;
		random_monster_l1[i].DEF = 5;
    random_monster_l1[i].type = 'm';
	}

	//store fixed HP_box into array
	int HP_box_x_l1[num_fixed_HP_box_l1] = {1,2,3};
	int HP_box_y_l1[num_fixed_HP_box_l1] = {7,7,18};
	for(int i = 0; i < num_fixed_HP_box_l1; i++){
		HP_box_l1[i].x = HP_box_x_l1[i];
		HP_box_l1[i].y = HP_box_y_l1[i];
    HP_box_l1[i].type = 'H';
	}

	//store fixed ATK_box into array
	//store them into struct
	int ATK_box_x_l1[num_fixed_HP_box_l1] = {1,4,4};
	int ATK_box_y_l1[num_fixed_HP_box_l1] = {6,12,20};
	for(int i = 0; i < num_fixed_ATK_box_l1; i++){
		ATK_box_l1[i].x = ATK_box_x_l1[i];
		ATK_box_l1[i].y = ATK_box_y_l1[i];
    ATK_box_l1[i].type = 'A';
	}
	//store fixed DEF_box into array
	//store them into struct
	int DEF_box_x_l1[num_fixed_DEF_box_l1] = {2,4,4};
	int DEF_box_y_l1[num_fixed_DEF_box_l1] = {6,16,18};
	for(int i = 0; i < num_fixed_DEF_box_l1; i++){
		DEF_box_l1[i].x = DEF_box_x_l1[i];
		DEF_box_l1[i].y = DEF_box_y_l1[i];
    DEF_box_l1[i].type = 'D';
	}

	//store fixed keys into array
	//store them into struct
	int key_x_l1[num_fixed_key_l1] = {7,10,5,3,3,1,3};
	int key_y_l1[num_fixed_key_l1] = {3,4,5,7,12,12,16};
	for(int i = 0; i < num_fixed_key_l1; i++){
		key_l1[i].x = key_x_l1[i];
		key_l1[i].y = key_y_l1[i];
    key_l1[i].type = '^';
	}

  // save the location of store
  store_l1[0].x = 3;
  store_l1[0].y = 20;
  store_l1[0].type = 'S';

  //save the location of guidance
  int guidance_x_l1[num_guidance_l1] = {2,5,9};
  int guidance_y_l1[num_guidance_l1] = {1,1,1};
  for (int i=0;i<num_guidance_l1;i++){
    guidance_l1[i].x = guidance_x_l1[i];
    guidance_l1[i].y = guidance_y_l1[i];
    guidance_l1[i].type = '?';
  }

}
// level up
void upgrade_l1(int* role_attribute){
	//atk
	role_attribute[3] = role_attribute[3] + 5;
	//def
	role_attribute[4] = role_attribute[4] + 5;
	//HP
	role_attribute[2] = role_attribute[5] + 100;
}
// modify role_attribute
void update_attribute_l1(int hp_value, int atk_value, int def_value,
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
		upgrade_l1(role_attribute);
	}
}
// monster's test
bool mtest(){
  move_l1(12,0);
  int a = rand()%6;
  if (a == 0){
    move_l1(12,0);
    printf("We can name a directory lab 1 with the following command:");
    move_l1(13,0);
    printf("mkdir lab 1");
    move_l1(14,0);
    printf("T/F");
    char c = getch_l1();
    while (c != 't' && c != 'f'){
      c = getch_l1();
    }
    if (c == 't'){
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  if (a == 1){
    move_l1(12,0);
    printf("We can move a directory dir to dir2 using the following command:");
    move_l1(13,0);
    printf("mv dir dir2");
    move_l1(14,0);
    printf("T/F");
    char c = getch_l1();
    while (c != 't' && c != 'f'){
      c = getch_l1();
    }
    if (c == 't'){
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  if (a == 2){
    move_l1(12,0);
    printf("In the vi editor, ':w' will save the file:");
    move_l1(13,0);
    printf("T/F");
    char c = getch_l1();
    while (c != 't' && c != 'f'){
      c = getch_l1();
    }
    if (c == 'f'){
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  if (a == 3){
    move_l1(12,0);
    printf("chmod o+rx file means giving everyone the read and execute permission？");
    move_l1(13,0);
    printf("T/F");
    char c = getch_l1();
    while (c != 't' && c != 'f'){
      c = getch_l1();
    }
    if (c == 't'){
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  if (a == 4){
    move_l1(12,0);
    printf("In the shell script, [$a -lt $b] is true if a>b:");
    move_l1(13,0);
    printf("T/F");
    char c = getch_l1();
    while (c != 't' && c != 'f'){
      c = getch_l1();
    }
    if (c == 't'){
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  if (a == 5){
    move_l1(12,0);
    printf("In shell script, '$#' means the number of command line variables given？");
    move_l1(13,0);
    printf("T/F");
    char c = getch_l1();
    while (c != 't' && c != 'f'){
      c = getch_l1();
    }
    if (c == 'f'){
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("No....That seems wrong.");
      return false;
    }else{
      move_l1(12,0);
      printf("\033[K");
      move_l1(13,0);
      printf("\033[K");
      move_l1(14,0);
      printf("\033[K");
      move_l1(12,0);
      printf("It is weakened!");
      return true;
    }
  }
  return true;
}
// guide function to give 3 guidances at the beginning
void guide(int x, int y, std::string user_name){
  if (x == 2 && y == 1){
    move_l1(12,0);
    cout << "Hi " << user_name << ", Welcome to Tower of Programmer!";
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("This is the first floor of the tower. If you want to meet Dr.L, here's the place for you!");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("Dr.L is a great professor and all best programmers used to learn from him, but he only stays in this tower.");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("No one knows why there are so many monsters (m/M) here. You need to use your wisdom to defeat them.");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("The right-hand side is your attributes. Try to find 'H'/'A'/'D' to increase your power.");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("Also, you will gain experience and gold by defeating monsters. You can go to store(S) to buy your power.");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("Some doors (@/$) are locked. You need to use keys (^/?) to open them.");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("Go ahead, young man! Try to find Dr.L and hope you can learn programming in this journey.");
  }
  if (x==5 && y==1){
    printf("Congratulation! You just defeat the first monster.");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("You can see that your HP is decreasing and you can press 'i' later to check how much HP required to defeat a monster.");
  }
  if (x==9 && y==1){
    printf("Did you see that you are required to answer questions?");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("That's because some monsters are so close to Dr.L that they also learn some programming skills.");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("If you can answer questions correctly, monsters will be weakened.");
		printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("In the second floor, if you answer M's question correctly, it will surrender.");
		printf(" (press p to continue)\n");
		while (getch_l1() != 'p'){
		}
		move_l1(12,0);
		printf("\033[K");
		move_l1(13,0);
		printf("\033[K");
		move_l1(12,0);
		printf("press 0 to save the game; press q to quit the game;");
    printf(" (press p to continue)\n");
    while (getch_l1() != 'p'){
    }
    move_l1(12,0);
    printf("\033[K");
    move_l1(13,0);
    printf("\033[K");
    move_l1(12,0);
    printf("Go ahead and try to find Dr.L!");
		printf(" (press p to continue)\n");
  }
}

// eliminate symbol
void delete_sth_l1(int x, int y, char n){
	if(n == '@'){
		for(int i = 0; i < printable_num_fixed_door_l1; ++i){
			if (door_l1[i].x == x && door_l1[i].y == y){
				for(int j = i; j < printable_num_fixed_door_l1 - 1; ++j){
					door_l1[j].x = door_l1[j+1].x;
					door_l1[j].y = door_l1[j+1].y;
					door_l1[j].type = door_l1[j+1].type;
				}
			}
		}
		printable_num_fixed_door_l1--;
	}else if (n == 'M'){
		for(int i = 0; i < printable_num_fixed_monster_l1;++i){
			if(monster_l1[i].x == x && monster_l1[i].y == y){
				for(int j = i; j < printable_num_fixed_monster_l1 - 1; ++j){
					monster_l1[j].x = monster_l1[j+1].x;
					monster_l1[j].y = monster_l1[j+1].y;
				}
			}
		}
		printable_num_fixed_monster_l1--;
	}else if (n == 'm'){
		for(int i = 0; i < printable_num_random_monster_l1;++i){
			if(random_monster_l1[i].x == x && random_monster_l1[i].y == y){
				for(int j = i; j < printable_num_random_monster_l1 - 1; ++j){
					random_monster_l1[j].x = random_monster_l1[j+1].x;
					random_monster_l1[j].y = random_monster_l1[j+1].y;
				}
			}
		}
		printable_num_random_monster_l1--;
	}else if(n == 'H'){
		for(int i = 0; i < printable_num_fixed_HP_box_l1;++i){
			if(HP_box_l1[i].x == x && HP_box_l1[i].y == y){
				for(int j = i; j < printable_num_fixed_HP_box_l1 - 1; ++j){
					HP_box_l1[j].x = HP_box_l1[j+1].x;
					HP_box_l1[j].y = HP_box_l1[j+1].y;
				}
			}
		}
		printable_num_fixed_HP_box_l1--;
	}else if(n == 'A'){
		for(int i = 0; i < printable_num_fixed_ATK_box_l1; ++i){
			if(ATK_box_l1[i].x == x && ATK_box_l1[i].y == y){
				for(int j = i; j < printable_num_fixed_ATK_box_l1 - 1; ++j){
					ATK_box_l1[j].x = ATK_box_l1[j+1].x;
					ATK_box_l1[j].y = ATK_box_l1[j+1].y;
				}
			}
		}
		printable_num_fixed_ATK_box_l1--;
	}else if(n == 'D'){
		for(int i = 0; i < printable_num_fixed_DEF_box_l1; ++i){
			if(DEF_box_l1[i].x == x && DEF_box_l1[i].y == y){
				for(int j = i; j < printable_num_fixed_DEF_box_l1 - 1; ++j){
					DEF_box_l1[j].x = DEF_box_l1[j+1].x;
					DEF_box_l1[j].y = DEF_box_l1[j+1].y;
				}
			}
		}
		printable_num_fixed_DEF_box_l1--;
	}else if(n == '?'){
		for(int i = 0; i < printable_num_guidance_l1; ++i){
			if(guidance_l1[i].x == x && guidance_l1[i].y == y){
				for(int j = i; j < printable_num_guidance_l1 - 1; ++j){
					guidance_l1[j].x = guidance_l1[j+1].x;
					guidance_l1[j].y = guidance_l1[j+1].y;
				}
			}
		}
		printable_num_guidance_l1--;
	}else if(n == '^'){
		for(int i = 0; i < printable_num_fixed_key_l1; ++i){
			if(key_l1[i].x == x && key_l1[i].y == y){
				for(int j = i; j < printable_num_fixed_key_l1 - 1; ++j){
					key_l1[j].x = key_l1[j+1].x;
					key_l1[j].y = key_l1[j+1].y;
				}
			}
		}
		printable_num_fixed_key_l1--;
	}
}
//function: calculate hp_needed to beat monster '0'
int hp_needed_beat_0_l1(int role_ATK, int role_DEF){
	int hp_needed_beat_0_l1;
	//when role_ATK <= rival_DEF, it means we can not attack the rival
	if(role_ATK <= random_monster_l1[0].DEF){
		hp_needed_beat_0_l1 = -1;
	//when role_ATK > rival_DEF, attack results in decreases in hp
	//if rival_ATK <= role_ATK, their attack can not hurt me
}else if (random_monster_l1[0].ATK <= role_DEF && role_ATK > random_monster_l1[0].DEF){
		hp_needed_beat_0_l1 = 0;
	}else if ((random_monster_l1[0].HP % (role_ATK-random_monster_l1[0].DEF)) == 0 && role_ATK > random_monster_l1[0].DEF){
		hp_needed_beat_0_l1 = (random_monster_l1[0].ATK-role_DEF)*((random_monster_l1[0].HP/(role_ATK-random_monster_l1[0].DEF))-1);
	}else if ((random_monster_l1[0].HP % (role_ATK-random_monster_l1[0].DEF)) != 0 && role_ATK > random_monster_l1[0].DEF){
		hp_needed_beat_0_l1 = (random_monster_l1[0].ATK-role_DEF)*(random_monster_l1[0].HP/(role_ATK-random_monster_l1[0].DEF));
	}
	return hp_needed_beat_0_l1;
}

//function: calculate hp_needed to beat monster 'M'
int hp_needed_beat_M_l1(int role_ATK, int role_DEF){
	int hp_needed_beat_M_l1;
	if(role_ATK <= monster_l1[0].DEF){
		hp_needed_beat_M_l1 = -1;
	}else if (monster_l1[0].ATK <= role_DEF && role_ATK > monster_l1[0].DEF){
		hp_needed_beat_M_l1 = 0;
	}else if (monster_l1[0].HP % (role_ATK-monster_l1[0].DEF) == 0 && role_ATK > monster_l1[0].DEF){
		hp_needed_beat_M_l1 = (monster_l1[0].ATK-role_DEF)*((monster_l1[0].HP/(role_ATK-monster_l1[0].DEF))-1);
	}else if (monster_l1[0].HP % (role_ATK-monster_l1[0].DEF) != 0 && role_ATK > monster_l1[0].DEF){
		hp_needed_beat_M_l1 = (monster_l1[0].ATK-role_DEF)*(monster_l1[0].HP/(role_ATK-monster_l1[0].DEF));
	}
	return hp_needed_beat_M_l1;
}

//print the map
void Draw_l1(int* role_attribute){
	clear_l1();
  move_l1(0,0);
	//print Floor 1 map wall_l1s
	for(int i = 0; i <= 21; ++i){
		if(i != 1 && i != 20){
      wall_l1[0][i] = true;
			printf("#");
		}else{
			printf(" ");
		}
  }
	printf("\n");

	for(int i = 1; i <= 11; ++i){
		for (int j = 0; j <= 21; ++j){
			if ((j == 0) || (j ==21) || (i == 11)
			|| (j == 2 && i!= 10)
			|| (j == 3 && (i==9 || i==11))
			|| (j == 4 && (i==1 || i==2 || i==4 || i==6 || i==9))
			|| (j == 5 && (i==4 || i==6 || i==9))
			|| (j == 6 && (i == 4 || i==6 || i==9))
			|| (j == 7 && (i==4 || i==6 || i==9))
			|| (j == 8 && (i!=5 && i!=10))
			|| (j == 11 && i!=1 )
      || (j == 12 && (i==2 || i==7))
      || (j == 13 && (i!=1 && i!=6 && i!=8 && i!=9 && i!=10))
      || (j == 14 && i ==7)
      || (j == 15 && (i==2 || i==3 || i==4 || i==5 || i==7))
      || (j == 16 && i ==2)
      || (j == 17 && (i !=1 && i!=10))
      || (j == 18 && i ==2)
      || (j == 19 && i ==2)
      || (j == 20 && i ==2)) {
        wall_l1[i][j] = true;
        printf("#");
      }else{
        printf(" ");
      }
		}
    printf("\n");
  }

	//print existing fixed monsters
	for(int k = 0; k < printable_num_fixed_monster_l1; ++k){
		move_l1(monster_l1[k].x, monster_l1[k].y);
		printf("M");
	}
	//print existing doors
	for(int k = 0; k < printable_num_fixed_door_l1; ++k){
		move_l1(door_l1[k].x, door_l1[k].y);
		printf("%c", door_l1[k].type);
	}

	//print exsiting random monsters
	for(int k = 0; k < printable_num_random_monster_l1; ++k){
		move_l1(random_monster_l1[k].x, random_monster_l1[k].y);
		printf("m");
	}
	//print existing HP_box
	for(int k = 0; k < printable_num_fixed_HP_box_l1; ++k){
		move_l1(HP_box_l1[k].x, HP_box_l1[k].y);
		printf("H");
	}

	//print existing ATK_box
	for(int k = 0; k < printable_num_fixed_ATK_box_l1; ++k){
		move_l1(ATK_box_l1[k].x, ATK_box_l1[k].y);
		printf("A");
	}

	//print existing DEF_box
	for(int k = 0; k < printable_num_fixed_DEF_box_l1; ++k){
		move_l1(DEF_box_l1[k].x, DEF_box_l1[k].y);
		printf("D");
	}

	//print existing keys
	for(int k = 0; k < printable_num_fixed_key_l1; ++k){
		move_l1(key_l1[k].x, key_l1[k].y);
		printf("^");
	}

  //print guidance
	for(int k = 0; k < printable_num_guidance_l1; ++k){
		move_l1(guidance_l1[k].x, guidance_l1[k].y);
		printf("?");
	}

  //print store
	for(int k = 0; k < printable_num_store_l1; ++k){
		move_l1(store_l1[k].x, store_l1[k].y);
		printf("S");
	}


	//print role position
	move_l1(x_l1,y_l1);
	printf("c");

	//print role_attribute
	move_l1(0,25);
	printf("Floor: %d", role_attribute[0]);
	move_l1(2,25);
	printf("Level: %d", role_attribute[1]);
	move_l1(3,25);
	printf("EXP: %d", role_attribute[5]);
	move_l1(4,25);
	printf("HP: %d", role_attribute[2]);
	move_l1(5,25);
	printf("ATK: %d",role_attribute[3]);
	move_l1(6,25);
	printf("DEF: %d",role_attribute[4]);
	move_l1(8,25);
	printf("GOLD: %d",role_attribute[6]);
	move_l1(9,25);
	printf("KEY(door): ^(@) %d",role_attribute[7]);
	move_l1(10,25);
	printf("           &($) %d",role_attribute[8]);
	move_l1(0,40);
	printf("'0' to save the game.");
	move_l1(1,40);
	printf("'q' to quit the game.");
	move_l1(12,0);
}

void print_prumpt_l1(char n, int * role_attribute){
	if (n == 'H'){
		move_l1(12,0);
		printf("HP +100!");
	}else if(n == 'A'){
		move_l1(12,0);
		printf("ATK +5!");
	}else if(n == 'D'){
		move_l1(12,0);
		printf("DEF +5!");
	}
}
// show some guidance information
void show_info_l1(int* role_attribute){
	int hp_0 = hp_needed_beat_0_l1(role_attribute[3], role_attribute[4]);
	int hp_M = hp_needed_beat_M_l1(role_attribute[3], role_attribute[4]);
	//print all information of monsters in Floor 2
	//if we can not attack the monster
	//the attack can not hurt them or hp_needed_kill_them > role_HP
	//just show ???
	move_l1(12,0);
	printf("Here is some information about rivals in Floor %d:\n", role_attribute[0]);
	if (hp_0 < 0){
		printf("random monster 'm': HP:  %d ATK: %d DEF:  %d HP_needed: ???\n", random_monster_l1[0].HP, random_monster_l1[0].ATK, random_monster_l1[0].DEF);
	}else{
		printf("random monster 'm': HP:  %d ATK: %d DEF:  %d HP_needed: %d\n", random_monster_l1[0].HP, random_monster_l1[0].ATK, random_monster_l1[0].DEF, hp_0);
	}
	if (hp_M < 0){
		printf("fixed monster 'M': HP:  %d ATK: %d DEF:  %d HP_needed: ???\n", monster_l1[0].HP, monster_l1[0].ATK, monster_l1[0].DEF);
	}else{
		printf("fixed monster 'M': HP:  %d ATK: %d DEF:  %d HP_needed: %d\n", monster_l1[0].HP, monster_l1[0].ATK, monster_l1[0].DEF, hp_M);
	}
	printf("'!' represents treasures. They will help you.\n");
	printf("'0' to save the game.\n");
  printf("'q' to quit the game.\n");

}
// store shopping
void shopping(int *role_attribute){
  move_l1(12,0);
  printf("happy shopping!\n");
  printf(" (press p to continue)\n");
  while (getch_l1() != 'p'){
  }
  move_l1(12,0);
  printf("\033[K");
  move_l1(13,0);
  printf("\033[K");
  move_l1(12,0);
  printf("press t: 50 gold for 10 ATK!\n");
  printf("press y: 50 gold for 10 DEF!\n");
  printf("press u: 50 gold for 100 HP!\n");
  printf(" (press p to continue)\n");
  char c = getch_l1();
  while (c != 'p'){
    if (c == 't'){
      if (role_attribute[6]>=50){
        role_attribute[6] = role_attribute[6] - 50;
        role_attribute[3] = role_attribute[3] + 10;
        Draw_l1(role_attribute);
        move_l1(12,0);
        printf("press t: 50 gold for 10 ATK!\n");
        printf("press y: 50 gold for 10 DEF!\n");
        printf("press u: 50 gold for 100 HP!\n");
        printf(" (press p to continue)\n");
      }
    }
    if (c == 'y'){
      if (role_attribute[6]>=50){
        role_attribute[6] = role_attribute[6] - 50;
        role_attribute[4] = role_attribute[4] + 10;
        Draw_l1(role_attribute);
        move_l1(12,0);
        printf("press t: 50 gold for 10 ATK!\n");
        printf("press y: 50 gold for 10 DEF!\n");
        printf("press u: 50 gold for 100 HP!\n");
        printf(" (press p to continue)\n");
      }
    }
    if (c == 'u'){
      if (role_attribute[5]>=50){
        role_attribute[6] = role_attribute[6] - 50;
        role_attribute[2] = role_attribute[2] + 100;
        Draw_l1(role_attribute);
        move_l1(12,0);
        printf("press t: 50 gold for 10 ATK!\n");
        printf("press y: 50 gold for 10 DEF!\n");
        printf("press u: 50 gold for 100 HP!\n");
        printf(" (press p to continue)\n");
      }
    }
    c = getch_l1();
  }
  move_l1(12,0);
  printf("\033[K");
  move_l1(13,0);
  printf("\033[K");
  move_l1(14,0);
  printf("\033[K");
  move_l1(15,0);
  printf("\033[K");
}
//use getch_l1() to read user input_l1 prumptly
void input_l1(int* role_attribute, std::string user_name, bool &GameOver){
	if(IsH_l1 == true){
		print_prumpt_l1('H', role_attribute);
		IsH_l1 = false;
	}else if(IsA_l1 == true){
		print_prumpt_l1('A', role_attribute);
		IsA_l1 = false;
	}else if(IsD_l1 == true){
		print_prumpt_l1('D', role_attribute);
		IsD_l1 = false;
	}else if(Is_Info_l1 == true){
		show_info_l1(role_attribute);
		Is_Info_l1 = false;
	}else if(save_l1 == true){
		saving_status_l1(role_attribute,user_name, GameOver);
		dir_l1 = STOP;
		return;
	}else if(Is_guidance_l1 == true){
    guide (x_l1,y_l1,user_name);
    Is_guidance_l1 = false;
  } else if (Is_m == true){
    if (mtest() == true){
      int a,b,c;
      a = monster_l1[0].ATK;
      b = monster_l1[0].DEF;
      c = monster_l1[0].HP;
      monster_l1[0].ATK = monster_l1[0].ATK / 2;
      monster_l1[0].DEF = monster_l1[0].DEF / 2;
      monster_l1[0].HP = monster_l1[0].HP / 2;
      int hp_M = hp_needed_beat_M_l1(role_attribute[3], role_attribute[4]);
      if (hp_M >= 0 && hp_M < role_attribute[2]){
        update_attribute_l1(50,12,5,9,10, role_attribute);
        delete_sth_l1(m_x,m_y,'M');
      }
      monster_l1[0].ATK = a;
      monster_l1[0].DEF = b;
      monster_l1[0].HP = c;
    } else {
      int hp_M = hp_needed_beat_M_l1(role_attribute[3], role_attribute[4]);
      if (hp_M >= 0 && hp_M < role_attribute[2]){
        update_attribute_l1(100,25,10,9,10, role_attribute);
				delete_sth_l1(m_x,m_y,'M');
      }
    }
    Is_m = false;
  } else if (Is_s == true){
    shopping(role_attribute);
    Is_s = false;
  }

	//user input_l1
	switch(getch_l1()){
		case 'w':
			dir_l1 = UP;
			break;
		case 's':
			dir_l1= DOWN;
			break;
		case 'a':
			dir_l1 = LEFT;
			break;
		case 'd':
			dir_l1 = RIGHT;
			break;
		case 'q':
			GameOver = true;
			dir_l1 = STOP;
			break;
		case '0':
		  save_l1 = true;
		  break;
		case 'i':
			Is_Info_l1 = true;
			break;
		default:
		//it is like a loop
			input_l1(role_attribute, user_name, GameOver);
			break;
	}
}


void logic_function_1_l1(int x, int y, int &change, int b, bool is_down,bool is_up, int* role_attribute, std::string user_name, bool &GameOver){
	//if the cell is wall_l1, then not move
	if(mvinch_l1(x, y) == '#'){

	//if the cell is the door to floor_2, then go to floor_2
}else if(x == 0 && y == 20 && is_up == true){
	  role_attribute[0]++;
		floor_2_main(role_attribute,user_name, GameOver);

	//it is a door
}else if(mvinch_l1(x, y) == '@'){
		if (role_attribute[7] != 0){
			delete_sth_l1(x,y,'@');
			role_attribute[7]--;
		}
	//it is a special door
}else if(mvinch_l1(x, y) == '^'){
		role_attribute[7]++;
		delete_sth_l1(x,y,'^');
	//it is a HP_box, it will increase HP by 100
}else if(mvinch_l1(x, y) == 'H'){
		delete_sth_l1(x,y,'H');
		IsH_l1 = true;
		role_attribute[2] = role_attribute[2] + 100;
	//it is a STK_box, it will increase ATK by 5
}else if(mvinch_l1(x, y) == 'A'){
		delete_sth_l1(x,y,'A');
		IsA_l1 = true;
		role_attribute[3] = role_attribute[3] + 5;
	//it is a DEF_box, it will increase DEF by 5
}else if(mvinch_l1(x, y) == 'D'){
		delete_sth_l1(x,y,'D');
		IsD_l1 = true;
		role_attribute[4] = role_attribute[4] + 5;
	//it is a surprising_box, it will have sword or diamond
}else if(mvinch_l1(x, y) == '?'){
    Is_guidance_l1 = true;
    delete_sth_l1(x,y,'?');
    change = change + b;
	//it is a fixed position monster
	//calculate the hp_needed_beat_M_l1, if we can kill it, then we can attack it
	//otherwise, we stay
}else if(mvinch_l1(x, y) == 'M'){
  m_x = x;
  m_y = y;
  Is_m = true;
	//it is a random_generated_monster, if we can kill it, then we can attack it
	//otherwise, we stay
}else if(mvinch_l1(x, y) == 'm'){
		int hp_0 = hp_needed_beat_0_l1(role_attribute[3], role_attribute[4]);
		if (hp_0 >= 0 && hp_0 < role_attribute[2]){
			update_attribute_l1(50,15,5,3,5,role_attribute);
			delete_sth_l1(x,y,'m');
		}else{
			x_l1 = x_l1;
		}
	}else if (mvinch_l1(x,y) == 'S'){
		Is_s = true;


	}else{
    change = change + b;
  }
}

//move up and down to generate different reactions
void logic_l1(int* role_attribute, std::string user_name, bool &GameOver){
	switch (dir_l1) {
		case UP:
		logic_function_1_l1(x_l1-1,y_l1,x_l1,-1,false, true, role_attribute, user_name, GameOver);
			break;
		case DOWN:
		logic_function_1_l1(x_l1+1,y_l1,x_l1,1,true, false, role_attribute, user_name,GameOver);
			break;
		case LEFT:
			logic_function_1_l1(x_l1,y_l1-1,y_l1,-1, false, false, role_attribute, user_name,GameOver);
			break;
		case RIGHT:
			logic_function_1_l1(x_l1,y_l1+1,y_l1,1, false, false, role_attribute, user_name,GameOver);
			break;
		default:
			break;
	}
  dir_l1 = STOP;
}

void floor_1_main(int *role_attribute, std::string user_name, bool &GameOver){
  srand (time(NULL));
	if(GameOver == false){
		for (int i=0;i<=11;i++){
	    for (int j=0;j<=21;j++ ){
	      wall_l1[i][j] =false;
	    }
	  }
		//start a loop
		if (count_Setup_l1_l1 == 0){
			Setup_l1();
			count_Setup_l1_l1++;
		}
		while(!GameOver){
			Draw_l1(role_attribute);
			input_l1(role_attribute, user_name, GameOver);
			logic_l1(role_attribute, user_name, GameOver);
		}
	}
	delete [] monster_l1;
	delete [] door_l1;
	delete [] key_l1;
	delete [] random_monster_l1;
	delete [] HP_box_l1;
	delete [] ATK_box_l1;
	delete [] DEF_box_l1;
	delete [] store_l1;
	delete [] guidance_l1;
}
