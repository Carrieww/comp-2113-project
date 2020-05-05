//Floor 3
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <string>
#include "floor.h"
using namespace std;

/* symbol used on floor 3
#: Wall, space between walls is the door connecting different floor
H: HP_box, increase 200 HP
A: ATK_box, increase 5 ATK
D: DEF_box, increase 5 DEF
@: door    ^: key for @
$: special door on the second floor    &: the key for $ on the third floor
Q: He has the key for $, find him to get more info.
!: surprising_box, sword and diamond(finish it and give it to Q on the third floor to know more info about Dr.L)
M: big monsters with fixed positions
m: small monsters with random positions
*/

//global variables
const int width_l3 = 20, height_l3 = 10;
int x_l3, y_l3, m_x_l3, m_y_l3;

int num_fixed_monster_l3, num_fixed_door_l3, num_random_monster_l3,
num_fixed_HP_box_l3, num_fixed_ATK_box_l3, num_fixed_DEF_box_l3, num_fixed_surprising_box_l3, num_fixed_key_l3;

//store everything with position into catagory and into this struct
struct Fixed_sth_coordinate{
	int x;
	int y;
	int ATK;
	int DEF;
	int HP;
	int type;
};
//use struct to store fixed and random monster's attribute, treasure's/door's/boxes' position,
Fixed_sth_coordinate * monster_l3 = new Fixed_sth_coordinate[num_fixed_monster_l3];
Fixed_sth_coordinate * door_l3 = new Fixed_sth_coordinate[num_fixed_door_l3];
Fixed_sth_coordinate * key_l3 = new Fixed_sth_coordinate[num_fixed_key_l3];
Fixed_sth_coordinate * random_monster_l3 = new Fixed_sth_coordinate[num_random_monster_l3];
Fixed_sth_coordinate * HP_box_l3 = new Fixed_sth_coordinate[num_fixed_HP_box_l3];
Fixed_sth_coordinate * ATK_box_l3 = new Fixed_sth_coordinate[num_fixed_ATK_box_l3];
Fixed_sth_coordinate * DEF_box_l3 = new Fixed_sth_coordinate[num_fixed_DEF_box_l3];

int count_Q = 0, count_setup_l3=0;
int Floor_l3 = 3;
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir_l3;
bool wall_l3[12][22], GameOver_l3, save_l3, IsH_l3, IsA_l3, IsD_l3, IsQ_l3, Is_Info_l3, Is_Shop_l3, Leave_Shop_l3, IsM_l3;

//all functions used in floor 3
//mimic move()in ncurses
void move_l3(int x,int y);
//clear the screen
void clear_l3();
//mimic getch() in ncurses, read user input without pressing entry
int getch_l3(void);
//mimic mvinch() in ncurses, check the char at (x,y) position
char mvinch_l3(int x, int y);
//use dynamic array to store all treasures except the wall by adding entrys
Fixed_sth_coordinate * addEntry_l3(Fixed_sth_coordinate * array, int & size, int x, int y);
//after collecting the treasure or killing monsters, delete that entry
void deleteEntry_l3(Fixed_sth_coordinate * &array, int & size, int x, int y);
//calculate hp_needed to beat random monster 'm'
int hp_needed_beat_m_l3(int role_ATK, int role_DEF);
//click 's', save status into .txt file in gamestatus folder
void saving_status_l3(string user_name);
// when click 'i' for more info of rivals in Floor 3
void show_info_l3(int *role_attribute);
//upgrade when exp reaches 99
void upgrade_l3(int* role_attribute);
//when encountering M, questions will be printed.
//once answer it correctly, M will disappear, player can move
void ask_questions_l3(int * role_attribute,int x, int y);
//when encountering treasures, print notifications
void print_prumpt_l3(char n, int* role_attribute);
//every time attacking rivals or collecting treasures
//we need to update role_attribute on the screen
void update_attribute_l3(int hp_value, int atk_value, int def_value,
	int add_exp, int add_gold, int* role_attribute);

//main functions
//setup all positions in this floor
void Setup_l3();
//print the map
void Draw_l3(int *role_attribute);
//check what is the char in the cell that the role is moving to
//different reaction regarding different cases
//we need to delete things except the wall(we should not print them in the next loop)
void logic_function_1_l3(int x, int y, int &change, int b, bool is_down,bool is_up, int *role_attribute, string user_name);
//print notifications or interact with users (if any), then get user input
void input_l3(int * role_attribute, string user_name);
//move up and down to generate different reactions
void logic_l3(int* role_attribute, string user_name);



