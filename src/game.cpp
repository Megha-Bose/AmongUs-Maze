#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <cstring>
#include <cmath>
#include <string> 

#include "headers.h"

using namespace std;

static int get_time()
{
	static time_t init_sec = 0;
	timeval tv;
	gettimeofday(&tv, NULL);
	if (init_sec == 0) 
		init_sec = tv.tv_sec;
	return (tv.tv_sec - init_sec) * 1000 + tv.tv_usec / 1000;
}

void display_end_message()
{
	string string1 = "";
	if(win)
	{
		glRasterPos2f(25, 35);
		string1 = string1 + " Your score: " + to_string(health);
	}
	else
	{
		glRasterPos2f(25, 35);
		string1 = string1 + "Game Over. You lost.";
	}
	int leng = string1.length();
	for (int i = 0; i < leng; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string1[i]);
	}
}

void display_end_screen()
{
	state = 2;
	glClearColor( bg_red, bg_green, bg_blue, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glColor3f( 1 - bg_red, 1 - bg_green, 1 - bg_blue );

	glLoadIdentity();
	
	glColor3f( 1.0, 1.0, 1.0 );

	display_end_message();

	glutSwapBuffers();
}

// the space bar toggles making maze or pause
void keyFunc( unsigned char key, int x, int y )
{
	switch (key) 
	{
		case 'q':
			exit(0);
			break;
	}
}

void display();
void reviewpoint();

static inline Cell& grid(int x, int y) 
{
	return cell[y * screen_width + x];
}

// remove the line between two connected cells
void erase_wall( int x, int y, int dest )
{
	// erasing maze walls (lines) in a direction
	glColor3f( bg_red, bg_green, bg_blue ); // using bg colour
	glBegin( GL_LINES );

	if(dest == up)
	{
		glVertex2f( (x + 1) * 10 + 0.02, (y + 2) * 10 );
		glVertex2f( (x + 2) * 10 - 0.02, (y + 2) * 10 );
	}
	else if(dest == down)
	{
		glVertex2f( (x + 1) * 10 + 0.02, (y + 1) * 10 );
		glVertex2f( (x + 2) * 10 - 0.02, (y + 1) * 10 );
	}
	else if(dest == right)
	{
		glVertex2f( (x + 2) * 10, (y + 1) * 10 + 0.02 );
		glVertex2f( (x + 2) * 10, (y + 2) * 10 - 0.02 );
	}
	else if(dest == left)
	{
		glVertex2f( (x + 1) * 10, (y + 1) * 10 + 0.02 );
		glVertex2f( (x + 1) * 10, (y + 2) * 10 - 0.02 );
	}

	glEnd();
}

// drawing the maze
void draw_maze()
{
	int x, y;

	for( int i = 0 ; i < screen_width * screen_height ; i++ )
	{
		x = i % screen_width;
		y = i / screen_width;
		
		if( cell[i].path[right] == true ) 
			erase_wall( x, y, right );
		if( cell[i].path[up] ==true ) 
			erase_wall( x, y, up );
		if( cell[i].path[down] == true ) 
			erase_wall( x, y, down );
		if( cell[i].path[left] ==true ) 
			erase_wall( x, y, left );
	}
	float radius = 2;
	float twicePI = 2 * PI;
	int num_triangles = 100;
	glColor3f( 0.0, 1.0, 0.0 );
	glBegin( GL_POLYGON );
		// finish circle
		for(int i = 0; i <= num_triangles; i++)
			glVertex2f(15.0 + 10.0 * exit_x + (radius * cos((i) *  twicePI / num_triangles)), 15.0 + 10.0 * exit_y + (radius * sin((i) * twicePI / num_triangles)));
	glEnd();
	
	for(int i = 0; i < num_buttons; i++)
	{
		glColor3f( i, 1.0, 1.0 );
		glBegin( GL_POLYGON );
			// button
			for(int j = 0; j <= num_triangles; j++)
				glVertex2f(15.0 + 10.0 * buttons[i][0] + (radius * cos((j) *  twicePI / num_triangles)), 15.0 + 10.0 * buttons[i][1] + (radius * sin((j) * twicePI / num_triangles)));
		glEnd();
	}

	if(rew_obs_released_flag == true)
	{
		float radius = 2;
		float twicePI = 2 * PI;
		int num_triangles = 100;
		glColor3f( 1.0, 1.0, 0.0 );
		for(int i=0; i < num_rewards; i++)
		{
			glBegin( GL_POLYGON );
				// coins
				for(int j = 0; j <= num_triangles; j++)
					glVertex2f(15.0 + 10.0 * rewards[i][0] + (radius * cos((j) *  twicePI / num_triangles)), 15.0 + 10.0 * rewards[i][1] + (radius * sin((j) * twicePI / num_triangles)));
			glEnd();
		}
		glColor3f( 1.0, 0.0, 0.0 );
		for( int i=0; i < num_obstacles; i++)
		{
			glBegin( GL_POLYGON );
				// bombs
				for(int j = 0; j <= num_triangles; j++)
					glVertex2f(15.0 + 10.0 * obstacles[i][0] + (radius * cos((j) *  twicePI / num_triangles)), 15.0 + 10.0 * obstacles[i][1] + (radius * sin((j) * twicePI / num_triangles)));
			glEnd();
		}
	}
}

void initalise_buttons_rewards_obs()
{
	for(int i = 0; i < num_buttons; i++)
	{
		buttons[i][0] = (rand() % (screen_width - 2)) + 1;
		buttons[i][1] = (rand() % (screen_height - 2)) + 1;
	}

	for(int i = 0; i < num_rewards; i++)
	{
		rewards[i][0] = rand() % (screen_width - 2) + 1;
		rewards[i][1] = rand() % (screen_height - 2) + 1;
	}

	for(int i = 0; i < num_obstacles; i++)
	{
		obstacles[i][0] = rand() % (screen_width - 2) + 1;
		obstacles[i][1] = rand() % (screen_height - 2) + 1;
	}
}

// generate the maze
void gen_maze()
{
	int x, y;						// position of the current cell
	int dest;						// direction of to be connected cell
	static int length = 0;			// visited cells array length
	int currTime, tmp;
	static int oldTime = 0;

	// when all cells visited
	if( length == screen_width * screen_height) 
	{
		state = 1;
		for(int i = 0; i < screen_width * screen_height; i++)
			cell[i].is_open = false;
		return;
	}

	// time
	currTime = get_time();
	if(currTime - oldTime > timefactor * 1)
		oldTime = currTime;
	else return;

	// start
	if( length == 0 )
	{
		// start point and exit
		dest = rand() % 2 + 1;
		if( dest == down )
		{
			// random starting point from top
			st_x = x = rand() % screen_width;
			st_y = y = screen_height - 1;
			grid(x, y).path[up] = true;
			// random exit point on downside
			exit_x = x = rand() % screen_width;
			exit_y = y = 0;
			grid(x, y).path[down] = true;
		}
		else
		{
			// random starting point from right
			st_x = x = screen_width - 1;
			st_y = y = rand() % screen_height;
			grid(x, y).path[right] = true;
			// random exit point on left
			exit_x = x = 0;
			exit_y = y = rand() % screen_height;
			grid(x, y).path[left] = true;
		}

		chosen = new int [screen_height * screen_width];

		// choosing first cell randomly
		x = rand() % screen_width;
		y = rand() % screen_height;
		grid(x, y).is_open = true;
		chosen[0] = screen_width * y + x;	// storing the first visited cell
		length = 1;

		initalise_buttons_rewards_obs();
		imposter_x = rand() % (screen_width - 1) + 1;
		imposter_y = rand() % (screen_height - 1) + 1;
	}

	bool chk_open = false;
	while (!chk_open) 
	{
		tmp = chosen[ rand() % length ];	// randomly choose a cell which is visited
		x = tmp % screen_width;
		y = tmp / screen_width;

		dest = rand() % 4;	

		if(dest == up)
		{
			// don't traverse if at border or already visited 
			if( y == screen_height-1 || grid(x, y + 1).is_open == true)
				continue;

			// marking visited
			grid(x, y + 1).is_open = true;

			// creating link between two cells
			grid(x, y).path[ up ] = true;
			grid(x, y + 1).path[ down ] = true;

			chosen[length] = x + screen_width * ( y + 1 );
			length++;
			chk_open = true;
		}
		else if(dest == down)
		{
			if( y == 0 || grid(x, y - 1).is_open == true )
					continue;

			grid(x, y - 1).is_open = true;

			grid(x, y).path[ down ] = true;
			grid(x, y - 1).path[ up ] = true;

			chosen[length] = x + screen_width * (y - 1);
			length++;
			chk_open = true;
		}
		else if(dest == left)
		{
			if( x == 0 || grid(x - 1,  y).is_open == true )
				continue;

			grid(x - 1,  y).is_open = true;

			grid(x,  y).path[ left ] = true;
			grid(x - 1,  y).path[ right ] = true;

			chosen[length] = screen_width * y + x - 1;
			length++;
			chk_open = true;
		}
		else if(dest == right)
		{
			if( x == screen_width-1 || grid(x + 1, y).is_open == true )
				continue;

			grid(x + 1,  y).is_open = true;

			grid(x,  y).path[ right ] = true;
			grid(x + 1,  y).path[ left ] = true;

			chosen[length] = screen_width * y + x + 1;
			length++;
			chk_open = true;
		}
	}
}

void reshape( int w, int h )
{
	int size = ( screen_width > screen_height )? screen_width : screen_height;
	double move = ( screen_width > screen_height )? ( screen_width-screen_height )/2.0 : ( screen_height-screen_width )/2.0;

	glViewport(0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	if( state == 0 )
	{
		if( screen_width == size )
		{
			camera_left = 0.0;
			camera_right = 20 + size * 10;
			camera_bottom = 0.0 - move * 10;
			camera_top = size*10 + 20 - move * 10;
		}
		else
		{
			camera_left = 0.0 - move * 10;
			camera_right = 20 + size * 10 - move * 10;
			camera_bottom = 0.0;
			camera_top = size * 10 + 20;
		}
	}

	gluOrtho2D( camera_left, camera_right, camera_bottom, camera_top );

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void display_time()
{
	glColor3f( 1.0, 1.0, 1.0 );
	glRasterPos2f(5, 65);
	
	int value = (int)(get_time() / 10 + .5);
    double val = value / 100;
	int rem = 7;
	val = TOTAL_GAME_TIME - val;
	
	auto string = to_string(val);
	
	string = " | Time left: " + string; 
	string = " | Tasks: " + to_string(2 - tasks_left) + " / 2 " + string; 
	string = "Health: " + to_string(health) + " " + string;
	int lenn = string.length();
	for (int i = 0; i < lenn - rem; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[i]);
	}
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ' ');
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 's');
}

