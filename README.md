### comp-2113-project text_based_game 
## Project Name: 
  Tower of the Programmer
## Project Producer: 
   Group 201  
   Chen Kangyi (Kevin) 3035447776   
   Wang Yun (Carrie) 3035604180
## Project description: 
1. Background  
Welcome to the "Tower of the programmer"! You are a novice programmer who are especially interested in writing codes. And the most experienced and prestigous coding king in the world -- Dr. L, is trapped in one of the most dangerous places in this world, Tower of the programmer. Your task is to rescue Dr. L. Certainly, you have to sharpen your programing skills on the way of this adventure so as to achieve the highest level of programming and reach the top of the tower, where you can finally rescue Dr. L. Why would Dr. L be trapped in the tower? Who would you encounter in the tower? They are all left for you to discover. What are you waiting for? Come and join this wonderful adventure! 
2. Basic game rules  
 * Maximum number of player: 1  
 * Basic operation: Player can use 'W', 'A', 'S', 'D' to control the character; 'ESC' to exit the game; 'S' to save the current game status; Other special operations, such as clicking 'i' for more information of rivals, will be instructed in the game. 
 * Character attribute:   
    ATK -- attack  
    DEF -- defense  
    HP -- health point  
    Level -- an attribute which direcrly influences ATK, DEF and HP  
    EXP -- experience rangeing from 0 to 99. When EXP exceeds 99, you will automatically level up.  
 * Equipment   
    Gold -- purchase  
    Keys(@, $, %) -- open the door in each level of the tower  
 * Battle rule: It is a turn based battle. For each turn, you will suffer a decrease in HP determined by (your rival's ATK - your DEF). Your rival will suffer a decrease in HP determined by (your ATK - your rival's DEF). When one's HP decreases below or equal to 0, the battle ends.
 * Special rules: Your EXP and gold will increase by winning battles. You can collect, sell or buy special items to increase your ATK, DEF, HP and EXP. There are three keys: @, $ and % to open the door of corresponding symbols and the number of keys you possess will appear on your screen.  
 * Enjoy this game and try to be a good programmer!  

3. List of features
   1. Apart from some big bosses, other rivals are generated randomly for each game.
   2. For the map, we will use a 2-dimension array to store the status of each point, e.g. rival, boss, door, wall, special item, etc. For the status of the player, we will use a 1-dimension array to store the attribute of the character plus the location of the character including the level of the tower and x/y-coordinate.
   3. For each battle or other special situations, we will use dynamic memory management to optimize the efficiency of memory usage. For example, in each battle, we will need one more array to record the real-time attribute of the rival, and the memory for this array will be released after the battle ends.
   4. Once the player decided to save the game status and leave, we will output a text file called "TofP_status.txt" to record the player's current status. If the program does not detect such text file named "TofP_status.txt", it will set level 1 as the default input, otherwise, it will input the file and load corresponding data. Remember to delete or rename this file if you want to have a new game.
   5. Since each level of the tower is a bit different, we will use 4 different files to store different levels, and the player can simply execute our main program to get into the game.