//floor_3_main function
void floor_3_main(int *role_attribute, string user_name){
	srand (time(NULL));
	for (int i=0;i<=11;i++){
    for (int j=0;j<=21;j++ ){
      wall_l3[i][j] = false;
    }
  }
	//start a loop
	if (count_setup_l3 == 0){
		Setup_l3();
		count_setup_l3++;
	}
	while(!GameOver_l3){
		Draw_l3(role_attribute);
		input_l3(role_attribute, user_name);
		logic_l3(role_attribute, user_name);

	}
}

Fixed_sth_coordinate * addEntry_l3(Fixed_sth_coordinate * array, int & size, int x, int y){
	Fixed_sth_coordinate * new_array = new Fixed_sth_coordinate[size+1];
	for(int i = 0; i < size; ++i){
		new_array[i].x = array[i].x;
		new_array[i].y = array[i].y;
	}
	new_array[size].x = x;
	new_array[size].y = y;
	delete [] array;
	size++;
	return new_array;
}

void deleteEntry_l3(Fixed_sth_coordinate * &array, int & size, int x, int y){
	Fixed_sth_coordinate * new_array = new Fixed_sth_coordinate[size-1];
	bool flag = false;
	int idx = 0;
	for(int i = 0; i < size; ++i){
		if(array[i].x == x && array[i].y == y){
			idx = i;
			flag = true;
		}
	}
	int count = 0;
	for(int i = 0; i < size; ++i){
		if(i != idx){
			new_array[count] = array[i];
			count++;
		}
	}
	size--;
	delete [] array;
	array = new_array;
}

void move_l3(int x,int y){
  printf("\033[%d;%dH", (x+1), (y+1));
}
void clear_l3(){
  printf("\033[2J");
}
int getch_l3(void)
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

char mvinch_l3(int x, int y){
  if (wall_l3[y][x] == true){
    return ('#');
  }
	if(x == 7 && y == 4){
		return ('+');
	}
	if(x==12 && y==6){
		return ('Q');
	}
	//check if the char is M
	for(int k = 0; k < num_fixed_monster_l3; ++k){
    if (monster_l3[k].x == x && monster_l3[k].y == y){
      return ('M');
    }
	}
	//check if the char is doors
	for(int k = 0; k < num_fixed_door_l3; ++k){
		if(door_l3[k].x == x &&  door_l3[k].y== y){
      return ('@');
    }
	}
	//check if the char is random monsters
	for(int k = 0; k < num_random_monster_l3; ++k){
		if(random_monster_l3[k].x == x &&  random_monster_l3[k].y == y){
      return ('m');
    }
	}

	//check if the char is HP_box
	for(int k = 0; k < num_fixed_HP_box_l3; ++k){
		if(HP_box_l3[k].x ==x &&  HP_box_l3[k].y == y){
      return ('H');
    }
	}

	//check if the char is ATK_box
	for(int k = 0; k < num_fixed_ATK_box_l3; ++k){
		if(ATK_box_l3[k].x ==x && ATK_box_l3[k].y==y){
      return ('A');
    }
	}

	//check if the char is DEF_box
	for(int k = 0; k < num_fixed_DEF_box_l3; ++k){
		if(DEF_box_l3[k].x ==x &&DEF_box_l3[k].y==y){
      return ('D');
    }
	}

	//check if the char is key
	for(int k = 0; k < num_fixed_key_l3; ++k){
		if(key_l3[k].x ==x &&key_l3[k].y == y){
      return ('^');
    }
	}
  return (' ');
}

//calculate hp_needed to beat random monster 'm'
int hp_needed_beat_m_l3(int role_ATK, int role_DEF){
	int hp_needed_beat_m;
	//when role_ATK <= rival_DEF, it means we can not attack the rival
	if(role_ATK <= random_monster_l3[1].DEF){
		hp_needed_beat_m = -1;
	//when role_ATK > rival_DEF, attack results in decreases in hp
	//if rival_ATK <= role_ATK, their attack can not hurt me
	}else if (random_monster_l3[1].ATK <= role_DEF && role_ATK > random_monster_l3[1].DEF){
		hp_needed_beat_m = 0;
	}else if ((random_monster_l3[1].HP % (role_ATK-random_monster_l3[1].DEF)) == 0 && role_ATK > random_monster_l3[1].DEF){
		hp_needed_beat_m = (random_monster_l3[1].ATK-role_DEF)*((random_monster_l3[1].HP/(role_ATK-random_monster_l3[1].DEF))-1);
	}else if ((random_monster_l3[1].HP % (role_ATK-random_monster_l3[1].DEF)) != 0 && role_ATK > random_monster_l3[1].DEF){
		hp_needed_beat_m = (random_monster_l3[1].ATK-role_DEF)*(random_monster_l3[1].HP/(role_ATK-random_monster_l3[1].DEF));
	}
	return hp_needed_beat_m;
}


