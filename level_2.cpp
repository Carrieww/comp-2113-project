//Floor 2
#include <ncurses.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "level.h"

//  Screen Dumping
//While writing games, some times it becomes necessary to store the state of the screen
//restore it back to the same state. The function scr_dump() can be used to dump the screen contents to a file given as an argument.
//Later it can be restored by scr_restore function. These two simple functions can be used effectively to maintain a fast moving game with changing scenarios.


/* symbol used on floor 2
#: Wall, space between walls is the door connecting different floor
H: HP_box, increase 200 HP
A: ATK_box, increase 5 ATK
D: DEF_box, increase 5 DEF
@: door    ^: key for @
$: special door on the second floor    &: the key for $ on the third floor
!: surprising_box, sword and diamond(finish it and give it to Q on the third floor to know more info about Dr.L)
M: monsters with fixed positions
0: monsters with random positions
*/

//global variables
const int width = 20, height = 10, num_fixed_monster = 11, num_fixed_door = 4, num_random_monster=4,
num_fixed_HP_box = 6, num_fixed_ATK_box = 3, num_fixed_DEF_box = 4, num_fixed_surprising_box = 2, num_fixed_key = 4;
int printable_num_fixed_door = 4, printable_num_fixed_monster = 11, printable_num_random_monster = 4,
printable_num_fixed_HP_box = 6, printable_num_fixed_ATK_box = 3, printable_num_fixed_DEF_box = 4, printable_num_fixed_surprising_box = 2, printable_num_fixed_key = 4;

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
Fixed_sth_coordinate monster[num_fixed_monster];
Fixed_sth_coordinate door[num_fixed_door];
Fixed_sth_coordinate special_door[1];
Fixed_sth_coordinate key[num_fixed_key];
Fixed_sth_coordinate random_monster[num_random_monster];
Fixed_sth_coordinate HP_box[num_fixed_HP_box];
Fixed_sth_coordinate ATK_box[num_fixed_ATK_box];
Fixed_sth_coordinate DEF_box[num_fixed_DEF_box];
Fixed_sth_coordinate surprising_box[num_fixed_surprising_box];

//small monster position
int x, y,small_monster_1x, small_monster_1y,small_monster_2x,
small_monster_2y, small_monster_3x, small_monster_3y, small_monster_4x, small_monster_4y;

int count_setup = 0, Floor = 2;

bool GameOver, save, IsH, IsA, IsD, Is_Sur, Is_Info, open_special_door = false;
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

//function: calculate hp_needed to beat monster '0'
int hp_needed_beat_0(int role_ATK, int role_DEF){
	int hp_needed_beat_0;
	//when role_ATK <= rival_DEF, it means we can not attack the rival
	if(role_ATK <= random_monster[1].DEF){
		hp_needed_beat_0 = -1;
	//when role_ATK > rival_DEF, attack results in decreases in hp
	//if rival_ATK <= role_ATK, their attack can not hurt me
	}else if (random_monster[1].ATK <= role_DEF && role_ATK > random_monster[1].DEF){
		hp_needed_beat_0 = 0;
	}else if ((random_monster[1].HP % (role_ATK-random_monster[1].DEF)) == 0 && role_ATK > random_monster[1].DEF){
		hp_needed_beat_0 = (random_monster[1].ATK-role_DEF)*((random_monster[1].HP/(role_ATK-random_monster[1].DEF))-1);
	}else if ((random_monster[1].HP % (role_ATK-random_monster[1].DEF)) != 0 && role_ATK > random_monster[1].DEF){
		hp_needed_beat_0 = (random_monster[1].ATK-role_DEF)*(random_monster[1].HP/(role_ATK-random_monster[1].DEF));
	}
	return hp_needed_beat_0;
}

