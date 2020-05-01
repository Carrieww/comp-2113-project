//Floor 3
#include <ncurses.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "floor.h"

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
m: small monsters with fixed positions
0: monsters with random positions
*/

//global variables
const int width_l3 = 20, height_l3 = 10;
int x_l3, y_l3;

const int num_fixed_small_monster_l3 = 5, num_fixed_big_monster_l3 = 8, num_fixed_door_l3 = 4, num_random_monster_l3= 5,
num_fixed_HP_box_l3 = 6, num_fixed_ATK_box_l3 = 3, num_fixed_DEF_box_l3 = 4, num_fixed_surprising_box_l3 = 2, num_fixed_key_l3 = 4;
int printable_num_fixed_door_l3 = 4, printable_num_fixed_small_monster_l3 = 5,printable_num_fixed_big_monster_l3 = 8, printable_num_random_monster_l3 = 5,
printable_num_fixed_HP_box_l3 = 6, printable_num_fixed_ATK_box_l3 = 3, printable_num_fixed_DEF_box_l3 = 4, printable_num_fixed_surprising_box_l3 = 2, printable_num_fixed_key_l3 = 4;

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
Fixed_sth_coordinate monster_l3[num_fixed_big_monster_l3];
Fixed_sth_coordinate small_monster_l3[num_fixed_small_monster_l3];
Fixed_sth_coordinate door_l3[num_fixed_door_l3];
Fixed_sth_coordinate key_l3[num_fixed_key_l3];
Fixed_sth_coordinate random_monster_l3[num_random_monster_l3];
Fixed_sth_coordinate HP_box_l3[num_fixed_HP_box_l3];
Fixed_sth_coordinate ATK_box_l3[num_fixed_ATK_box_l3];
Fixed_sth_coordinate DEF_box_l3[num_fixed_DEF_box_l3];
Fixed_sth_coordinate surprising_box_l3[num_fixed_surprising_box_l3];

//small monster position
int count_Q = 0, count_setup_l3=0;
int random_monster_l3_1x, random_monster_l3_1y,random_monster_l3_2x,random_monster_l3_2y,
random_monster_l3_3x, random_monster_l3_3y, random_monster_l3_4x, random_monster_l3_4y, random_monster_l3_5x, random_monster_l3_5y;

int Floor_l3 = 3;
//int role_level = 2, role_HP = 1047, role_ATK = 82, role_DEF = 46 ,
//role_EXP = 32, role_GOLD = 160, num_role_key = 2, num_role_key_2=0;
//char user_name[10] = "carrie";
//int role_attribute[10] = {user_name, role_level, role_EXP, role_HP, role_ATK, role_DEF, role_GOLD, num_role_key, num_role_key_2, diamond};

bool GameOver_l3, save_l3;
bool IsH_l3, IsA_l3, IsD_l3, IsQ_l3, Is_Info_l3, Is_Shop_l3, Leave_Shop_l3;
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir_l3;

//function: calculate hp_needed to beat monster '0'
int hp_needed_beat_0_l3(int role_ATK, int role_DEF){
	int hp_needed_beat_0;
	//when role_ATK <= rival_DEF, it means we can not attack the rival
	if(role_ATK <= random_monster_l3[1].DEF){
		hp_needed_beat_0 = -1;
	//when role_ATK > rival_DEF, attack results in decreases in hp
	//if rival_ATK <= role_ATK, their attack can not hurt me
	}else if (random_monster_l3[1].ATK <= role_DEF && role_ATK > random_monster_l3[1].DEF){
		hp_needed_beat_0 = 0;
	}else if ((random_monster_l3[1].HP % (role_ATK-random_monster_l3[1].DEF)) == 0 && role_ATK > random_monster_l3[1].DEF){
		hp_needed_beat_0 = (random_monster_l3[1].ATK-role_DEF)*((random_monster_l3[1].HP/(role_ATK-random_monster_l3[1].DEF))-1);
	}else if ((random_monster_l3[1].HP % (role_ATK-random_monster_l3[1].DEF)) != 0 && role_ATK > random_monster_l3[1].DEF){
		hp_needed_beat_0 = (random_monster_l3[1].ATK-role_DEF)*(random_monster_l3[1].HP/(role_ATK-random_monster_l3[1].DEF));
	}
	return hp_needed_beat_0;
}