//click 's', save status into .txt file in gamestatus folder
void saving_status_l3(string user_name){
	cout << "Do you want to leave the game?";

	string filename = "game_status/"+user_name+"_floor_2.txt";

	cout<<"game status already saved!\ngame over...";
	GameOver_l3 = true;
}

// when click 'i' for more info of rivals in Floor 2
void show_info_l3(int *role_attribute){
	int hp_m = hp_needed_beat_m_l3(role_attribute[3], role_attribute[4]);
	//print all information of monsters in Floor 2
	//if we can not attack the monster
	//the attack can not hurt them or hp_needed_kill_them > role_HP
	//just show ???
	move_l3(12,0);
	cout << "Here is some information about rivals in Floor "<< Floor_l3<<": "<< endl;;
	if (hp_m < 0){
		cout << "random monster 'm': HP:  "<<random_monster_l3[1].HP<<" ATK: "<<random_monster_l3[1].ATK<<" DEF:  "<< random_monster_l3[1].DEF<<" HP_needed: ???"<< endl;
	}else{
		cout << "random monster 'm': HP:  "<<random_monster_l3[1].HP<<" ATK: "<<random_monster_l3[1].ATK<<" DEF:  "<< random_monster_l3[1].DEF<<" HP_needed: "<<hp_m<< endl;
	}
}

//upgrade when exp reaches 99
void upgrade_l3(int* role_attribute){
	if(role_attribute[0] <= 3){
		//atk
		role_attribute[3] = role_attribute[3] + 3;
		//def
		role_attribute[4] = role_attribute[4] + 3;
		//gold
		role_attribute[5] = role_attribute[5] + 20;
	}else{
		role_attribute[3] = role_attribute[3] + 9;
		role_attribute[4] = role_attribute[4] + 9;
		role_attribute[5] = role_attribute[5] + 60;
	}
}
//when encountering M, questions will be printed.
//once answer it correctly, M will disappear, player can move
void ask_questions_l3(int * role_attribute,int x, int y){
	int type;
	move_l3(12,0);
	cout << "I have very hard programming questions. \nCome on litter boy!" << endl;

	for (int k = 0; k < num_fixed_monster_l3; ++k){
		if(monster_l3[k].x == x && monster_l3[k].y == y){
			type = monster_l3[k].type;
		}
	}
	if(type == 0){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: The default deliminator character for the getline function is \'\\n\'. ('t'/'f')\n";
			ans = getch_l3();
			if (ans == 't'){
				cout << "Great job!\n";
				deleteEntry_l3(monster_l3,num_fixed_monster_l3,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 'f'){
				cout << "No\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch_l3();
			}else{
				cout << "please input again: ";
			}
		}
	}else if(type == 1){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: Structure variables can work with logical operators(&&/||). ('t'/'f')\n";
			ans = getch_l3();
			if (ans == 'f'){
				cout << "Great job!\n";
				deleteEntry_l3(monster_l3,num_fixed_monster_l3,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 't'){
				cout << "No\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch_l3();
			}else{
				cout << "please input again: ";
			}
		}
	}else if(type == 2){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: Name of an array is indeed a pointer. ('t'/'f')\n";
			ans = getch_l3();
			if (ans == 't'){
				cout << "Great job!\n";
				deleteEntry_l3(monster_l3,num_fixed_monster_l3,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 'f'){
				cout << "No\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch_l3();
			}else{
				cout << "please input again: ";
			}
		}
	}else if(type == 3){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: We can use .c_str() member function of a string object to convert a C++ string to C-string. ('t'/'f')\n";
			ans = getch_l3();
			if (ans == 't'){
				cout << "Great job!\n";
				deleteEntry_l3(monster_l3,num_fixed_monster_l3,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 'f'){
				cout << "No\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch_l3();
			}else{
				cout << "please input again: ";
			}
		}
	}
	cout << "press 'p' to continue...\n";
}