//function: calculate hp_needed to beat monster 'M'
int hp_needed_beat_M(int role_ATK, int role_DEF){
	int hp_needed_beat_M;
	if(role_ATK <= monster[1].DEF){
		hp_needed_beat_M = -1;
	}else if (monster[1].ATK <= role_DEF && role_ATK > monster[1].DEF){
		hp_needed_beat_M = 0;
	}else if (monster[1].HP % (role_ATK-monster[1].DEF) == 0 && role_ATK > monster[1].DEF){
		hp_needed_beat_M = (monster[1].ATK-role_DEF)*((monster[1].HP/(role_ATK-monster[1].DEF))-1);
	}else if (monster[1].HP % (role_ATK-monster[1].DEF) != 0 && role_ATK > monster[1].DEF){
		hp_needed_beat_M = (monster[1].ATK-role_DEF)*(monster[1].HP/(role_ATK-monster[1].DEF));
	}
	return hp_needed_beat_M;
}
//function: click 's', save status into .txt file in gamestatus folder
void saving_status(char* user_name){
	move(12,0);
	char filename[40] = "game_status/";
	strcat(filename, user_name);
	strcat(filename, "_floor_2.txt");
	scr_dump(filename);
	printw("game status already saved!\ngame over...");
	GameOver = true;
}

// when click 'i' for more info of rivals in Floor 2
void show_info(int* role_attribute){
	int hp_0 = hp_needed_beat_0(role_attribute[3], role_attribute[4]);
	int hp_M = hp_needed_beat_M(role_attribute[3], role_attribute[4]);
	//print all information of monsters in Floor 2
	//if we can not attack the monster
	//the attack can not hurt them or hp_needed_kill_them > role_HP
	//just show ???
	move(12,0);
	printw("Here is some information about rivals in Floor %d:\n", Floor);
	if (hp_0 < 0){
		printw("random monster '0': HP:  %d ATK: %d DEF:  %d HP_needed: ???\n", random_monster[1].HP, random_monster[1].ATK, random_monster[1].DEF);
	}else{
		printw("random monster '0': HP:  %d ATK: %d DEF:  %d HP_needed: %d\n", random_monster[1].HP, random_monster[1].ATK, random_monster[1].DEF, hp_0);
	}
	if (hp_M < 0){
		printw("fixed monster 'M': HP:  %d ATK: %d DEF:  %d HP_needed: ???\n", monster[1].HP, monster[1].ATK, monster[1].DEF);
	}else{
		printw("fixed monster 'M': HP:  %d ATK: %d DEF:  %d HP_needed: %d\n", monster[1].HP, monster[1].ATK, monster[1].DEF, hp_M);
	}
	printw("'!' represents treasures. They will help you.\n");

}

//find the element in the correspond struct
//move the element after that in the struct forward
//when printing map next time, only print up to printable number
//things being collected or attacked will not be shown
void delete_sth(int x, int y, char n){
	if(n == '@'){
		for(int i = 0; i < printable_num_fixed_door; ++i){
			if (door[i].x == x && door[i].y == y){
				for(int j = i; j < printable_num_fixed_door - 1; ++j){
					door[j].x = door[j+1].x;
					door[j].y = door[j+1].y;
					door[j].type = door[j+1].type;
				}
			}
		}
		printable_num_fixed_door--;
	}else if (n == 'M'){
		for(int i = 0; i < printable_num_fixed_monster;++i){
			if(monster[i].x == x && monster[i].y == y){
				for(int j = i; j < printable_num_fixed_monster - 1; ++j){
					monster[j].x = monster[j+1].x;
					monster[j].y = monster[j+1].y;
				}
			}
		}
		printable_num_fixed_monster--;
	}else if (n == '0'){
		for(int i = 0; i < printable_num_random_monster;++i){
			if(random_monster[i].x == x && random_monster[i].y == y){
				for(int j = i; j < printable_num_random_monster - 1; ++j){
					random_monster[j].x = random_monster[j+1].x;
					random_monster[j].y = random_monster[j+1].y;
				}
			}
		}
		printable_num_random_monster--;
	}else if(n == 'H'){
		for(int i = 0; i < printable_num_fixed_HP_box;++i){
			if(HP_box[i].x == x && HP_box[i].y == y){
				for(int j = i; j < printable_num_fixed_HP_box - 1; ++j){
					HP_box[j].x = HP_box[j+1].x;
					HP_box[j].y = HP_box[j+1].y;
				}
			}
		}
		printable_num_fixed_HP_box--;
	}else if(n == 'A'){
		for(int i = 0; i < printable_num_fixed_ATK_box; ++i){
			if(ATK_box[i].x == x && ATK_box[i].y == y){
				for(int j = i; j < printable_num_fixed_ATK_box - 1; ++j){
					ATK_box[j].x = ATK_box[j+1].x;
					ATK_box[j].y = ATK_box[j+1].y;
				}
			}
		}
		printable_num_fixed_ATK_box--;
	}else if(n == 'D'){
		for(int i = 0; i < printable_num_fixed_DEF_box; ++i){
			if(DEF_box[i].x == x && DEF_box[i].y == y){
				for(int j = i; j < printable_num_fixed_DEF_box - 1; ++j){
					DEF_box[j].x = DEF_box[j+1].x;
					DEF_box[j].y = DEF_box[j+1].y;
				}
			}
		}
		printable_num_fixed_DEF_box--;
	}else if(n == '!'){
		for(int i = 0; i < printable_num_fixed_surprising_box; ++i){
			if(surprising_box[i].x == x && surprising_box[i].y == y){
				for(int j = i; j < printable_num_fixed_surprising_box - 1; ++j){
					surprising_box[j].x = surprising_box[j+1].x;
					surprising_box[j].y = surprising_box[j+1].y;
				}
			}
		}
		printable_num_fixed_surprising_box--;
	}else if(n == '^'){
		for(int i = 0; i < printable_num_fixed_key; ++i){
			if(key[i].x == x && key[i].y == y){
				for(int j = i; j < printable_num_fixed_key - 1; ++j){
					key[j].x = key[j+1].x;
					key[j].y = key[j+1].y;
				}
			}
		}
		printable_num_fixed_key--;
	}
}

