#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include <string>
/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void split_bubble(vector<Bubble> &bubbles , int i ,int factor ,Color color){  //splits bubble into two bubbles
    double x,y;                                                  //with velocities of x in opposite direction
    x = bubbles[i].get_center_x();                               //and with initial velocity of y
    y = bubbles[i].get_center_y();
    bubbles.erase(bubbles.begin()+i);
    bubbles.push_back(Bubble(x, y, factor*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, -90, BUBBLE_DEFAULT_AY, color));
    bubbles.push_back(Bubble(x, y, factor*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, -90, BUBBLE_DEFAULT_AY, color));

}

void move_bubbles(vector<Bubble> &bubbles , vector<Bullet> &bullets , Color color){
    // move all bubbles
    for(unsigned int i=0; i < bubbles.size(); i++){
      for(unsigned int j=0; j < bullets.size(); j++){
        double cx ,cy ,rc ,bx ,by ,rb;
        cx = bubbles[i].get_center_x();           // bullet is approximated as a circle with radius as height/2
        cy = bubbles[i].get_center_y();           // cx , cy ,rc are coordinates and radius of bubble
        rc = bubbles[i].get_radius();             // bx , by ,rb are coordinates and approx. radius of bullet
        bx = bullets[j].get_center_x();
        by = bullets[j].get_center_y();
        rb = bullets[j].get_height()/2.0;

        if((sqrt((cx-bx)*(cx-bx)+(cy-by)*(cy-by)) <= rc + rb)&&    //condition for collision of bullet and bubble of default radius
        (bubbles[i].get_radius()==BUBBLE_DEFAULT_RADIUS)){
           bubbles.erase(bubbles.begin()+i);                        //collided bubble disapppears
           bullets.erase(bullets.begin()+j);                        //collided bullet disappears
        }

        else if((sqrt((cx-bx)*(cx-bx)+(cy-by)*(cy-by)) <= rc + rb)&&    //condition for collision of bullet and bubble of radius double of default
        (bubbles[i].get_radius()==2*BUBBLE_DEFAULT_RADIUS)){
           split_bubble(bubbles,i,1,color);                         //collided bubble splits
           bullets.erase(bullets.begin()+j);                        //collided bullet disappears
        }

        else if((sqrt((cx-bx)*(cx-bx)+(cy-by)*(cy-by)) <= rc + rb)&&    //condition for collision of bullet and bubble of radius 4 times of default
        (bubbles[i].get_radius()==4*BUBBLE_DEFAULT_RADIUS)){
           split_bubble(bubbles,i,2,color);                         //collided bubble splits
           bullets.erase(bullets.begin()+j);                        //collided bullet disappears
        }

      }
    }

    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);                            //move bubble
    }
}

vector<Bubble> create_bubbles_level_1()
{
    // create initial bubbles in the game for level 1
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(255,255,0)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(255,255,0)));
    return bubbles;
}

vector<Bubble> create_bubbles_level_2()
{
    // create initial bubbles in the game for level 2
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(LEFT_MARGIN , 70 , BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(255,110,0)));
    bubbles.push_back(Bubble(WINDOW_X/3.0 , 70 , BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(255,110,0)));
    return bubbles;
}

vector<Bubble> create_bubbles_level_3()
{
    // create initial bubbles in the game for level 3
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(LEFT_MARGIN , 70 , BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(4,108,0)));
    bubbles.push_back(Bubble(WINDOW_X-LEFT_MARGIN , 70 , BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY, COLOR(4,108,0)));
    return bubbles;
}