//when encountering treasures, print notifications
void print_prumpt_l3(char n, int* role_attribute){
	move_l3(12,0);
	if (n == 'H'){
		move_l3(12,0);
		cout << "HP +200!";
	}else if(n == 'A'){
		move_l3(12,0);
		cout << "ATK +5!";
	}else if(n == 'D'){
		move_l3(12,0);
		cout << "DEF +5!";
	}else if(n == 'L'){
		move_l3(12,0);
		cout << "Good luck! Bye!";
	}else if(n == 'Q'){
		if(count_Q == 1 && role_attribute[7] == 0){
			cout << "Thanks for saving me! This is the key for the door $ on the second floor.\n";
			cout << "Collect '!' and give it back to me, I can tell you how to rescue Dr.L.\n";
			role_attribute[7]++;
		}else if(count_Q > 1 && role_attribute[7] == 1){
			cout << "Thanks for saving me! This is the key for the door $ on the second floor.\n";
			cout << "Collect '!' and give it back to me, I can tell you how to rescue Dr.L.\n";
		}else if(role_attribute[8] == 1){
			role_attribute[8]--;
			cout << "Great! The Big Big monster is afraid of snakes.\nGo and save Dr.L!\n";
		}
	}
}
//every time attacking rivals or collecting treasures
//we need to update role_attribute on the screen
void update_attribute_l3(int hp_value, int atk_value, int def_value,
	int add_exp, int add_gold, int* role_attribute){
	int hp = hp_value, atk = atk_value, def = def_value;
	hp = hp - (role_attribute[3]-def);
	while(hp > 0){
		if (atk > role_attribute[4]){
			role_attribute[2] = role_attribute[2] - (atk - role_attribute[4]);
		}else{
			role_attribute[2] =role_attribute[2];
		}
		hp = hp - (role_attribute[3]-def);
	}
	role_attribute[3] = role_attribute[3] + atk/10;
	role_attribute[4] = role_attribute[4] + def/10;
	role_attribute[1] = role_attribute[1] + add_exp;
	role_attribute[5] = role_attribute[5] + add_gold;
	if(role_attribute[1] > 99){
		role_attribute[0]++;
		role_attribute[1] = role_attribute[1] - 99;
		upgrade_l3(role_attribute);
	}
}