void upgrade(int* role_attribute){
	//atk
	role_attribute[3] = role_attribute[3] + 5;
	//def
	role_attribute[4] = role_attribute[4] + 5;
	//gold
	role_attribute[5] = role_attribute[5] + 20;
}

void print_prumpt(char n, int * role_attribute){
	if (n == 'H'){
		move(12,0);
		printw("HP +200!");
	}else if(n == 'A'){
		move(12,0);
		printw("ATK +5!");
	}else if(n == 'D'){
		move(12,0);
		printw("DEF +5!");
	}else if(n == '!'){
		move(12,0);
		if(y == 10){
			printw("This is a diamond.\n");
			role_attribute[8]++;
		}else{
			printw("You've got a sword! ATK +10!");
			role_attribute[3] = role_attribute[3]+10;
		}
	}
}


//every time attacking rivals or collecting treasures
//we need to update role_attribute on the screen
void update_attribute(int hp_value, int atk_value, int def_value,
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
		upgrade(role_attribute);
	}
}
//setup all positions in this floor
void Setup(){
	GameOver =  false;
	dir = STOP;
	x = 2;
	y = 1;
	small_monster_1x = 1;
	small_monster_1y = 1+rand()%(height-1);
	small_monster_2x = 3+rand()% (width-4);
	small_monster_2y = 6;
	small_monster_3x = 2;
	small_monster_3y = 2+rand()%(height-1);
	small_monster_4x = 8+rand()%2;
	small_monster_4y = 1+rand()%5;

	//store fixed monsters into array
	//store them into struct
	int monster_x[num_fixed_monster] = {5,5,6,width-2,5,5,6,width-2,width-3, 13,13};
	int monster_y[num_fixed_monster] = {1,2,2,3, height, height-1, height-1, height-2, height-2, 3, 4};
	int monster_hp[num_fixed_monster] = {100, 100, 100, 100, 100, 100,100,100,100, 100,100};
	int monster_atk[num_fixed_monster] = {20, 20, 20, 20, 20, 20, 20, 20, 20,20,20};
	int monster_def[num_fixed_monster] = {10, 10, 10, 10, 10, 10, 10, 10, 10,10,10};
	for(int i = 0; i < num_fixed_monster; i++){
		monster[i].x = monster_x[i];
		monster[i].y = monster_y[i];
		monster[i].HP = monster_hp[i];
		monster[i].ATK = monster_atk[i];
		monster[i].DEF = monster_def[i];
	}

	//store fixed doors and special door into array
	//store them into struct
	special_door[0].x = 11;
	special_door[0].y = 9;
	int door_x[num_fixed_door] = {7,7,width-2,width-2};
	int door_y[num_fixed_door] = {1, height, height-3,2};
	char door_type[num_fixed_door] = {'@', '@', '@', '@'};
	for(int i = 0; i < num_fixed_door; i++){
		door[i].x = door_x[i];
		door[i].y = door_y[i];
		door[i].type = door_type[i];
	}

	//store random monsters into array
	//store them into struct
	int r_monster_x[num_random_monster] = {small_monster_1x, small_monster_2x, small_monster_3x, small_monster_4x};
	int r_monster_y[num_random_monster] = {small_monster_1y, small_monster_2y, small_monster_3y, small_monster_4y};
	int r_monster_hp[num_random_monster] = {50, 50, 50, 50};
	int r_monster_atk[num_random_monster] = {15,15,15,15};
	int r_monster_def[num_random_monster] = {5, 5, 5, 5};
	for(int i = 0; i < num_random_monster; i++){
		random_monster[i].x = r_monster_x[i];
		random_monster[i].y = r_monster_y[i];
		random_monster[i].HP = r_monster_hp[i];
		random_monster[i].ATK = r_monster_atk[i];
		random_monster[i].DEF = r_monster_def[i];
	}

	//store fixed surprising_box into array
	//store them into struct
	int box_x[num_fixed_surprising_box] = {18,11};
	int box_y[num_fixed_surprising_box] = {10,3};
	for(int i = 0; i < num_fixed_surprising_box; i++){
		surprising_box[i].x = box_x[i];
		surprising_box[i].y = box_y[i];
	}

	//store fixed HP_box into array
	int HP_box_x[num_fixed_HP_box] = {4,5,5,6,12,14};
	int HP_box_y[num_fixed_HP_box] = {4,4,8,8,height,height};
	for(int i = 0; i < num_fixed_HP_box; i++){
		HP_box[i].x = HP_box_x[i];
		HP_box[i].y = HP_box_y[i];
	}

	//store fixed ATK_box into array
	//store them into struct
	int ATK_box_x[num_fixed_HP_box] = {4,15,17};
	int ATK_box_y[num_fixed_HP_box] = {1,10,10};
	for(int i = 0; i < num_fixed_ATK_box; i++){
		ATK_box[i].x = ATK_box_x[i];
		ATK_box[i].y = ATK_box_y[i];
	}
	//store fixed DEF_box into array
	//store them into struct
	int DEF_box_x[num_fixed_DEF_box] = {11,12,13,16};
	int DEF_box_y[num_fixed_DEF_box] = {4,4,10,10};
	for(int i = 0; i < num_fixed_DEF_box; i++){
		DEF_box[i].x = DEF_box_x[i];
		DEF_box[i].y = DEF_box_y[i];
	}

	//store fixed keys into array
	//store them into struct
	int key_x[num_fixed_key] = {6,14,4,4};
	int key_y[num_fixed_key] = {4,4,8,10};
	for(int i = 0; i < num_fixed_key; i++){
		key[i].x = key_x[i];
		key[i].y = key_y[i];
	}

}