//function: calculate hp_needed to beat monster 'M'
int hp_needed_beat_M_l3(int role_ATK, int role_DEF){
	int hp_needed_beat_M;
	if(role_ATK <= monster_l3[1].DEF){
		hp_needed_beat_M = -1;
	}else if (monster_l3[1].ATK <= role_DEF && role_ATK > monster_l3[1].DEF){
		hp_needed_beat_M = 0;
	}else if (monster_l3[1].HP % (role_ATK-monster_l3[1].DEF) == 0 && role_ATK > monster_l3[1].DEF){
		hp_needed_beat_M = (monster_l3[1].ATK-role_DEF)*((monster_l3[1].HP/(role_ATK-monster_l3[1].DEF))-1);
	}else if (monster_l3[1].HP % (role_ATK-monster_l3[1].DEF) != 0 && role_ATK > monster_l3[1].DEF){
		hp_needed_beat_M = (monster_l3[1].ATK-role_DEF)*(monster_l3[1].HP/(role_ATK-monster_l3[1].DEF));
	}
	return hp_needed_beat_M;
}

//function: calculate hp_needed to beat monster 'M'
int hp_needed_beat_m_l3(int role_ATK, int role_DEF){
	int hp_needed_beat_m;
	if(role_ATK <= small_monster_l3[1].DEF){
		hp_needed_beat_m = -1;
	}else if (small_monster_l3[1].ATK <= role_DEF && role_ATK > small_monster_l3[1].DEF){
		hp_needed_beat_m = 0;
	}else if (small_monster_l3[1].HP % (role_ATK-small_monster_l3[1].DEF) == 0 && role_ATK > small_monster_l3[1].DEF){
		hp_needed_beat_m = (small_monster_l3[1].ATK-role_DEF)*((small_monster_l3[1].HP/(role_ATK-small_monster_l3[1].DEF))-1);
	}else if (small_monster_l3[1].HP % (role_ATK-small_monster_l3[1].DEF) != 0 && role_ATK > small_monster_l3[1].DEF){
		hp_needed_beat_m = (small_monster_l3[1].ATK-role_DEF)*(small_monster_l3[1].HP/(role_ATK-small_monster_l3[1].DEF));
	}
	return hp_needed_beat_m;
}
//function: click 's', save status into .txt file in gamestatus folder
void saving_status_l3(char* user_name){
	move(12,0);
	char filename[40] = "game_status/";
	strcat(filename, user_name);
	strcat(filename, "_floor_2.txt");
	scr_dump(filename);
	printw("game status already saved!\ngame over...");
	GameOver_l3 = true;
}