//setup all positions in this floor
void Setup_l3(){
	GameOver_l3 =  false;
	dir_l3 = STOP;
	x_l3 = 10;
	y_l3 = 10;

	//store fixed big monsters into struct
	num_fixed_monster_l3 = 0;
	monster_l3 = addEntry_l3(monster_l3, num_fixed_monster_l3, 7,10);
	monster_l3 = addEntry_l3(monster_l3, num_fixed_monster_l3, 12,1);
	monster_l3 = addEntry_l3(monster_l3, num_fixed_monster_l3, 2,1);
	monster_l3 = addEntry_l3(monster_l3, num_fixed_monster_l3, 18,8);
	for(int i = 0; i < num_fixed_monster_l3; i++){
		monster_l3[i].type = i;
	}

	//store fixed doors into struct
	num_fixed_door_l3 = 0;
	door_l3 = addEntry_l3(door_l3, num_fixed_door_l3, 1,5);
	door_l3 = addEntry_l3(door_l3, num_fixed_door_l3, 8,10);
	door_l3 = addEntry_l3(door_l3, num_fixed_door_l3, 11,1);
	door_l3 = addEntry_l3(door_l3, num_fixed_door_l3, 18,9);

	//store random monsters into struct
	num_random_monster_l3 = 0;
	random_monster_l3 = addEntry_l3(random_monster_l3, num_random_monster_l3, 1+rand()% 2, 7+rand()% 4);
	random_monster_l3 = addEntry_l3(random_monster_l3, num_random_monster_l3, 1+rand()% 8, 6);
	random_monster_l3 = addEntry_l3(random_monster_l3, num_random_monster_l3, 9, 1+rand()%10);
	random_monster_l3 = addEntry_l3(random_monster_l3, num_random_monster_l3, 11+rand()%7, 10);
	random_monster_l3 = addEntry_l3(random_monster_l3, num_random_monster_l3, 10, 6+rand()% 3);
	for(int i = 0; i < num_random_monster_l3; i++){
		random_monster_l3[i].HP = 120;
		random_monster_l3[i].ATK = 50;
		random_monster_l3[i].DEF = 40;
	}

	//store fixed HP_box into struct
	num_fixed_HP_box_l3 = 0;
	HP_box_l3 = addEntry_l3(HP_box_l3, num_fixed_HP_box_l3, 15,6);
	HP_box_l3 = addEntry_l3(HP_box_l3, num_fixed_HP_box_l3, 7,2);
	HP_box_l3 = addEntry_l3(HP_box_l3, num_fixed_HP_box_l3, 3,2);
	HP_box_l3 = addEntry_l3(HP_box_l3, num_fixed_HP_box_l3, 4,8);
	HP_box_l3 = addEntry_l3(HP_box_l3, num_fixed_HP_box_l3, 18,2);
	HP_box_l3 = addEntry_l3(HP_box_l3, num_fixed_HP_box_l3, 18,1);

	//store fixed ATK_box into array
	//store them into struct
	num_fixed_ATK_box_l3 = 0;
	ATK_box_l3 = addEntry_l3(ATK_box_l3, num_fixed_ATK_box_l3, 7,3);
	ATK_box_l3 = addEntry_l3(ATK_box_l3, num_fixed_ATK_box_l3, 18,6);
	ATK_box_l3 = addEntry_l3(ATK_box_l3, num_fixed_ATK_box_l3, 18,7);

	//store fixed DEF_box into struct
	num_fixed_DEF_box_l3 = 0;
	DEF_box_l3 = addEntry_l3(DEF_box_l3, num_fixed_DEF_box_l3, 1,3);
	DEF_box_l3 = addEntry_l3(DEF_box_l3, num_fixed_DEF_box_l3, 2,3);
	DEF_box_l3 = addEntry_l3(DEF_box_l3, num_fixed_DEF_box_l3, 5,8);
	DEF_box_l3 = addEntry_l3(DEF_box_l3, num_fixed_DEF_box_l3, 18,3);


	//store fixed keys into struct
	num_fixed_key_l3 = 0;
	key_l3 = addEntry_l3(key_l3, num_fixed_key_l3, 5,4);
	key_l3 = addEntry_l3(key_l3, num_fixed_key_l3, 18,4);
	key_l3 = addEntry_l3(key_l3, num_fixed_key_l3, 17,4);
	key_l3 = addEntry_l3(key_l3, num_fixed_key_l3, 6,8);
}