//print the map
void Draw(int* role_attribute){
	clear();
	//print Floor 2 map walls
	for(int i = 0; i < width; ++i){
		if(i != 2){
			printw("#");
		}else{
			printw(" ");
		}}
	printw("\n");

	for(int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j){
			if(j == 0 || (j == 3 && i != height / 2 )
			|| (i == 4  && j != 1 && j != 2 && j != 8 && j != 9 && j != width - 1)
			|| (i == 6  && j != 1 && j != 2 && j != 8 && j != 9 && j != width - 1 && j!= width - 2)
			|| (j == 7 && i != 0 && i != 4 && i != 5 && i != 6 && i != height - 1)
			|| (j == 10 && i != 0 && i != 4 && i != 5 && i != 6 )
			|| (i == 1 && j == 11) || (i == 1  && j == 12) || (i == 1 && j == 13)
			|| (i == 1 && j == 14) || (i == 1 && j == 15) || (i == 1 && j == 16) || (i == 1 && j == 17)
			|| (i == height - 2 && j == 18) || (i == height - 2  && j == 12) || (i == height - 2 && j == 13)
			|| (i == height - 2 && j == 14) || (i == height - 2 && j == 15) || (i == height - 2 && j == 16) || (i == height - 2 && j == 17)
					){
				printw("#");
			}else if(j == width - 1){
				printw("#\n");
			}else{
				printw(" ");
			}
		}
	}

	for(int i = 0; i < width; ++i){
		if (i != 2){
			printw("#");
		}else{
			printw(" ");
		}
	}

	//print existing fixed monsters
	for(int k = 0; k < printable_num_fixed_monster; ++k){
		move(monster[k].y, monster[k].x);
		printw("M");
	}
	//print existing doors
	for(int k = 0; k < printable_num_fixed_door; ++k){
		move(door[k].y, door[k].x);
		printw("%c", door[k].type);
	}

	//print exsiting random monsters
	for(int k = 0; k < printable_num_random_monster; ++k){
		move(random_monster[k].y, random_monster[k].x);
		printw("0");
	}
	//print existing HP_box
	for(int k = 0; k < printable_num_fixed_HP_box; ++k){
		move(HP_box[k].y, HP_box[k].x);
		printw("H");
	}

	//print existing ATK_box
	for(int k = 0; k < printable_num_fixed_ATK_box; ++k){
		move(ATK_box[k].y, ATK_box[k].x);
		printw("A");
	}

	//print existing DEF_box
	for(int k = 0; k < printable_num_fixed_DEF_box; ++k){
		move(DEF_box[k].y, DEF_box[k].x);
		printw("D");
	}

	//print existing surprising_box
	for(int k = 0; k < printable_num_fixed_surprising_box; ++k){
		move(surprising_box[k].y, surprising_box[k].x);
		printw("!");
	}

	//print existing keys
	for(int k = 0; k < printable_num_fixed_key; ++k){
		move(key[k].y, key[k].x);
		printw("^");
	}

	//print role position
	move(y,x);
	printw("c");
	if(open_special_door == false){
		move(special_door[0].y, special_door[0].x);
		printw("$");
	}
	//print role_attribute
	move(0,25);
	printw("Floor: %d", Floor);
	move(2,25);
	printw("Level: %d", role_attribute[0]);
	move(3,25);
	printw("EXP: %d", role_attribute[1]);
	move(4,25);
	printw("HP: %d", role_attribute[2]);
	move(5,25);
	printw("ATK: %d",role_attribute[3]);
	move(6,25);
	printw("DEF: %d",role_attribute[4]);
	move(8,25);
	printw("GOLD: %d",role_attribute[5]);
	move(9,25);
	printw("KEY(door): ^(@) %d",role_attribute[6]);
	move(10,25);
	printw("           ?($) %d",role_attribute[7]);
	move(12,0);
}

