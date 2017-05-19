/*********************************************
 *                                           *
 *        CS 50 - Problem Set 3              *
 *        Student: Mu'awiyah Namadi          *
 *                                           *
 *        Description:                       *
 *        Block breaker Game.                *
 *                                           *
 *********************************************/
 
// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 10

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddle
#define PADDLE_WIDTH 70
#define PADDLE_HEIGHT 10

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

// number of lives & points to start with
int lives = 3;
int points = 0;

//start main program
int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;


    double drand48(void);
    
    // ball speed of left and right movement 
    double vertical = 2.0;   
    double horizontal = 2 * drand48();
    
    
    //wait for players click to start the game
    waitForClick();
    
    // keep playing until game over at 50 points (same # as bricks)
    while (lives > 0 && points < 50)
    {
         //move the ball
         move(ball, horizontal, vertical);
         
         // detects ball collision with the window
         GObject object = detectCollision(window, ball);
         
         // detect ball collision with paddle
         if (object == paddle) 
         {
            vertical = -vertical;
         }
         
         if (object != NULL) 
         {
             // detect ball collision with bricks
            if (strcmp(getType(object), "GRect") == 0 && object != paddle)
            {
                removeGWindow(window, object);
                points++;
                updateScoreboard(window, label, points);
                vertical = -vertical;
            }
            // detect ball collision with scoreboard
            else if (strcmp(getType(object), "GLabel") == 0)
            {
                continue;
            }
         }
        // bounce off top
        else if (getY(ball) <= 0 )
        {
            vertical = -vertical;
        }
        // if user misses ball with paddle & ball falls through bottom
        else if (getY(ball) > HEIGHT) 
        {
            --lives;
            main();
        }
         // bounce off right and left respectively
        else if (getX(ball) + getWidth(ball) >= getWidth(window) || getX(ball) <= 0 )
        {
            horizontal = -horizontal;
        }
       
       
        // linger before moving again
        pause(10);

        
        // move paddle w/ mouse
        GEvent event = getNextEvent(MOUSE_EVENT);

        // if we heard one
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure circle follows top cursor
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, HEIGHT/1.1);
            }
        }
    }

    // send game over message
    GLabel glabel = newGLabel("Game Over!!!");
    setFont(glabel, "Fantasy-30");
    setColor(glabel, "RED");
    setLocation(glabel, WIDTH/2-100, HEIGHT/2-10);
    add(window, glabel);
    
    // wait for click before exiting
    waitForClick();
    
    // game over
    closeGWindow(window);
    
    // end program
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
        
        int brick_x = 2;
        int brick_y = 40;
        
        string colors[10];
        colors[0] = "RED";
        colors[1] = "ORANGE";
        colors[2] = "YELLOW";
        colors[3] = "GREEN";
        colors[4] = "BLUE";
        colors[5] = "RED";
        colors[6] = "ORANGE";
        colors[7] = "YELLOW";
        colors[8] = "GREEN";
        colors[9] = "BLUE";
    
    for (int row = 0; row < ROWS; row++) 
    {
        for (int col = 0; col < COLS; col++, brick_x+=40) 
        {
            GRect brick = newGRect(brick_x,brick_y,WIDTH/11,10);
            setFilled(brick, true);
            setColor(brick, colors[row]);
            add(window, brick);
        }
        // reset x position for each row
        brick_y += 20;
        brick_x = 2;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
   int ball_size = 20;
   int ball_x = WIDTH/2 - 10;
   int ball_y = HEIGHT - 85;
   GOval ball = newGOval(ball_x, ball_y, ball_size, ball_size); //xywh
   setFilled(ball, true);
   setColor(ball, "BLACK");
   add(window, ball);
   return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // centers paddle and places it mostly towards the bottom
    int paddle_x = WIDTH/2 - (PADDLE_WIDTH/2);
    int paddle_y = HEIGHT/1.1 - (PADDLE_HEIGHT/2);
    GRect rect = newGRect(paddle_x,paddle_y,PADDLE_WIDTH,PADDLE_HEIGHT);
    setFilled(rect, true);
    setColor(rect, "BLACK");
    add(window, rect);
    return rect;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-18");
    setLocation(label, 25, 25);
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