//print the map
void Draw_l3(int *role_attribute){
	clear_l3();
	move_l3(0,0);
	//print Floor 2 map walls
	for(int i = 0; i < width_l3; ++i){
		if(i != 2){
			wall_l3[0][i] = true;
			cout << "#";
		}else{
			cout << " ";
		}}
	cout << "\n";

	for(int i = 1; i <= height_l3; ++i){
		for (int j = 0; j < width_l3; ++j){
			if(j == 0 || (j == 3 && i != height_l3 / 2 +1 && i != 1 && i != 2 && i != 3 && i != 4)
			|| (i == 5  && j != 1 && j != 9 && j != width_l3 - 1 && j != 10)
			|| (j == 8  && i != 5 && i != height_l3  && i != 6)
			|| (i == height_l3 - 1  && j == 12) || (i == height_l3 - 1 && j == 13)
			|| (i == height_l3 - 1 && j == 14) || (i == height_l3 - 1 && j == 15) || (i == height_l3 - 1 && j == 16) || (i == height_l3 - 1 && j == 17)
			|| (j == 11 && i != 10 && i != 5 && i != 1)
			|| (i == 7 && j == 4) || (i == 7 && j == 5) || (i == 7 && j == 6) || (i == 7 && j == 7)
					){
				wall_l3[i][j]=true;
				cout << "#";
			}else if(j == width_l3 - 1){
				wall_l3[i][j] = true;
				cout << "#\n";
			}else{
				cout << " ";
			}
		}
	}

	for(int i = 0; i < width_l3; ++i){
		if (i != 10){
			wall_l3[11][i] = true;
			cout << "#";
		}else{
			cout << " ";
		}
	}
	//print existing fixed big monsters
	for(int k = 0; k < num_fixed_monster_l3; ++k){
		move_l3(monster_l3[k].y, monster_l3[k].x);
		cout << "M";
	}

	//print existing doors
	for(int k = 0; k < num_fixed_door_l3; ++k){
		move_l3(door_l3[k].y, door_l3[k].x);
		cout << "@";
	}

	//print exsiting random monsters
	for(int k = 0; k < num_random_monster_l3; ++k){
		move_l3(random_monster_l3[k].y, random_monster_l3[k].x);
		cout << "m";
	}
	//print existing HP_box
	for(int k = 0; k < num_fixed_HP_box_l3; ++k){
		move_l3(HP_box_l3[k].y, HP_box_l3[k].x);
		cout << "H";
	}

	//print existing ATK_box
	for(int k = 0; k < num_fixed_ATK_box_l3; ++k){
		move_l3(ATK_box_l3[k].y, ATK_box_l3[k].x);
		cout << "A";
	}

	//print existing DEF_box
	for(int k = 0; k < num_fixed_DEF_box_l3; ++k){
		move_l3(DEF_box_l3[k].y, DEF_box_l3[k].x);
		cout << "D";
	}

	//print existing keys
	for(int k = 0; k < num_fixed_key_l3; ++k){
		move_l3(key_l3[k].y, key_l3[k].x);
		cout << "^";
	}

	//print role position
	move_l3(y_l3,x_l3);
	cout << "c";
	move_l3(6,12);
	cout << "Q";
	move_l3(4,7);
	cout << "+";
	move_l3(12,0);
	//print role_attribute
	move_l3(0,25);
	cout << "Floor: "<<Floor_l3;
	move_l3(2,25);
	cout << "Level: "<<role_attribute[0];
	move_l3(3,25);
	cout << "EXP: "<<role_attribute[1];
	move_l3(4,25);
	cout << "HP: "<<role_attribute[2];
	move_l3(5,25);
	cout << "ATK: "<<role_attribute[3];
	move_l3(6,25);
	cout << "DEF: "<<role_attribute[4];
	move_l3(8,25);
	cout << "GOLD: "<<role_attribute[5];
	move_l3(9,25);
	cout << "KEY(door): ^(@) "<<role_attribute[6];
	move_l3(10,25);
	cout << "           ?($) "<<role_attribute[7];
	move_l3(12,0);
}

//check what is the char in the cell that the role is moving to
//different reaction regarding different cases
//we need to delete things except the wall(we should not print them in the next loop)
void logic_function_1_l3(int x, int y, int &change, int b, bool is_down,bool is_up, int *role_attribute, string user_name){
	//if the cell is wall, then not move_l3
	if(mvinch_l3(x,y) == '#'){
		x = x;
	//if the cell is the door to floor_4, then go to floor_4
	}else if(y == 0 && x == 2 && is_up == true){
		floor_4_main(role_attribute,user_name);

	//if the cell is the door to floor_2, then go to floor_2
	}else if(y == height_l3+1 && x == 10 && is_down == true){
		floor_2_main(role_attribute,user_name);

	//it is a door
	}else if(mvinch_l3(x,y) == '@'){
		if (role_attribute[6] != 0){
			deleteEntry_l3(door_l3, num_fixed_door_l3,x,y);
			role_attribute[6]--;
		}
	//Q is info NPC
	}else if(mvinch_l3(x,y) == '^'){
		role_attribute[6]++;
		deleteEntry_l3(key_l3,num_fixed_key_l3,x,y);
	//Q is info npc
	}else if(mvinch_l3(x,y) == 'Q'){
		IsQ_l3 = true;
		count_Q++;//we only want Q to give special key once
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch_l3(x,y) == '+'){
		Is_Shop_l3 = true;
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch_l3(x,y) == 'H'){
		deleteEntry_l3(HP_box_l3,num_fixed_HP_box_l3,x,y);
		IsH_l3 = true;
		role_attribute[2] = role_attribute[2] + 200;
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch_l3(x,y) == 'A'){
		deleteEntry_l3(ATK_box_l3, num_fixed_ATK_box_l3, x,y);
		IsA_l3 = true;
		role_attribute[3] = role_attribute[3] + 5;
	//it is a DEF_box, it will increase DEF by 5
	}else if(mvinch_l3(x,y) == 'D'){
		deleteEntry_l3(DEF_box_l3, num_fixed_DEF_box_l3,x,y);
		IsD_l3 = true;
		role_attribute[4] = role_attribute[4] + 5;
		//it is a fixed position monster, with questions
	}else if(mvinch_l3(x,y) == 'M'){
		IsM_l3 = true;
		m_x_l3 = x;
		m_y_l3 = y;
	//it is a random small monster, if we can kill it, then we can attack it
	//otherwise, we stay
}else if(mvinch_l3(x,y) == 'm'){
		int hp_m = hp_needed_beat_m_l3(role_attribute[3], role_attribute[4]);
		if (hp_m >= 0 && hp_m < role_attribute[2]){
			update_attribute_l3(120,50,40,15,10, role_attribute);
			deleteEntry_l3(random_monster_l3, num_random_monster_l3,x,y);
		}else{
			x = x;
		}
	}else{
		change = change + b;
	}
}