//use getch() to read user input prumptly
void input(int* role_attribute, char* user_name){
	//when collecting H, A, D, !, prumpt information of that
	if(IsH == true){
		print_prumpt('H', role_attribute);
		IsH = false;
	}else if(IsA == true){
		print_prumpt('A', role_attribute);
		IsA = false;
	}else if(IsD == true){
		print_prumpt('D', role_attribute);
		IsD = false;
	}else if(Is_Sur == true){
		print_prumpt('!', role_attribute);
		Is_Sur = false;
	}else if(save == true){
		saving_status(user_name);
	}else if(Is_Info == true){
		show_info(role_attribute);
		Is_Info = false;
	}
	//user input
	switch(getch()){
		case KEY_UP:
			dir = UP;
			break;
		case KEY_DOWN:
			dir = DOWN;
			break;
		case KEY_LEFT:
			dir = LEFT;
			break;
		case KEY_RIGHT:
			dir = RIGHT;
			break;
		case 27:
			GameOver = true;
			break;
		case 's':
			save = true;
			break;
		case 'i':
			Is_Info = true;
			break;
		default:
		//it is like a loop
			input(role_attribute, user_name);
			break;
	}
}
//function: check what is the char in the cell that the role is moving to
//different reaction regarding different cases
//we need to delete things except the wall(we should not print them in the next loop)
void logic_function_1(int x, int y, int &change, int b, bool is_down,bool is_up, int* role_attribute, char* user_name){
	//if the cell is wall, then not move
	if(mvinch(y, x) == '#'){
		x = x;
	//if the cell is the door to floor_1, then go to floor_1
	}else if(y == 0 && is_up == true){
		//floor_1();

	//if the cell is the door to floor_2, then go to floor_2
	}else if(y == height+1 && is_down == true){
		floor_3_main(role_attribute,user_name);

	//it is a door
	}else if(mvinch(y, x) == '@'){
		if (role_attribute[6] != 0){
			delete_sth(x,y,'@');
			role_attribute[6]--;
		}
	//it is a special door
	}else if(mvinch(y, x) == '$'){
		if (role_attribute[7] != 0){
			open_special_door = true;
			role_attribute[7]--;
		}
	//it is a key
	}else if(mvinch(y, x) == '^'){
		role_attribute[6]++;
		delete_sth(x,y,'^');
	//it is a HP_box, it will increase HP by 200
	}else if(mvinch(y, x) == 'H'){
		delete_sth(x,y,'H');
		IsH = true;
		role_attribute[2] = role_attribute[2] + 200;
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch(y, x) == 'A'){
		delete_sth(x,y,'A');
		IsA = true;
		role_attribute[3] = role_attribute[3] + 5;
	//it is a DEF_box, it will increase DEF by 5
	}else if(mvinch(y, x) == 'D'){
		delete_sth(x,y,'D');
		IsD = true;
		role_attribute[4] = role_attribute[4] + 5;
	//it is a surprising_box, it will have sword or diamond
	}else if(mvinch(y, x) == '!'){
		delete_sth(x,y,'!');
		Is_Sur = true;
	//it is a fixed position monster
	//calculate the hp_needed_beat_M, if we can kill it, then we can attack it
	//otherwise, we stay
	}else if(mvinch(y, x) == 'M'){
		int hp_M = hp_needed_beat_M(role_attribute[3], role_attribute[4]);
		if (hp_M >= 0 && hp_M < role_attribute[2]){
			update_attribute(100,20,10,9,10, role_attribute);
			delete_sth(x,y,'M');
		}else{
			x = x;
		}
	//it is a random_generated_monster, if we can kill it, then we can attack it
	//otherwise, we stay
	}else if(mvinch(y, x) == '0'){
		int hp_0 = hp_needed_beat_0(role_attribute[3], role_attribute[4]);
		if (hp_0 >= 0 && hp_0 < role_attribute[2]){
			update_attribute(50,15,5,3,5,role_attribute);
			delete_sth(x,y,'0');
		}else{
			x = x;
		}
	}else{
		change = change + b;
	}
}

