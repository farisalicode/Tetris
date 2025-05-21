
//Name: Faris Ali, Roll Number: 22I-0804, Assignment: Project 01



/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */

//---Piece Starts to Fall When Game Starts---//

#include<fstream>

void fallingPiece(double& timer, double& delay, long int& colorNum,int& bomb,long int& n,int& l)
{
 if (timer>delay)
 {
  for (int i=0;i<4;i++)
  {
   point_2[i][0]=point_1[i][0];
   point_2[i][1]=point_1[i][1];
   point_1[i][1]+=1;
  }

  if (!anamoly())
  {
 
 //Randomly pick an index of array BLOCKS .
   n = rand()%8;

   if (bomb==0)
   {
    for(int i = 0;i<4;i++)
     {
    //If anamoly found, add the piece to the grid.
    //The backup coordinates are used to colour particular blocks of the grid.
      gameGrid [point_2 [i] [1]] [point_2 [i] [0]] = colorNum;
     }
    }
  
   //Assigning random coloured tiles.
     colorNum = 1 + rand()%7;
   //Helps the blocks fall from random cooridnates at the start.
     int rxcor = rand()%9;            

//Current falling piece is selected by generating coordinates from BLOCKS array random row.  
     
      for (int i=0;i<4;i++)
      {
      point_1[i][0] = ((BLOCKS[n][i] % 2)+rxcor);
      point_1[i][1] = (BLOCKS[n][i] / 2);
      }
     
      if (n==7)
      bomb = 1;
      else if (n!=7)
      bomb = 0;
            
   }
   timer=0;
  }
}

   ////////////////////////////////////////////////////////////////////////

void moveblock(long int& delta_x)
{           
 for (int i = 0;i<4;i++)
 {
//Assign current coordinates to backup in case of anamoly.
  point_2 [i] [0] = point_1 [i] [0];
  point_2 [i] [1] = point_1 [i] [1];
//Add value of delta_x to the current x-coordinates.
  point_1 [i] [0] = point_1 [i] [0] + delta_x;
 }
 if (!anamoly())
 {
  for(long int i = 0;i<4;i++)
  {
   for(long int j = 0;j<2;j++)
   {
  //If anaomaly is found, backup coordinates are assigned to current ones.
    point_1 [i] [j] = point_2 [i] [j];
   }
  }
 }
}

void rotateblock()
{
//Rotation happens about a fixed point.
//The x and y coordinates around the fixed point get switched.
//In this case the second point of each piece is a fixed one.
//New x = fixed x - (previous y - fix y).
//New y = fixed y + (previous x - fix x).
 float fix [2];
 fix [0] = point_1 [1] [0];
 fix [1] = point_1 [1] [1];
 for(int i = 0;i<4;i++)
 {
//Temporary storage of one part of the formula.
  long int xinc = point_1 [i] [1] - fix[1];
  long int yinc = point_1 [i] [0] - fix[0];
//Difference between the temporary and fixed part used as current coordinates.
  point_1 [i] [0] = fix [0] - xinc;
  point_1 [i] [1] = fix [1] + yinc;
 } 
//Backup in case of anaomally. 
 if (!anamoly())
 {
  for(long int i = 0;i<4;i++)
  {
   for(long int j = 0;j<2;j++)
   {
    point_1 [i] [j] = point_2 [i] [j];
   }
  }
 } 
}

void linedestroy(long int& checker)
{
//Checker helps start checking from last row of gameGrid array.
//Outer loop checks all rows, starting from checker till top.
 for(int i = M-1;i>0;i--)
 {

//It is used to check how many elements of grid are coloured.
 double counter;
//Counter is initallised for each seprate row.
 counter = 0;

//Inner loop checks columns of grid.
  for(int j = 0;j<N;j++)
  {
 //Condition to check if grid element is coloured or not.
   if (gameGrid[i][j]!=0)
 //If coloured, means block is present, increment counter.
   counter = counter + 1;
   gameGrid [checker] [j] = gameGrid [i] [j];
  }
//All elements of row are not coloured.
 if (counter<N)
//If row is not full, checker and i value continue to be equal.
 checker = checker - 1;
 }
}

void checkbomb(bool& bombgrid,long int& colorNum)
{
//This condition checks if a grid block below the bomb has the same colour or not.
 if (gameGrid [point_1 [3][1]+1] [point_1 [3][0]] == colorNum)
 {
  bombgrid = true;
 }
}

void bombeffectone()
{
 for (int i = 0;i<M;i++)
 {
  for (int j = 0;j<N;j++)
  {
//If grid block and bomb color match, all grid blocks become empty.
   gameGrid [i] [j] = 0;
  }
 }
//An anamoly is created on purpose so that the bomb disappears.
 point_1[3][1] = 42;
}

void bombeffecttwo()
{
 if (gameGrid [point_1 [3][1]+1] [point_1 [3][0]] > 0)
 {
//The square coordinates below the bomb are all cleared.
  gameGrid [point_1 [3][1]+1] [point_1 [3][0]] = 0;
  gameGrid [point_1 [3][1]+1] [point_1 [3][0]+1] = 0;
  gameGrid [point_1 [3][1]+2] [point_1 [3][0]] = 0;
  gameGrid [point_1 [3][1]+2] [point_1 [3][0]+1] = 0;
//Anamoly created on purpose so that the bomb disappears.
  point_1[3][1] = 42;
 }
}

void lastline(long int& line)
{
//Check if the last line of the grid is filled or not.
 for (int j = 0;j<N;j++)
 {
//If any element of the 0th row of grid is filled, the game is over. 
  if (gameGrid [0] [j] > 0)
  {
   line = -1;
   break;
  } 
 }
}

void sumscore(int& cleared_rows,int& score,int& l)
{
//Score is updated according to the level and the lines cleared
 if (cleared_rows==0)
 score = (score + (0*l));
 else if (cleared_rows==1)
 score = (score + (10*l));
 else if (cleared_rows==2)
 score = (score + (30*l));
 else if (cleared_rows==3)
 score = (score + (60*l));
 else if (cleared_rows==4)
 score = (score + (100*l));
}

void writescores(long int highscores [5],int& score)
{
//If the score is greater than previous high scores, it replaces them.
 int top = -1;
 for (int i = 0;i<5;i++)
 {
//Check if the current score is greater then any of the current scores.
  if (score>highscores[i])
  top = i;
 }
//Score is stored at the appropriate index.
 if (top != -1)
 highscores [top] = score;
}

void writetofile(long int highscores [5])
{
//Writing the scores array to a text file for permanent storage.
 std::fstream file;
 file.open("scoring.txt");
 file << "Top 5 High Scores" << std::endl;
 for (int i = 0;i<5;i++)
 {
  file << highscores [i] << " Points"<< std::endl;
 }
 file.close();
}

   //////////////////////////////////////////////////////////////////////