//print notifications or interact with users (if any), then get user input
void input_l3(int * role_attribute, string user_name){
	//when collecting H, A, D, !, prumpt information of that
	if(IsH_l3 == true){
		print_prumpt_l3('H', role_attribute);
		IsH_l3 = false;
	}else if(IsA_l3 == true){
		print_prumpt_l3('A', role_attribute);
		IsA_l3 = false;
	}else if(IsD_l3 == true){
		print_prumpt_l3('D', role_attribute);
		IsD_l3 = false;
	}else if(IsQ_l3 == true){
		print_prumpt_l3('Q', role_attribute);
		IsQ_l3 = false;
	}else if(save_l3 == true){
		saving_status_l3(user_name);
	}else if(Is_Info_l3 == true){
		show_info_l3(role_attribute);
		Is_Info_l3 = false;
	}else if(Leave_Shop_l3 == true){
		print_prumpt_l3('L', role_attribute);
		Leave_Shop_l3 = false;
	}else if(IsM_l3 == true){
		ask_questions_l3(role_attribute,m_x_l3,m_y_l3);
		IsM_l3 = false;
		char ch;
		dir_l3 = STOP;
		if(getch_l3() == 'p'){
			return;
		}
	}else if(Is_Shop_l3 == true){
		//when there is a is a shop.
		//players can purchase by golds
		move_l3(12,0);
		cout << "Hello! The forth floor is extremely dangerous.\n";
		cout << "You should equip yourself if you have enough Gold.\n";
		cout << "HP+1000: 100(press '1')\nATK+20: 50(press '2')\nDEF+20: 50(press'3')\n(press '0' to leave)\n";
		char ch = getch_l3();
		if(ch == '1'){
			if(role_attribute[5] >= 100){
				role_attribute[2] = role_attribute[2] + 1000;
				role_attribute[5] = role_attribute[5] - 100;
			}
			dir_l3 = STOP;
			return;
		}else if (ch == '2'){
			if(role_attribute[5] >= 50){
				role_attribute[3] = role_attribute[3] + 20;
				role_attribute[5] = role_attribute[5] - 50;
			}
			dir_l3 = STOP;
			return;
		}else if( ch == '3'){
			if(role_attribute[5] >= 50){
				role_attribute[4] = role_attribute[4] + 20;
				role_attribute[5]= role_attribute[5] - 50;
			}
			dir_l3 = STOP;
			return;
		}else if(ch == '0'){
			Leave_Shop_l3 = true;
			dir_l3 = STOP;
			Is_Shop_l3 = false;
			return;
		}
	}
	//user input
	switch(getch_l3()){
		case 'w':
			dir_l3 = UP;
			break;
		case 's':
			dir_l3 = DOWN;
			break;
		case 'a':
			dir_l3 = LEFT;
			break;
		case 'd':
			dir_l3 = RIGHT;
			break;
		case 'q':
			GameOver_l3 = true;
			break;
		case '0':
			save_l3 = true;
			break;
		case 'i':
			Is_Info_l3 = true;
			dir_l3 = STOP;
			break;
		default:
		//it is like a loop
			input_l3(role_attribute, user_name);
			break;
	}
}

//move up and down to generate different reactions
void logic_l3(int* role_attribute, string user_name){
	switch (dir_l3) {
		case UP:
		logic_function_1_l3(x_l3,y_l3-1,y_l3,-1,false, true, role_attribute, user_name);
			break;
		case DOWN:
		logic_function_1_l3(x_l3,y_l3+1,y_l3,1,true, false, role_attribute, user_name);
			break;
		case LEFT:
			logic_function_1_l3(x_l3-1,y_l3,x_l3,-1, false, false, role_attribute, user_name);
			break;
		case RIGHT:
			logic_function_1_l3(x_l3+1,y_l3,x_l3,1, false, false, role_attribute, user_name);
			break;
		default:
			break;
	}
}