bool check_loss(vector<Bubble> &bubbles , Shooter &shooter){
    for(unsigned int i=0; i< bubbles.size(); i++){
       double cx ,cy ,rc , hx, hy, rh, b1x , b1y ,rb1 , b2x , b2y ,rb2 ;
       cx = bubbles[i].get_center_x();               //cx , cy , rc are coordinates and radius of bubble
       cy = bubbles[i].get_center_y();               //hx , hy ,rh are coordinates and radius of head of shooter
       rc = bubbles[i].get_radius();                 // b1 and b2 represent two circles  which represent body of shooter
       hx = shooter.get_head_center_x();
       hy = shooter.get_head_center_y();
       rh = shooter.get_head_radius();
       b1x= shooter.get_body_center_x();
       b1y= shooter.get_body_center_y()-(shooter.get_body_height()/2.0)+(shooter.get_body_width()/2.0);
       rb1= shooter.get_body_width()/2.0;
       b2x= shooter.get_body_center_x();
       b2y= shooter.get_body_center_y()-(shooter.get_body_height()/2.0)+(3.0*shooter.get_body_width()/2.0);
       rb2= shooter.get_body_width()/2.0;

       if((sqrt((cx-hx)*(cx-hx)+(cy-hy)*(cy-hy)) <= rc + rh)       //condition for bubble collision with shooter
        ||(sqrt((cx-b1x)*(cx-b1x)+(cy-b1y)*(cy-b1y)) <= rc + rb1)  //i.e. intersection of bubble with head or body
        ||(sqrt((cx-b2x)*(cx-b2x)+(cy-b2y)*(cy-b2y)) <= rc + rb2))
        {
            return 1;
        }

    }
    return 0;
}

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);                //creates game window

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    string level("Level: 1/3");
    Text Level(WINDOW_X/2.0 , BOTTOM_MARGIN , level);                //always display current level

  {
    Text level_1(WINDOW_X/2.0,WINDOW_Y/2.0,"Level 1");               //display user the start of level 1
    wait(2.5);
  }

  {   //braces for level 1
    //Initialise the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX,30 ,18 ,8 ,COLOR(0,128,255) );

    // Initialize the bubbles
    vector<Bubble> bubbles1 = create_bubbles_level_1();

    //set color used for bubbles of level 1
    Color color = COLOR(255,255,0);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop for level 1
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        if(bubbles1.size()==0)break;           //signifies completion of level 1

        // Update the bubbles
        move_bubbles(bubbles1,bullets,color);

        // Update the bullets
        move_bullets(bullets);

        if(check_loss(bubbles1,shooter)){                                     //check if user loses the game
            Text game_over(WINDOW_X/2.0 , WINDOW_Y/2.0 , "Game Over!");       // display "Game Over!"
            wait (2.5);
            return 0;
        }

        wait(STEP_TIME);

    }

  }

  {
    Text level_2(WINDOW_X/2.0,WINDOW_Y/2.0,"Level 2");              //display user the start of level 2
    level[level.length()-3]='2';                                    //update cuurent level
    Level.setMessage(level);
    wait(2.5);
  }

  {   //braces for level 2
    //Initialise the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX,30 ,18 ,8 ,COLOR(0,0,255) );

    // Initialize the bubbles
    vector<Bubble> bubbles2 = create_bubbles_level_2();

    //set color used for bubbles of level 2
    Color color = COLOR(255,110,0);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop for level 2
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        if(bubbles2.size()==0)break;           //signifies completion of level 2

        // Update the bubbles
        move_bubbles(bubbles2,bullets,color);

        // Update the bullets
        move_bullets(bullets);

        if(check_loss(bubbles2,shooter)){                                      //check if user loses the game
            Text game_over(WINDOW_X/2.0 , WINDOW_Y/2.0 , "Game Over!");        // display "Game Over!"
            wait (2.5);
            return 0;
        }

        wait(STEP_TIME);

    }

  }

  {
    Text level_3(WINDOW_X/2.0,WINDOW_Y/2.0,"Level 3");       //display user the start of level 1
    level[level.length()-3]='3';                             //update cuurent level
    Level.setMessage(level);
    wait(2.5);
  }

  {   //braces for level 2
    //Initialise the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX,30 ,18 ,8 ,COLOR(0,0,153) );

    // Initialize the bubbles
    vector<Bubble> bubbles3 = create_bubbles_level_3();

    //set color used for bubbles of level 3
    Color color = COLOR(4,108,0);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop for level 2
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        if(bubbles3.size()==0)break;           //signifies completion of level 3

        // Update the bubbles
        move_bubbles(bubbles3,bullets,color);

        // Update the bullets
        move_bullets(bullets);

        if(check_loss(bubbles3,shooter)){                                     //check if user loses the game
            Text game_over(WINDOW_X/2.0 , WINDOW_Y/2.0 , "Game Over!");       // display "Game Over!"
            wait (2.5);
            return 0;
        }

        wait(STEP_TIME);

    }

  }

  {
    Text win(WINDOW_X/2.0, WINDOW_Y/2.0, "You Win");      //display "You WIn" , i.e. signifies completion of game
    wait(2.5);
    return 0;
  }



}
