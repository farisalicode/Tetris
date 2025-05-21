
//Name: Faris Ali, Roll Number: 22I-0804, Assignment: Project 01



/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You don't need to change anything in the driver code/main function except the part(s) where instructed.
 * You are expected to define your functionalities in the "functionality.h" header and call them here in the instructed part(s).
 * The game is started with a small box, you need to create other possible in "pieces.h" file in form of array.
    E.g., Syntax for 2 pieces, each having four coordinates -> int Pieces[2][4];
 * Load the images into the textures objects from img directory.
 * Don't make any changes to the "utils.h" header file except for changing the coordinate of screen or global variables of the game.
 * SUGGESTION: If you want to make changes in "utils.h", do it once you complete rest of the functionalities.
 * TIP: You can study SFML for drawing scenes, sprites and window.
 * */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"
using namespace sf;
int main(){
    srand(time(0));
    RenderWindow window(VideoMode(320, 480), title);
    Texture obj1, obj2, obj3, obj4;
    
 
    obj1.loadFromFile("img/tiles.png");
    obj2.loadFromFile("img/background.png");
    obj3.loadFromFile("img/frame.png");
    obj4.loadFromFile("img/gameover.png");//The background when the player loses.
    Sprite sprite(obj1), background(obj2), frame(obj3), gameover(obj4);
    
    //These lines play the background music, using the required libraries.
    Music music;
    if (!music.openFromFile("music.ogg"))
    return -1;
    music.setVolume(100);
    music.setLoop(true);
    music.play();  
    
    long int delta_x=0, colorNum=0;
    double timer=0, delay=0.8;
    bool rotate=0;
    int l = 1;//The level of the game.
    long int n;//Variable to randomly select which piece will arrive next.
    int bomb;//Variable to detect presence of bomb.
    long int go = 0;//Variable used to see if game over or not.
    bool pause = false;//Variable to help pause the game
    int score = 0;//Variable to count the score.
    long int line;//Variable to keep track of what line of blocks is completed.
    long int highscores [5];//Array to store the 5 highest scores.
    Clock clock;
    while (window.isOpen()){
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;
        //---Event Listening Part---//
        Event e;
        while (window.pollEvent(e)){                    //Event is occurring - until the game is in running state
            if (e.type == Event::Closed)                   //If cross/close is clicked/pressed
                window.close();                            //Opened window disposes
            if (e.type == Event::KeyPressed) {             //If any other key (not cross) is pressed
                 if (e.key.code == Keyboard::Up)            //Check if the other key pressed is UP key
                    rotate = true;                         //Rotation gets on
                 else if (e.key.code == Keyboard::Left)     //Check if the other key pressed is LEFT key
                    delta_x = -1;                          //Change in X-Axis - Negative
                 else if (e.key.code == Keyboard::Right)    //Check if the other key pressed is RIGHT key
                    delta_x = 1;                           //Change in X-Axis - Positive         
               //Keyboard command to help pause and resume the game.
                 else if (e.key.code == Keyboard::H)
                 pause = !pause;
               //Used to increase the level of the game.
                 else if (e.key.code == Keyboard::U)
                 l = 2;
               //Used to decrease the level of the game.
                 else if (e.key.code == Keyboard::D)
                 l = 1;            }
        }
   
//Selecting more difficult level increases the speed with which piece falls.
        if (l==2)
        delay = 0.2;
        if (l==1)
        delay = 0.4;
       
     //Pressing of the Down key makes the piece fall quicker.
       if ((Keyboard::isKeyPressed(Keyboard::Down))&&(bomb!=1))
           {
            int temp1 = l;
            l = 1;
            delay=0.05;
            l = temp1;
           }
  
      //Pressing of the Space key makes the piece fall immediately.
        else if (Keyboard::isKeyPressed(Keyboard::Space))
           {
            int temp2 = l; 
            l = 1;
            delay = 0.006;
            l = temp2;
           }
     
     /////////////////////////////////////////////////////////////////////

if (pause==false)//Condition to pause and unpause the game.
{
 
 if (go==0)//Condition to check if player has lost game or not.
 {

 //Function to check if the top of grid has been reached.
   lastline(line);
    
  //If the top of the grid is reached, then the game is over.
    if (line==-1)
     go = 1;
   
 //Vertical movement of piece.
   fallingPiece(timer, delay, colorNum,bomb,n,l);
      
 //Condition to restirct the movement of the bomb.
   if (bomb==0)
   {
  //Horizontal movement of piece is controlled by this function.
    moveblock(delta_x);
   }
 //No Rotation of bomb. 
   if (bomb==0)
   {
  //No rotation of square or O piece.
    if (rotate&&n!=6)
    {
   //Rotation of pieces using a fixed point.
     rotateblock();
    }
   }
  
//Variable to check if a particular grid line is filled or not. 
  long int checker = M -1;
  
//Uses above given variable to clear a particular number of lines.
  linedestroy(checker);
  
//Check how many lines were cleared.
  int cleared_rows = M - checker;
 
//Increase score if lines are cleared, also depends upon the level of the game.
  sumscore(cleared_rows,score,l);
  
//Variable to see if bomb has touched same color piece.
  bool bombgrid = false;
  
  if (bomb==1)
  {
 //Function to see if bomb has touched same colour or not.
   checkbomb(bombgrid,colorNum);
  }
  
  if (bombgrid==true)
  {
 //Destroy all blocks on grid.
   bombeffectone();
  }
  
  if (bomb==1&&bombgrid==false)
  {
 //Destroy only 4 blocks if colours do not match.
   bombeffecttwo();
  }
  }
 
 else if (go==1)
 {
//Transfers current score to highscores array.
  writescores(highscores,score);

//Transfers current score to a text file.
  writetofile(highscores);
  }
}
        
    ////////////////////////////////////////////////////////////////////////

        delta_x = 0; rotate = 0; delay = 0.8;
        window.clear(Color::Black);
        //The below code only executes if the game is not over.
        if (go==0)
        { 
        window.draw(background);
        
        for (int i=0; i<M; i++){
            for (int j=0; j<N; j++){
                if (gameGrid[i][j]==0)
                    continue;
                sprite.setTextureRect(IntRect(gameGrid[i][j]*18,0,18,18));
                sprite.setPosition(j*18,i*18);
                sprite.move(28,31); //offset
                window.draw(sprite);
            }
        }
        
        for (int i=0; i<4; i++){
            sprite.setTextureRect(IntRect(colorNum*18,0,18,18));
            sprite.setPosition(point_1[i][0]*18,point_1[i][1]*18);
            sprite.move(28,31);
            window.draw(sprite);
          }
        //---The Final on Which Everything is Drawn Over is Loaded---//
        window.draw(frame);
        }
        //Display gameover background.
        else if (go==1)
        {
        window.draw(gameover);
        }
        //---The Window that now Contains the Frame is Displayed---//
        window.display();
    }
    return 0;
}