void display()
{
	double x;

	glClearColor( bg_red, bg_green, bg_blue, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glColor3f( 1 - bg_red, 1 - bg_green, 1 - bg_blue );

	// default maze
	glLoadIdentity();
	glBegin( GL_LINES );
	for( x = 1 ; x < screen_width+2 ; x++ )
	{
		glVertex2f( x * 10, 10.0 );
		glVertex2f( x * 10, screen_height * 10 + 10.0 );
	}
	for( x = 1 ; x < screen_height+2; x++ )
	{
		glVertex2f( 10.0 , x*10 );
		glVertex2f( screen_width * 10 + 10.0 , x * 10 );
	}
	glEnd();

	draw_maze();
	display_time();

	const double SHIFTFACTOR_X = -10.0;
	const double SHIFTFACTOR_Y = -11.5;

	// draw player
	if(player != NULL) 
	{
		glLoadIdentity();
		glTranslatef(player->get_x() + SHIFTFACTOR_X, player->get_y() + SHIFTFACTOR_Y, 0);
		glScalef(0.1, 0.1, 1);
		player->draw();
	}

	if(imposter != NULL) 
	{
		glLoadIdentity();
		glTranslatef(imposter->get_x() + SHIFTFACTOR_X, imposter->get_y() + SHIFTFACTOR_Y, 0);
		glScalef(0.1, 0.1, 1);
		imposter->draw();
	}

	glutSwapBuffers();
}

void check_reward_collision(int x, int y)
{
	for(int i=0; i<num_rewards; i++)
	{
		if(x == rewards[i][0] && y == rewards[i][1])
		{
			health += 1;
			float radius = 2;
			float twicePI = 2 * PI;
			int num_triangles = 100;
			glColor3f( bg_red, bg_green, bg_blue );
			glBegin( GL_POLYGON );
				// coins
				for(int j = 0; j <= num_triangles; j++)
					glVertex2f(15.0 + 10.0 * rewards[i][0] + (radius * cos((j) *  twicePI / num_triangles)), 15.0 + 10.0 * rewards[i][1] + (radius * sin((j) * twicePI / num_triangles)));
			glEnd();
			rewards[i][0] = -100;
		}
	}
}

void check_obstacle_collision(int x, int y)
{
	for(int i=0; i<num_obstacles; i++)
	{
		if(x == obstacles[i][0] && y == obstacles[i][1])
		{
			health = health - 1;
			float radius = 2;
			float twicePI = 2 * PI;
			int num_triangles = 100;
			glColor3f( bg_red, bg_green, bg_blue );
			glBegin( GL_POLYGON );
				// bombs
				for(int j = 0; j <= num_triangles; j++)
					glVertex2f(15.0 + 10.0 * obstacles[i][0] + (radius * cos((j) *  twicePI / num_triangles)), 15.0 + 10.0 * obstacles[i][1] + (radius * sin((j) * twicePI / num_triangles)));
			glEnd();
			obstacles[i][0] = -100;
		}
	}
}

void game_play()
{
	static Astro finder1(st_x, st_y, 0);
	static Astro finder2(imposter_x, imposter_y, 1);
	static int x1 = st_x;
	static int y1 = st_y;
	static int x2 = imposter_x;
	static int y2 = imposter_y;

	if (player == NULL) 
	{
		player = &finder1;
		finder1.set_color(1.0-bg_red, 1.0-bg_green, 1.0-bg_blue);
	}

	if (imposter == NULL) 
	{
		imposter = &finder2;
		finder2.set_color(1.0-bg_red, 1.0-bg_green, 1.0-bg_blue);
	}

	finder1.updateStatus();
	if(finder1.isMoving())
		return;

	// moving the imposter
	finder2.updateStatus();

	grid(x2, y2).is_open = true;

	// moving the player
	if (userInputDir > -1) 
	{
		if(userInputDir == up) 
		{
			if(grid(x1, y1).path[up] == true && y1 < ::screen_height-1) 
			{
				finder1.set_dir(Astro::UP);
				y1++;
			}
		}
		else if(userInputDir == down)
		{
			if(grid(x1, y1).path[down] == true && y1 > 0) 
			{
				finder1.set_dir(Astro::DOWN);
				y1--;
			}
		}
		else if(userInputDir == right)
		{
			if(grid(x1, y1).path[right] == true && x1 < ::screen_width-1) 
			{
				finder1.set_dir(Astro::RIGHT);
				x1++;
			}
		}
		else if(userInputDir == left)
		{
			if(grid(x1, y1).path[left] == true && x1 > 0) 
			{
				finder1.set_dir(Astro::LEFT);
				x1--;
			}
		}
		if(y1 > y2  && finder2.get_clear() == 0) 
		{
			if(grid(x2, y2).path[up] == true && y2 < ::screen_height-1) 
			{
				finder2.set_dir(Astro::UP);
				y2++;
			}
		}
		else if(y1 < y2  && finder2.get_clear() == 0)
		{
			if(grid(x2, y2).path[down] == true && y2 > 0) 
			{
				finder2.set_dir(Astro::DOWN);
				y2--;
			}
		}
		else if(x1 > x2  && finder2.get_clear() == 0)
		{
			if(grid(x2, y2).path[right] == true && x2 < ::screen_width-1) 
			{
				finder2.set_dir(Astro::RIGHT);
				x2++;
			}
		}
		else if(x1 < x2  && finder2.get_clear() == 0)
		{
			if(grid(x2, y2).path[left] == true && x2 > 0) 
			{
				finder2.set_dir(Astro::LEFT);
				x2--;
			}
		}
		userInputDir = -1;
	}

	for(int i=0; i<num_buttons; i++)
	{
		if(x1 == buttons[i][0] && y1 == buttons[i][1])
		{
			if(i == 0 && rew_obs_released_flag == false)
			{
				rew_obs_released_flag = true;
				tasks_left = max(0, tasks_left - 1);
			}
			else if(i == 1 && imposter_killed_flag == false)
			{
				finder2.set_clear(1);
				imposter_killed_flag = true;   
				tasks_left = max(0, tasks_left - 1);
			}
		}
	}

	if(rew_obs_released_flag == true)
	{
		check_reward_collision(x1, y1);
		check_obstacle_collision(x1, y1);
	}

	if(tasks_left <= 0 && player != NULL && x1 == exit_x && y1 == exit_y)
		state = 2;
	
	if(imposter_killed_flag == false && x1 == x2 && y1 == y2)
	{
		health = health - 2;
		imposter_killed_flag = true;
		finder2.set_clear(1);
		tasks_left = max(0, tasks_left - 1);
	}
}

// view function
void reviewpoint()
{
	double move;
	if(player == NULL) return;

	move = (screen_width - screen_height) / 2.0;
	
	camera_bottom = 0.0 - move * 10;
	camera_top = screen_width  *10 + 20 - move * 10;
	camera_left = 0.0;
	camera_right = 20 + screen_width * 10;
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D( camera_left, camera_right, camera_bottom, camera_top );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// taking input for moving player
void specialKeyFunc( int key, int x, int y )
{
	if(key == GLUT_KEY_RIGHT)
	{
		userInputDir = right;
	}
	else if(key == GLUT_KEY_LEFT)
	{
		userInputDir = left;
	}
	else if(key == GLUT_KEY_DOWN)
	{
		userInputDir = down;
	}
	else if(key == GLUT_KEY_UP)
	{
		userInputDir = up;
	}

	reviewpoint();
	if(state != 2)
		display();
}

// idle function
void idle()
{
	int v = (int)(get_time() / 1000 + .5);
	v = (int) TOTAL_GAME_TIME - v;
	if(v <= 0 || health <= 0)
	{
		win = 0;
		display_end_screen();
	}
	if(state == 0) 
		gen_maze();
	else if(state == 1)
	{
		game_play();
		reviewpoint();
	}
	else if(state == 2)
		display_end_screen();

	if(state != 2)
		display();
}

int main()
{
	srand( ( unsigned )time( NULL ) );
	screen_width = screen_height = 5;

	// instructions
	cout << endl;
	cout << "Q key : quit\n";
	cout << "Arrow key : move the character\n";

	cell = new Cell[screen_width * screen_height];

	// background color
	bg_red = 0.0;
	bg_green = 0.0;
	bg_blue = 0.0;

	timefactor = INIT_TIMEFACTOR;

	camera_zoom = 20;

	char *myargv[1];
	int myargc = 1;
	myargv[0] = strdup("Among Us Maze");
	glutInit(&myargc, myargv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize ( 500, 500 );
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Among Us Maze");

	glutIdleFunc( idle );
	glutSpecialFunc( specialKeyFunc );
	glutKeyboardFunc( keyFunc );
	glutReshapeFunc( reshape );
	glutDisplayFunc( display );

	glutMainLoop();

	return 0;
}
