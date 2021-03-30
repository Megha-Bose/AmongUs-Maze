#define Suit 4
#define Glass  3
#define Bag  1
#define Leg  2

#define turn_fract	10
#define	up		0
#define	down	1
#define right	2
#define left	3
#define NO_DIR 4	// cannot go in any direction
#define NOTUP	4	// cannot go up
#define NOTDOWN	5	// cannot go down
#define NOTRIGHT 6	// cannot go right
#define NOTLEFT	7	// cannot go left
#define INIT_TIMEFACTOR 20
#define PI 3.14159265

typedef struct Cell{
	bool is_open;	// has this cell visited for making maze
	bool path[4];	// is each four directions of cells(up, down, right, left) connected to this cell

	Cell() 
	{
		path[4] = { false };
		is_open = false;
	}
} Cell;

class Astro {
public:
	enum DIR { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };
	// constructor
	Astro(int x_position, int y_position , int imposter_chk);
	// destructor
	~Astro() 
	{ 
		delete recur_stk; 
	}

	// check if moving
	bool isMoving() 
	{ 
		return ismoving; 
	}

	// set direction
	void set_dir( DIR new_look_dir );

	// move character
	void move();

	// colour body
	void set_color(double r, double g, double b)	
	{ 
		body_red = r; body_green = g, body_blue = b; 
	}

	// draw character
	void draw();

	// update character
	void updateStatus();

	// get coordinates
	double get_x() { return this->curr_x; }
	double get_y() { return this->curr_y; }

	void set_exit_status() { this->get_exit = true; }

	// stack function for imposter path finding
	bool stkIsEmpty() 
	{ 
		return stk_top == -1; 
	}
	void stkPush(int look_dir) 
	{ 
		recur_stk[++stk_top] = look_dir; 
	}
	int stkPop() 
	{ 
		return (stk_top == -1)? recur_stk[stk_top+1]: recur_stk[stk_top--]; 
	}
	int stkTop() 
	{
		 return recur_stk[stk_top]; 
	}
private:
	// path finding
	double curr_x;
	double curr_y;
	double prev_x;
	double prev_y;
	int look_dir;
	int init_look_dir;
	bool get_exit;
	int imposter_check;
	// animation
	bool ismoving;
	int walking;		// Walking check
	int turning;		// Turning status
	int maze_exited;	// Maze exit status
	double body_red;
	double body_green;
	double body_blue;
	void create_body();	// body components in call list of the character

	// stack for path finding
	int* recur_stk;
	int stk_top;
};

static double bg_red, bg_green, bg_blue;		// background color
static double TOTAL_GAME_TIME = 200.0;

static Cell *cell;
static int screen_width, screen_height;			// the size of maze
static int st_x, st_y;							// start
static int exit_x, exit_y;						// exit
static int imposter_x, imposter_y;						// position of imposter
static int *chosen;								// the pointer of array of connected cells

static int state = 0;							// current state (making maze, playing game or end)
static Astro* imposter = NULL;					// Imposter
static Astro* player = NULL;					// Player
static bool full_view = true;					
static int userInputDir = -1;

static int win = 1;
static int camera_left, camera_right;
static int camera_bottom, camera_top;	        // view points
static int camera_zoom;
static int timefactor;							// duration
static int tasks_left = 2;						// number of tasks left
static int health = 10;							// health of player
static int danger = 0;							// for decreasing health
static int gift = 0;							// for increasing health
const int num_buttons = 2;
const int num_rewards = 5;
const int num_obstacles = 3;

static int buttons[num_buttons][2];
static int rewards[num_rewards][2];
static int obstacles[num_obstacles][2];

// utility functions
static int get_time();
void display_end_message();
void display_end_screen();
void keyFunc( unsigned char key, int x, int y );
void display();
static inline Cell& grid(int x, int y);
void erase_wall( int x, int y, int dest );
void draw_maze();
void gen_maze();
void display_time();
void display();
void game_play();
void specialKeyFunc( int key, int x, int y );
void idle();

static bool rew_obs_released_flag = false;
static bool imposter_killed_flag = false;