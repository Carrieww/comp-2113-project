//Floor 2
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <string>
#include "level.h"
using namespace std;

/* symbol used on floor 2
#: Wall, space between walls is the door connecting different floor
H: HP_box, increase 200 HP
A: ATK_box, increase 5 ATK
D: DEF_box, increase 5 DEF
@: door    ^: key for @
$: special door on the second floor    &: the key for $ on the third floor
!: surprising_box, sword and diamond(finish it and give it to Q on the third floor to know more info about Dr.L)
M: monsters with fixed positions
m: monsters with random positions
*/

//global variables
const int width = 20, height = 10;
//fixed monster's x,y
int m_x_l2, m_y_l2;
int num_random_monster,num_fixed_monster, num_fixed_door, num_fixed_DEF_box, num_fixed_ATK_box,
num_fixed_HP_box, num_fixed_key, num_fixed_surprising_box;
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
Fixed_sth_coordinate * monster = new Fixed_sth_coordinate[num_fixed_monster];
Fixed_sth_coordinate * door = new Fixed_sth_coordinate[num_fixed_door];
Fixed_sth_coordinate special_door[1];
Fixed_sth_coordinate * key = new Fixed_sth_coordinate[num_fixed_key];
Fixed_sth_coordinate * random_monster = new Fixed_sth_coordinate[num_random_monster];
Fixed_sth_coordinate * HP_box = new Fixed_sth_coordinate[num_fixed_HP_box];
Fixed_sth_coordinate * ATK_box = new Fixed_sth_coordinate[num_fixed_ATK_box];
Fixed_sth_coordinate * DEF_box = new Fixed_sth_coordinate[num_fixed_DEF_box];
Fixed_sth_coordinate * surprising_box = new Fixed_sth_coordinate[num_fixed_surprising_box];

int x, y;
int count_setup_l2 = 0, Floor_l2 = 2;

bool wall[12][22],GameOver, save, IsM, IsH, IsA, IsD, Is_Sur, Is_Info, open_special_door = false;
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

//all functions used in floor 2
//mimic move()in ncurses
void move(int x,int y);
//clear the screen
void clear();
//mimic getch() in ncurses, read user input without pressing entry
int getch(void);
//mimic mvinch() in ncurses, check the char at (x,y) position
char mvinch(int x, int y);
//use dynamic array to store all treasures except the wall by adding entrys
Fixed_sth_coordinate * addEntry(Fixed_sth_coordinate * array, int & size, int x, int y);
//after collecting the treasure or killing monsters, delete that entry
void deleteEntry(Fixed_sth_coordinate * &array, int & size, int x, int y);
//calculate hp_needed to beat random monster 'm'
int hp_needed_beat_m(int role_ATK, int role_DEF);
//click 's', save status into .txt file in gamestatus folder
void saving_status(string user_name);
// when click 'i' for more info of rivals in Floor 2
void show_info(int *role_attribute);
//upgrade when exp reaches 99
void upgrade(int* role_attribute);
//when encountering M, questions will be printed.
//once answer it correctly, M will disappear, player can move
void ask_questions(int * role_attribute,int x, int y);
//when encountering treasures, print notifications
void print_prumpt(char n, int* role_attribute);
//every time attacking rivals or collecting treasures
//we need to update role_attribute on the screen
void update_attribute(int hp_value, int atk_value, int def_value,
	int add_exp, int add_gold, int* role_attribute);

//main functions
//setup all positions in this floor
void Setup();
//print the map
void Draw(int *role_attribute);
//check what is the char in the cell that the role is moving to
//different reaction regarding different cases
//we need to delete things except the wall(we should not print them in the next loop)
void logic_function_1(int x, int y, int &change, int b, bool is_down,bool is_up, int *role_attribute, string user_name);
//print notifications or interact with users (if any), then get user input
void input(int * role_attribute, string user_name);
//move up and down to generate different reactions
void logic(int* role_attribute, string user_name);



//floor_2_main function
void floor_2_main(int *role_attribute, string user_name){
	srand (time(NULL));

	for (int i=0;i<=11;i++){
    for (int j=0;j<=21;j++ ){
      wall[i][j] = false;
    }
  }

	//start a loop
	if (count_setup_l2 == 0){
		Setup();
		count_setup_l2++;
	}
	while(!GameOver){
		Draw(role_attribute);
		input(role_attribute, user_name);
		logic(role_attribute, user_name);
	}
}

void move(int x,int y){
  printf("\033[%d;%dH", (x+1), (y+1));
}
void clear(){
  printf("\033[2J");
}
int getch(void)
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