//move up and down to generate different reactions
void logic(int* role_attribute, char* user_name){
	switch (dir) {
		case UP:
		logic_function_1(x,y-1,y,-1,false, true, role_attribute, user_name);
			break;
		case DOWN:
		logic_function_1(x,y+1,y,1,true, false, role_attribute, user_name);
			break;
		case LEFT:
			logic_function_1(x-1,y,x,-1, false, false, role_attribute, user_name);
			break;
		case RIGHT:
			logic_function_1(x+1,y,x,1, false, false, role_attribute, user_name);
			break;
		default:
			break;
	}
}

/*this is used to test the game compliation between level 2 and level 3
int main(){
	int role_level = 2, role_HP = 1000, role_ATK = 10, role_DEF = 10, role_EXP = 20, role_GOLD = 50, num_role_key = 10, num_role_key_2 = 0, diamond = 0;
	char user_name[10] = "carrie";
	int role_attribute[9] = {role_level, role_EXP, role_HP,
		role_ATK, role_DEF, role_GOLD, num_role_key, num_role_key_2, diamond};
	floor_2_main(role_attribute, user_name);
	return 0;
}*/

//floor_2_main function
int floor_2_main(int *role_attribute, char* user_name){
	srand (time(NULL));

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	//start a loop
	if (count_setup == 0){
		Setup();
		count_setup++;
	}
	while(!GameOver){
		Draw(role_attribute);
		input(role_attribute, user_name);
		logic(role_attribute, user_name);
	}
	endwin();
	return 0;
}