// when click 'i' for more info of rivals in Floor 2
void show_info_l3(int *role_attribute){
	int hp_0 = hp_needed_beat_0_l3(role_attribute[3], role_attribute[4]);
	int hp_M = hp_needed_beat_M_l3(role_attribute[3], role_attribute[4]);
	int hp_m = hp_needed_beat_m_l3(role_attribute[3], role_attribute[4]);
	//print all information of monsters in Floor 2
	//if we can not attack the monster
	//the attack can not hurt them or hp_needed_kill_them > role_HP
	//just show ???
	move(12,0);
	printw("Here is some information about rivals in Floor %d:\n", Floor_l3);
	if (hp_0 < 0){
		printw("random monster '0': HP:  %d ATK: %d DEF: %d  HP_needed: ???\n", random_monster_l3[1].HP, random_monster_l3[1].ATK, random_monster_l3[1].DEF);
	}else{
		printw("random monster '0': HP:  %d ATK: %d DEF: %d  HP_needed: %d\n", random_monster_l3[1].HP, random_monster_l3[1].ATK, random_monster_l3[1].DEF, hp_0);
	}
	if (hp_M < 0){
		printw("fixed big monster 'M': HP:  %d ATK: %d DEF: %d  HP_needed: ???\n", monster_l3[1].HP, monster_l3[1].ATK, monster_l3[1].DEF);
	}else{
		printw("fixed big monster 'M': HP:  %d ATK: %d DEF: %d  HP_needed: %d\n", monster_l3[1].HP, monster_l3[1].ATK, monster_l3[1].DEF, hp_M);
	}
	if (hp_m < 0){
		printw("fixed small monster 'm': HP:  %d ATK: %d DEF: %d  HP_needed: ???\n", small_monster_l3[1].HP, small_monster_l3[1].ATK, small_monster_l3[1].DEF);
	}else{
		printw("fixed small monster 'm': HP:  %d ATK: %d DEF: %d  HP_needed: %d\n", small_monster_l3[1].HP, small_monster_l3[1].ATK, small_monster_l3[1].DEF, hp_m);
	}
}

//find the element in the correspond struct
//move the element after that in the struct forward
//when printing map next time, only print up to printable number
//things being collected or attacked will not be shown
void delete_sth_l3(int x, int y, char n){
	if(n == '$' || n == '@'){
		for(int i = 0; i < printable_num_fixed_door_l3; ++i){
			if (door_l3[i].x == x && door_l3[i].y == y){
				for(int j = i; j < printable_num_fixed_door_l3 - 1; ++j){
					door_l3[j].x = door_l3[j+1].x;
					door_l3[j].y = door_l3[j+1].y;
					door_l3[j].type = door_l3[j+1].type;
				}
			}
		}
		printable_num_fixed_door_l3--;
	}else if (n == 'M'){
		for(int i = 0; i < printable_num_fixed_big_monster_l3;++i){
			if(monster_l3[i].x == x && monster_l3[i].y == y){
				for(int j = i; j < printable_num_fixed_big_monster_l3 - 1; ++j){
					monster_l3[j].x = monster_l3[j+1].x;
					monster_l3[j].y = monster_l3[j+1].y;
				}
			}
		}
		printable_num_fixed_big_monster_l3--;
	}else if (n == 'm'){
		for(int i = 0; i < printable_num_fixed_small_monster_l3;++i){
			if(small_monster_l3[i].x == x && small_monster_l3[i].y == y){
				for(int j = i; j < printable_num_fixed_small_monster_l3 - 1; ++j){
					small_monster_l3[j].x = small_monster_l3[j+1].x;
					small_monster_l3[j].y = small_monster_l3[j+1].y;
				}
			}
		}
		printable_num_fixed_small_monster_l3--;
	}else if (n == '0'){
		for(int i = 0; i < printable_num_random_monster_l3;++i){
			if(random_monster_l3[i].x == x && random_monster_l3[i].y == y){
				for(int j = i; j < printable_num_random_monster_l3 - 1; ++j){
					random_monster_l3[j].x = random_monster_l3[j+1].x;
					random_monster_l3[j].y = random_monster_l3[j+1].y;
				}
			}
		}
		printable_num_random_monster_l3--;
	}else if(n == 'H'){
		for(int i = 0; i < printable_num_fixed_HP_box_l3;++i){
			if(HP_box_l3[i].x == x && HP_box_l3[i].y == y){
				for(int j = i; j < printable_num_fixed_HP_box_l3 - 1; ++j){
					HP_box_l3[j].x = HP_box_l3[j+1].x;
					HP_box_l3[j].y = HP_box_l3[j+1].y;
				}
			}
		}
		printable_num_fixed_HP_box_l3--;
	}else if(n == 'A'){
		for(int i = 0; i < printable_num_fixed_ATK_box_l3; ++i){
			if(ATK_box_l3[i].x == x && ATK_box_l3[i].y == y){
				for(int j = i; j < printable_num_fixed_ATK_box_l3 - 1; ++j){
					ATK_box_l3[j].x = ATK_box_l3[j+1].x;
					ATK_box_l3[j].y = ATK_box_l3[j+1].y;
				}
			}
		}
		printable_num_fixed_ATK_box_l3--;
	}else if(n == 'D'){
		for(int i = 0; i < printable_num_fixed_DEF_box_l3; ++i){
			if(DEF_box_l3[i].x == x && DEF_box_l3[i].y == y){
				for(int j = i; j < printable_num_fixed_DEF_box_l3 - 1; ++j){
					DEF_box_l3[j].x = DEF_box_l3[j+1].x;
					DEF_box_l3[j].y = DEF_box_l3[j+1].y;
				}
			}
		}
		printable_num_fixed_DEF_box_l3--;
	}else if(n == '^'){
		for(int i = 0; i < printable_num_fixed_key_l3; ++i){
			if(key_l3[i].x == x && key_l3[i].y == y){
				for(int j = i; j < printable_num_fixed_key_l3 - 1; ++j){
					key_l3[j].x = key_l3[j+1].x;
					key_l3[j].y = key_l3[j+1].y;
				}
			}
		}
		printable_num_fixed_key_l3--;
	}
}

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