char mvinch(int x, int y){
  if (wall[y][x] == true){
    return ('#');
  }
	//check if the char is M
	for(int k = 0; k < num_fixed_monster; ++k){
    if (monster[k].x == x && monster[k].y == y){
      return ('M');
    }
	}
	//check if the char is doors
	for(int k = 0; k < num_fixed_door; ++k){
		if(door[k].x == x &&  door[k].y== y){
      return ('@');
    }
	}
	//check if the char is random monsters
	for(int k = 0; k < num_random_monster; ++k){
		if(random_monster[k].x == x &&  random_monster[k].y== y){
      return ('m');
    }
	}
	//check if the char is surprising_box
	for(int k = 0; k < num_fixed_surprising_box; ++k){
		if(surprising_box[k].x == x &&  surprising_box[k].y== y){
      return ('!');
    }
	}
	//check if the char is special door
	for(int k = 0; k < 1; ++k){
		if(special_door[k].x == x && special_door[k].y == y && open_special_door == false){
      return ('$');
    }
	}
	//check if the char is HP_box
	for(int k = 0; k < num_fixed_HP_box; ++k){
		if(HP_box[k].x ==x &&  HP_box[k].y == y){
      return ('H');
    }
	}

	//check if the char is ATK_box
	for(int k = 0; k < num_fixed_ATK_box; ++k){
		if(ATK_box[k].x ==x && ATK_box[k].y==y){
      return ('A');
    }
	}

	//check if the char is DEF_box
	for(int k = 0; k < num_fixed_DEF_box; ++k){
		if(DEF_box[k].x ==x &&DEF_box[k].y==y){
      return ('D');
    }
	}

	//check if the char is keys
	for(int k = 0; k < num_fixed_key; ++k){
		if(key[k].x ==x &&key[k].y == y){
      return ('^');
    }
	}
  return (' ');

}

//function: calculate hp_needed to beat monster '0'
int hp_needed_beat_m(int role_ATK, int role_DEF){
	int hp_needed_beat_m;
	//when role_ATK <= rival_DEF, it means we can not attack the rival
	if(role_ATK <= random_monster[1].DEF){
		hp_needed_beat_m = -1;
	//when role_ATK > rival_DEF, attack results in decreases in hp
	//if rival_ATK <= role_ATK, their attack can not hurt me
	}else if (random_monster[1].ATK <= role_DEF && role_ATK > random_monster[1].DEF){
		hp_needed_beat_m = 0;
	}else if ((random_monster[1].HP % (role_ATK-random_monster[1].DEF)) == 0 && role_ATK > random_monster[1].DEF){
		hp_needed_beat_m = (random_monster[1].ATK-role_DEF)*((random_monster[1].HP/(role_ATK-random_monster[1].DEF))-1);
	}else if ((random_monster[1].HP % (role_ATK-random_monster[1].DEF)) != 0 && role_ATK > random_monster[1].DEF){
		hp_needed_beat_m = (random_monster[1].ATK-role_DEF)*(random_monster[1].HP/(role_ATK-random_monster[1].DEF));
	}
	return hp_needed_beat_m;
}

//function: click 's', save status into .txt file in gamestatus folder
void saving_status(string user_name){
	move(12,0);
	string filename = "game_status/" + user_name + "_floor_2.txt";
	cout << "game status already saved!\ngame over...";
	GameOver = true;
}

// when click 'i' for more info of rivals in Floor 2
void show_info(int* role_attribute){
	int hp_m = hp_needed_beat_m(role_attribute[3], role_attribute[4]);
	//print all information of monsters in Floor 2
	//if we can not attack the monster
	//the attack can not hurt them or hp_needed_kill_them > role_HP
	//just show ???
	move(12,0);
	cout << "Here is some information about rivals in Floor "<<Floor_l2 << ": "<< endl;;
	if (hp_m < 0){
		cout << "random monster 'm': HP:  "<<random_monster[1].HP<<" ATK: "<<random_monster[1].ATK<<" DEF:  "<< random_monster[1].DEF<<" HP_needed: ???"<< endl;
	}else{
		cout << "random monster 'm': HP:  "<<random_monster[1].HP<<" ATK: "<<random_monster[1].ATK<<" DEF:  "<< random_monster[1].DEF<<" HP_needed: "<<hp_m<< endl;
	}
	cout <<"'!' represents treasures. They will help you.\n";

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
		cout << "HP +200!";
	}else if(n == 'A'){
		move(12,0);
		cout << "ATK +5!";
	}else if(n == 'D'){
		move(12,0);
		cout << "DEF +5!";
	}else if(n == '!'){
		move(12,0);
		if(y == 10){
			cout << "This is a diamond.";
			role_attribute[8]++;
		}else{
			cout << "You've got a sword! ATK +10!";
			role_attribute[3] = role_attribute[3]+10;
		}
	}
}

void ask_questions(int * role_attribute,int x, int y){
	int type;
	for (int k = 0; k < num_fixed_monster; ++k){
		if(monster[k].x == x && monster[k].y == y){
			type = monster[k].type;
		}
	}
	cout <<"I have very hard programming questions. \nCome on litter boy!" << endl;
	if(type == 0){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: % can be applied to double. ('t'/'f')\n";
			ans = getch();
			if (ans == 'f'){
				cout << "Great job!\n";
				deleteEntry(monster,num_fixed_monster,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 't'){
				cout << "No, think about it carefully.\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch();
			}else{
				cout << "Please input again: ";
			}
		}
	}else if(type == 1){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: .PHONY targets will be generated even if they are up to date. ('t'/'f')\n";
			ans = getch();
			if (ans == 't'){
				cout << "Great job!\n";
				deleteEntry(monster,num_fixed_monster,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 'f'){
				cout << "No, think about it carefully.\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch();
			}else{
				cout << "Please input again: ";
			}
		}
	}else if(type == 2){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: 7 + rand()%100 generate [7,107]. ('t'/'f')\n";
			ans = getch();
			if (ans == 'f'){
				cout << "Great job!\n";
				deleteEntry(monster,num_fixed_monster,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 't'){
				cout << "No, think about it carefully.\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch();
			}else{
				cout << "Please input again: ";
			}
		}
	}else if(type == 3){
		char ans_2 = 'a';
		char ans;
		while(ans_2 == 'a'){
			cout << "True or False: The arraySize of (char array = \"john\") is 4. ('t'/'f')\n";
			ans = getch();
			if (ans == 'f'){
				cout << "Great job!\n";
				deleteEntry(monster,num_fixed_monster,x,y);
				role_attribute[1]= role_attribute[1]+5;
				role_attribute[2]= role_attribute[2]+5;
				role_attribute[3]= role_attribute[3]+5;
				role_attribute[4]= role_attribute[4]+5;
				role_attribute[5]= role_attribute[5]+5;
				ans_2 = 'b';
			}else if(ans == 't'){
				cout << "No, think about it carefully.\n'a': answer it again or 'b': leave('a'/'b'):\n";
				ans_2 = getch();
			}else{
				cout << "Please input again: ";
			}
		}
	}
	cout << "Press 'p' to continue...\n";
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

Fixed_sth_coordinate * addEntry(Fixed_sth_coordinate * array, int & size, int x, int y){
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

void deleteEntry(Fixed_sth_coordinate * &array, int & size, int x, int y){
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

//setup all positions in this floor
void Setup(){
	GameOver =  false;
	dir = STOP;
	x = 2;
	y = 1;

	//store fixed monsters into array
	//store them into struct
	num_fixed_monster = 0;
	monster = addEntry(monster, num_fixed_monster,6,2);
	monster = addEntry(monster, num_fixed_monster,18,3);
	monster = addEntry(monster, num_fixed_monster,18,8);
	monster = addEntry(monster, num_fixed_monster,6,9);
	for(int i = 0; i < num_fixed_monster; i++){
		monster[i].type = i;
	}

	//store fixed doors and special door into array
	//store them into struct
	special_door[0].x = 11;
	special_door[0].y = 9;
	num_fixed_door = 0;
	door = addEntry(door, num_fixed_door, 7,1);
	door = addEntry(door, num_fixed_door, 7,10);
	door = addEntry(door, num_fixed_door, 18,7);
	door = addEntry(door, num_fixed_door, 18,2);


	num_random_monster = 0;
	random_monster = addEntry(random_monster, num_random_monster, 1, 1+rand()%6);
	random_monster = addEntry(random_monster, num_random_monster, 3+rand()% 16, 6);
	random_monster = addEntry(random_monster, num_random_monster, 2, 3+rand()%5);
	random_monster = addEntry(random_monster, num_random_monster, 8+rand()%2,1+rand()%5);
	for(int i = 0; i < num_random_monster; i++){
		random_monster[i].HP = 50;
		random_monster[i].ATK = 15;
		random_monster[i].DEF = 5;
	}

	//store fixed surprising_box into array
	//store them into struct
	num_fixed_surprising_box = 0;
	surprising_box = addEntry(surprising_box,num_fixed_surprising_box, 18,10);
	surprising_box = addEntry(surprising_box,num_fixed_surprising_box, 11, 3);

	//store fixed HP_box into array
	num_fixed_HP_box = 0;
	HP_box = addEntry(HP_box, num_fixed_HP_box, 4,4);
	HP_box = addEntry(HP_box, num_fixed_HP_box, 5,4);
	HP_box = addEntry(HP_box, num_fixed_HP_box, 5,8);
	HP_box = addEntry(HP_box, num_fixed_HP_box, 6,8);
	HP_box = addEntry(HP_box, num_fixed_HP_box, 12,10);
	HP_box = addEntry(HP_box, num_fixed_HP_box, 14,10);

	//store fixed ATK_box into array
	//store them into struct
	num_fixed_ATK_box = 0;
	ATK_box = addEntry(ATK_box, num_fixed_ATK_box, 4,1);
	ATK_box = addEntry(ATK_box, num_fixed_ATK_box, 15,10);
	ATK_box = addEntry(ATK_box, num_fixed_ATK_box, 17,10);
	//store fixed DEF_box into array
	//store them into struct
	num_fixed_DEF_box = 0;
	DEF_box = addEntry(DEF_box, num_fixed_DEF_box, 11,4);
	DEF_box = addEntry(DEF_box, num_fixed_DEF_box, 12,4);
	DEF_box = addEntry(DEF_box, num_fixed_DEF_box, 13,10);
	DEF_box = addEntry(DEF_box, num_fixed_DEF_box, 16,10);


	//store fixed keys into array
	//store them into struct
	num_fixed_key = 0;
	key = addEntry(key, num_fixed_key, 6,4);
	key = addEntry(key, num_fixed_key, 14,4);
	key = addEntry(key, num_fixed_key, 4,8);
	key = addEntry(key, num_fixed_key, 4,10);

}

//print the map
void Draw(int* role_attribute){
	clear();
	move(0,0);
	//print Floor 2 map walls
	for(int i = 0; i < width; ++i){
		if(i != 2){
			wall[0][i] = true;
			cout << "#";
		}else{
			cout << " ";
		}
	}
	cout << "\n";

	for(int i = 1; i <= height; ++i){
		for (int j = 0; j < width; ++j){
			if(j == 0 || (j == 3 && i != height / 2 +1 )
			|| (i == 5  && j != 1 && j != 2 && j != 8 && j != 9 && j != width - 1)
			|| (i == 7  && j != 1 && j != 2 && j != 8 && j != 9 && j != width - 1 && j!= width - 2)
			|| (j == 7 && i != 1 && i != 5 && i != 6 && i != 7 && i != height)
			|| (j == 10 && i != 1 && i != 5 && i != 6 && i != 7 )
			|| (i == 2 && j == 11) || (i == 2  && j == 12) || (i == 2 && j == 13)
			|| (i == 2 && j == 14) || (i == 2 && j == 15) || (i == 2 && j == 16) || (i == 2 && j == 17)
			|| (i == height - 1 && j == 18) || (i == height - 1  && j == 12) || (i == height - 1 && j == 13)
			|| (i == height - 1 && j == 14) || (i == height - 1 && j == 15) || (i == height - 1 && j == 16) || (i == height - 1 && j == 17)
					){
				wall[i][j]=true;
				cout << "#";
			}else if(j == width - 1){
				wall[i][j] = true;
				cout << "#\n";
			}else{
				cout << " ";
			}
		}
	}

	for(int i = 0; i < width; ++i){
		if (i != 2){
			wall[11][i] = true;
			cout << "#";
		}else{
			cout << " ";
		}
	}

	//print existing fixed monsters
	for(int k = 0; k < num_fixed_monster; ++k){
		move(monster[k].y, monster[k].x);
		cout << "M";
	}
	//print existing doors
	for(int k = 0; k < num_fixed_door; ++k){
		move(door[k].y, door[k].x);
		cout << "@";
	}

	//print exsiting random monsters
	for(int k = 0; k < num_random_monster; ++k){
		move(random_monster[k].y, random_monster[k].x);
		cout << "m";
	}
	//print existing HP_box
	for(int k = 0; k < num_fixed_HP_box; ++k){
		move(HP_box[k].y, HP_box[k].x);
		cout << "H";
	}

	//print existing ATK_box
	for(int k = 0; k < num_fixed_ATK_box; ++k){
		move(ATK_box[k].y, ATK_box[k].x);
		cout << "A";
	}

	//print existing DEF_box
	for(int k = 0; k < num_fixed_DEF_box; ++k){
		move(DEF_box[k].y, DEF_box[k].x);
		cout << "D";
	}

	//print existing surprising_box
	for(int k = 0; k < num_fixed_surprising_box; ++k){
		move(surprising_box[k].y, surprising_box[k].x);
		cout << "!";
	}

	//print existing keys
	for(int k = 0; k < num_fixed_key; ++k){
		move(key[k].y, key[k].x);
		cout << "^";
	}

	//print role position
	move(y,x);
	cout << "c";
	if(open_special_door == false){
		move(special_door[0].y, special_door[0].x);
		cout << "$";
	}
	//print role_attribute
	move(0,25);
	cout << "Floor: "<<Floor_l2;
	move(2,25);
	cout << "Level: "<< role_attribute[0];
	move(3,25);
	cout << "EXP: "<< role_attribute[1];
	move(4,25);
	cout << "HP: "<< role_attribute[2];
	move(5,25);
	cout << "ATK: "<<role_attribute[3];
	move(6,25);
	cout << "DEF: "<< role_attribute[4];
	move(8,25);
	cout << "GOLD: "<<role_attribute[5];
	move(9,25);
	cout << "KEY(door): ^(@) " << role_attribute[6];
	move(10,25);
	cout << "           ?($) " << role_attribute[7];
	move(12,0);

}

//use getch() to read user input prumptly
void input(int* role_attribute, string user_name){
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
	}else if(IsM == true){
		ask_questions(role_attribute,m_x_l2,m_y_l2);
		IsM = false;
		char ch;
		dir = STOP;
		if(getch() == 'p'){
			return;
		}
	}else if(save == true){
		saving_status(user_name);
	}else if(Is_Info == true){
		show_info(role_attribute);
		Is_Info = false;
	}
	//user input
	switch(getch()){
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'q':
			GameOver = true;
			break;
		case '0':
			save = true;
			break;
		case 'i':
			Is_Info = true;
			dir = STOP;
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
void logic_function_1(int x, int y, int &change, int b, bool is_down,bool is_up, int* role_attribute, string user_name){
	//if the cell is wall, then not move
	if(mvinch(x,y) == '#'){

	//if the cell is the door to floor_1, then go to floor_1
	}else if(y == 0 && is_up == true){
		//floor_1();

	//if the cell is the door to floor_2, then go to floor_2
	}else if(y == height+1 && is_down == true){
		floor_3_main(role_attribute, user_name);

	//it is a door
}else if(mvinch(x,y) == '@'){
		if (role_attribute[6] != 0){
			deleteEntry(door, num_fixed_door,x,y);
			role_attribute[6]--;
		}
	//it is a special door
}else if(mvinch(x,y) == '$'){
		if (role_attribute[7] != 0){
			open_special_door = true;
			role_attribute[7]--;
		}
	//it is a key
	}else if(mvinch(x,y) == '^'){
		role_attribute[6]++;
		deleteEntry(key,num_fixed_key,x,y);
	//it is a HP_box, it will increase HP by 200
	}else if(mvinch(x,y) == 'H'){
		deleteEntry(HP_box,num_fixed_HP_box,x,y);
		IsH = true;
		role_attribute[2] = role_attribute[2] + 200;
	//it is a STK_box, it will increase ATK by 5
	}else if(mvinch(x,y) == 'A'){
		deleteEntry(ATK_box,num_fixed_ATK_box,x,y);
		IsA = true;
		role_attribute[3] = role_attribute[3] + 5;
	//it is a DEF_box, it will increase DEF by 5
	}else if(mvinch(x,y) == 'D'){
		deleteEntry(DEF_box,num_fixed_DEF_box,x,y);
		IsD = true;
		role_attribute[4] = role_attribute[4] + 5;
	//it is a surprising_box, it will have sword or diamond
	}else if(mvinch(x,y) == '!'){
		deleteEntry(surprising_box,num_fixed_surprising_box,x,y);
		Is_Sur = true;
	//it is a fixed position monster, with questions
	}else if(mvinch(x,y) == 'M'){
		IsM = true;
		m_x_l2 = x;
		m_y_l2 = y;
	//it is a random_generated_monster, if we can kill it, then we can attack it
	//otherwise, we stay
	}else if(mvinch(x,y) == 'm'){
		int hp_m = hp_needed_beat_m(role_attribute[3], role_attribute[4]);
		if (hp_m >= 0 && hp_m < role_attribute[2]){
			update_attribute(50,15,5,3,5,role_attribute);
			deleteEntry(random_monster, num_random_monster,x,y);
		}else{
			x = x;
		}
	}else{
		change = change + b;
	}
}

//move up and down to generate different reactions
void logic(int* role_attribute, string user_name){
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