void print_prumpt_l3(char n, int* role_attribute){
	move(12,0);
	if (n == 'H'){
		move(12,0);
		printw("HP +200!");
	}else if(n == 'A'){
		move(12,0);
		printw("ATK +5!");
	}else if(n == 'D'){
		move(12,0);
		printw("DEF +5!");
	}else if(n == 'L'){
		move(12,0);
		printw("Good luck! Bye!");
	}else if(n == 'Q'){
		if(count_Q == 1 && role_attribute[7] == 0){
			printw("Thanks for saving me! This is the key for the door $ on the second floor.\n");
			printw("Collect '!' and give it back to me, I can tell you how to rescue Dr.L.\n");
			role_attribute[7]++;
		}else if(count_Q > 1 && role_attribute[7] == 1){
			printw("Thanks for saving me! This is the key for the door $ on the second floor.\n");
			printw("Collect '!' and give it back to me, I can tell you how to rescue Dr.L.\n");
		}else if(role_attribute[8] == 1){
			role_attribute[8]--;
			printw("Great! The Big Big monster is afraid of snakes.\nGo and save Dr.L!\n");
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
	random_monster_l3_1x = 1+rand()% 2;
	random_monster_l3_1y = 7+rand()% 4;
	random_monster_l3_2x = 1+rand()% 8;
	random_monster_l3_2y = 6;
	random_monster_l3_3x = 9;
	random_monster_l3_3y = 1+rand()%10;
	random_monster_l3_4x = 11+rand()%7;
	random_monster_l3_4y = 10;
	random_monster_l3_5x = 10;
	random_monster_l3_5y = 6+rand()% 3;

	//store fixed big monsters into array
	//store them into struct
	int monster_l3_x[num_fixed_big_monster_l3] = {6,7,14,13,2,18,17,12};
	int monster_l3_y[num_fixed_big_monster_l3] = {10,9,1,2,1,8,8,3};
	int monster_l3_hp[num_fixed_big_monster_l3] = {250,250,250,250,250,250,250,250};
	int monster_l3_atk[num_fixed_big_monster_l3] = {100,100,100,100,100,100,100,100};
	int monster_l3_def[num_fixed_big_monster_l3] = {80,80,80,80,80,80,80,80};
	for(int i = 0; i < num_fixed_big_monster_l3; i++){
		monster_l3[i].x = monster_l3_x[i];
		monster_l3[i].y = monster_l3_y[i];
		monster_l3[i].HP = monster_l3_hp[i];
		monster_l3[i].ATK = monster_l3_atk[i];
		monster_l3[i].DEF = monster_l3_def[i];
	}

	//store fixed small monsters into array
	//store them into struct
	int small_monster_l3_x[num_fixed_small_monster_l3] = {6,13,12,1,18};
	int small_monster_l3_y[num_fixed_small_monster_l3] = {9,1,2,1,10};
	int small_monster_l3_hp[num_fixed_small_monster_l3] = {180,180,180,180,180};
	int small_monster_l3_atk[num_fixed_small_monster_l3] = {80,80,80,80,80};
	int small_monster_l3_def[num_fixed_small_monster_l3] = {70,70,70,70,70};
	for(int i = 0; i < num_fixed_small_monster_l3; i++){
		small_monster_l3[i].x = small_monster_l3_x[i];
		small_monster_l3[i].y = small_monster_l3_y[i];
		small_monster_l3[i].HP = small_monster_l3_hp[i];
		small_monster_l3[i].ATK = small_monster_l3_atk[i];
		small_monster_l3[i].DEF = small_monster_l3_def[i];
	}

	//store fixed doors into array
	//store them into struct
	int door_l3_x[num_fixed_door_l3] = {1,8,11,width_l3-2};
	int door_l3_y[num_fixed_door_l3] = {5,10,1,9};
	char door_l3_type[num_fixed_door_l3] = {'@', '@', '@', '@'};
	for(int i = 0; i < num_fixed_door_l3; i++){
		door_l3[i].x = door_l3_x[i];
		door_l3[i].y = door_l3_y[i];
		door_l3[i].type = door_l3_type[i];
	}

	//store random monsters into array
	//store them into struct
	int r_monster_x[num_random_monster_l3] = {random_monster_l3_1x, random_monster_l3_2x, random_monster_l3_3x, random_monster_l3_4x, random_monster_l3_5x};
	int r_monster_y[num_random_monster_l3] = {random_monster_l3_1y, random_monster_l3_2y, random_monster_l3_3y, random_monster_l3_4y, random_monster_l3_5y};
	int r_monster_hp[num_random_monster_l3] = {120,120,120,120,120};
	int r_monster_atk[num_random_monster_l3] = {50,50,50,50,50};
	int r_monster_def[num_random_monster_l3] = {40,40,40,40,40};
	for(int i = 0; i < num_random_monster_l3; i++){
		random_monster_l3[i].x = r_monster_x[i];
		random_monster_l3[i].y = r_monster_y[i];
		random_monster_l3[i].HP = r_monster_hp[i];
		random_monster_l3[i].ATK = r_monster_atk[i];
		random_monster_l3[i].DEF = r_monster_def[i];
	}

	//store fixed HP_box into array
	int HP_box_x[num_fixed_HP_box_l3] = {15,7,3,4,18,18};
	int HP_box_y[num_fixed_HP_box_l3] = {6,2,2,8,2,1};
	for(int i = 0; i < num_fixed_HP_box_l3; i++){
		HP_box_l3[i].x = HP_box_x[i];
		HP_box_l3[i].y = HP_box_y[i];
	}

	//store fixed ATK_box into array
	//store them into struct
	int ATK_box_x[num_fixed_HP_box_l3] = {7,18,18};
	int ATK_box_y[num_fixed_HP_box_l3] = {3,6,7};
	for(int i = 0; i < num_fixed_ATK_box_l3; i++){
		ATK_box_l3[i].x = ATK_box_x[i];
		ATK_box_l3[i].y = ATK_box_y[i];
	}
	//store fixed DEF_box into array
	//store them into struct
	int DEF_box_x[num_fixed_DEF_box_l3] = {1,2,5,18};
	int DEF_box_y[num_fixed_DEF_box_l3] = {3,3,8,3};
	for(int i = 0; i < num_fixed_DEF_box_l3; i++){
		DEF_box_l3[i].x = DEF_box_x[i];
		DEF_box_l3[i].y = DEF_box_y[i];
	}

	//store fixed keys into array
	//store them into struct
	int key_x[num_fixed_key_l3] = {5,18,17,6};
	int key_y[num_fixed_key_l3] = {4,4,4,8};
	for(int i = 0; i < num_fixed_key_l3; i++){
		key_l3[i].x = key_x[i];
		key_l3[i].y = key_y[i];
	}

}

//print the map
void Draw_l3(int *role_attribute){
	clear();
	//print Floor 2 map walls
	for(int i = 0; i < width_l3; ++i){
		if(i != 2){
			printw("#");
		}else{
			printw(" ");
		}}
	printw("\n");

	for(int i = 0; i < height_l3; ++i){
		for (int j = 0; j < width_l3; ++j){
			if(j == 0 || (j == 3 && i != height_l3 / 2 && i != 0 && i != 1 && i != 2 && i != 3)
			|| (i == 4  && j != 1 && j != 9 && j != width_l3 - 1 && j != 10)
			|| (j == 8  && i != 4 && i != height_l3 - 1 && i != 5)
			|| (i == height_l3 - 2  && j == 12) || (i == height_l3 - 2 && j == 13)
			|| (i == height_l3 - 2 && j == 14) || (i == height_l3 - 2 && j == 15) || (i == height_l3 - 2 && j == 16) || (i == height_l3 - 2 && j == 17)
			|| (j == 11 && i != 9 && i != 4 && i != 0)
			|| (i == 6 && j == 4) || (i == 6 && j == 5) || (i == 6 && j == 6) || (i == 6 && j == 7)
					){
				printw("#");
			}else if(j == width_l3 - 1){
				printw("#\n");
			}else{
				printw(" ");
			}
		}
	}

	for(int i = 0; i < width_l3; ++i){
		if (i != 10){
			printw("#");
		}else{
			printw(" ");
		}
	}
	//print existing fixed big monsters
	for(int k = 0; k < printable_num_fixed_big_monster_l3; ++k){
		move(monster_l3[k].y, monster_l3[k].x);
		printw("M");
	}

	//print existing fixed small monsters
	for(int k = 0; k < printable_num_fixed_small_monster_l3; ++k){
		move(small_monster_l3[k].y, small_monster_l3[k].x);
		printw("m");
	}
	//print existing doors
	for(int k = 0; k < printable_num_fixed_door_l3; ++k){
		move(door_l3[k].y, door_l3[k].x);
		printw("%c", door_l3[k].type);
	}

	//print exsiting random monsters
	for(int k = 0; k < printable_num_random_monster_l3; ++k){
		move(random_monster_l3[k].y, random_monster_l3[k].x);
		printw("0");
	}
	//print existing HP_box
	for(int k = 0; k < printable_num_fixed_HP_box_l3; ++k){
		move(HP_box_l3[k].y, HP_box_l3[k].x);
		printw("H");
	}

	//print existing ATK_box
	for(int k = 0; k < printable_num_fixed_ATK_box_l3; ++k){
		move(ATK_box_l3[k].y, ATK_box_l3[k].x);
		printw("A");
	}

	//print existing DEF_box
	for(int k = 0; k < printable_num_fixed_DEF_box_l3; ++k){
		move(DEF_box_l3[k].y, DEF_box_l3[k].x);
		printw("D");
	}

	//print existing keys
	for(int k = 0; k < printable_num_fixed_key_l3; ++k){
		move(key_l3[k].y, key_l3[k].x);
		printw("^");
	}

	//print role position
	move(y_l3,x_l3);
	printw("c");
	move(6,12);
	printw("Q");
	move(4,7);
	printw("+");
	move(12,0);
	//print role_attribute
	move(0,25);
	printw("Floor: %d", Floor_l3);
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
void input_l3(){
	//user input
	switch(getch()){
		case KEY_UP:
			dir_l3 = UP;
			break;
		case KEY_DOWN:
			dir_l3 = DOWN;
			break;
		case KEY_LEFT:
			dir_l3 = LEFT;
			break;
		case KEY_RIGHT:
			dir_l3 = RIGHT;
			break;
		case 27:
			GameOver_l3 = true;
			break;
		case 's':
			save_l3 = true;
			break;
		case 'i':
			Is_Info_l3 = true;
			dir_l3 = STOP;
			break;
		default:
		//it is like a loop
			input_l3();
			break;
	}
}

void logic_function_2_l3(int* role_attribute, char* user_name){
	//when collecting H, A, D, !, prumpt information of that
	if(IsH_l3 == true){
		print_prumpt_l3('H', role_attribute);
		IsH_l3 = false;
		input_l3();
	}else if(IsA_l3 == true){
		print_prumpt_l3('A', role_attribute);
		IsA_l3 = false;
		input_l3();
	}else if(IsD_l3 == true){
		print_prumpt_l3('D', role_attribute);
		IsD_l3 = false;
		input_l3();
	}else if(IsQ_l3 == true){
		print_prumpt_l3('Q', role_attribute);
		IsQ_l3 = false;
		input_l3();
	}else if(save_l3 == true){
		saving_status_l3(user_name);
	}else if(Is_Info_l3 == true){
		show_info_l3(role_attribute);
		Is_Info_l3 = false;
		input_l3();
	}else if(Leave_Shop_l3 == true){
		print_prumpt_l3('L', role_attribute);
		Leave_Shop_l3 = false;
		input_l3();
	}else if(Is_Shop_l3 == true){
		printw("Hello! The forth floor is extremely dangerous.\n");
		printw("You should equip yourself if you have enough Gold.\n");
		printw("HP+1000: 100(press '1')\nATK+20: 50(press '2')\nDEF+20: 50(press'3')\n(press '0' to leave)\n");

		switch(getch()){
			case '0':
				Is_Shop_l3 = false;
				Leave_Shop_l3 = true;
			case '1':
				if(role_attribute[5] >= 100){
					role_attribute[2] = role_attribute[2] + 1000;
					role_attribute[5] = role_attribute[5] - 100;
				}break;
			case '2':
				if(role_attribute[5] >= 50){
					role_attribute[3] = role_attribute[3] + 20;
					role_attribute[5]= role_attribute[5] - 50;
				}break;
			case '3':
				if(role_attribute[5] >= 50){
					role_attribute[4] = role_attribute[4] + 20;
					role_attribute[5]= role_attribute[5] - 50;
				}break;
		}
		dir_l3 = STOP;
	}else{
		input_l3();
	}
}

//function: check what is the char in the cell that the role is moving to
//different reaction regarding different cases
//we need to delete things except the wall(we should not print them in the next loop)
void logic_function_1_l3(int x, int y, int &change, int b, bool is_down,bool is_up, int *role_attribute, char* user_name){
	//if the cell is wall, then not move
	if(mvinch(y, x) == '#'){
		x = x;
	//if the cell is the door to floor_4, then go to floor_4
	}else if(y == 0 && x == 2 && is_up == true){
		//floor_4();

	//if the cell is the door to floor_2, then go to floor_2
	}else if(y == height_l3+1 && x == 10 && is_down == true){
		floor_2_main(role_attribute,user_name);

	//it is a door
	}else if(mvinch(y, x) == '@'){
		if (role_attribute[6] != 0){
			delete_sth_l3(x,y,'@');
			role_attribute[6]--;
		}
	//it is a special door
	}else if(mvinch(y, x) == '$'){
		if (role_attribute[7] != 0){
			delete_sth_l3(x,y,'$');
			role_attribute[7]--;
		}
	//Q is info NPC
	}else if(mvinch(y, x) == '^'){
		role_attribute[6]++;
		delete_sth_l3(x,y,'^');
	//Q is info npc
	}else if(mvinch(y, x) == 'Q'){
		IsQ_l3 = true;
		count_Q++;
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch(y, x) == '+'){
		Is_Shop_l3 = true;
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch(y, x) == 'H'){
		delete_sth_l3(x,y,'H');
		IsH_l3 = true;
		role_attribute[2] = role_attribute[2] + 200;
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch(y, x) == 'A'){
		delete_sth_l3(x,y,'A');
		IsA_l3 = true;
		role_attribute[3] = role_attribute[3] + 5;
	//it is a DEF_box, it will increase DEF by 5
	}else if(mvinch(y, x) == 'D'){
		delete_sth_l3(x,y,'D');
		IsD_l3 = true;
		role_attribute[4] = role_attribute[4] + 5;
		//it is a fixed position monster
		//calculate the hp_needed_beat_M, if we can kill it, then we can attack it
		//otherwise, we stay
	}else if(mvinch(y, x) == 'M'){
		int hp_M = hp_needed_beat_M_l3(role_attribute[3], role_attribute[4]);
		if (hp_M >= 0 && hp_M < role_attribute[2]){
			update_attribute_l3(250,100,80,25,30, role_attribute);
			delete_sth_l3(x,y,'M');
		}else{
			x = x;
		}
	//it is a fixed small monster, if we can kill it, then we can attack it
	//otherwise, we stay
	}else if(mvinch(y, x) == 'm'){
		int hp_m = hp_needed_beat_m_l3(role_attribute[3], role_attribute[4]);
		if (hp_m >= 0 && hp_m < role_attribute[2]){
			update_attribute_l3(100,80,70,20,20, role_attribute);
			delete_sth_l3(x,y,'m');
		}else{
			x = x;
		}
	//it is a random_generated_monster, if we can kill it, then we can attack it
	//otherwise, we stay
	}else if(mvinch(y, x) == '0'){
		int hp_0 = hp_needed_beat_0_l3(role_attribute[3], role_attribute[4]);
		if (hp_0 >= 0 && hp_0 < role_attribute[2]){
			update_attribute_l3(120,50,40,15,10, role_attribute);
			delete_sth_l3(x,y,'0');
		}else{
			x = x;
		}
	}else{
		change = change + b;
	}
}

//move up and down to generate different reactions
void logic_l3(int *role_attribute, char* user_name){
	switch (dir_l3) {
		case UP:
		logic_function_1_l3(x_l3,y_l3-1,y_l3,-1,false, true, role_attribute, user_name);
			break;
		case DOWN:
		logic_function_1_l3(x_l3,y_l3+1,y_l3,1,true, false, role_attribute, user_name);
			break;
		case LEFT:
			logic_function_1_l3(x_l3-1,y_l3,x_l3,-1, false, false, role_attribute,user_name);
			break;
		case RIGHT:
			logic_function_1_l3(x_l3+1,y_l3,x_l3,1, false, false, role_attribute,user_name);
			break;
		default:
			break;
	}
}

//floor_3_main function
int floor_3_main(int *role_attribute, char* user_name){
	srand (time(NULL));

	int role_level = role_attribute[0], role_EXP = role_attribute[1], role_HP = role_attribute[2],
	role_ATK = role_attribute[3], role_DEF = role_attribute[4], role_GOLD = role_attribute[5], num_role_key = role_attribute[6],
	num_role_key_2 = role_attribute[7], diamond = role_attribute[8];

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	//start a loop
	if (count_setup_l3 == 0){
		Setup_l3();
		count_setup_l3++;
	}
	while(!GameOver_l3){
		Draw_l3(role_attribute);
		logic_function_2_l3(role_attribute, user_name);
		logic_l3(role_attribute, user_name);
	}
	endwin();
	return 0;
}
