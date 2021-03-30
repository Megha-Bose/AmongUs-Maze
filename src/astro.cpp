#include <GL/gl.h>
#include <GL/glut.h>

#include <cmath>
#include <cstdlib>
#include "headers.h"

double step_size;

// function to create body components
void Astro::create_body()
{
	// creating bag
	glNewList( Bag, GL_COMPILE );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( -5, 20 );
			glVertex2f( 13, 20 );
			glVertex2f( 13, -15 );
			glVertex2f( -5, -15 );
		glEnd();
	glEndList();

	// creating legs
	glNewList( Leg, GL_COMPILE );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( -5, 0 );
			glVertex2f( 8, 0 );
			glVertex2f( 8, -20.5 );
			glVertex2f( -5, -20.5 );
		glEnd();
	glEndList();

	// creating glass
	glNewList( Glass, GL_COMPILE );
		glColor3f( 0.6, 0.6, 0.6 );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( -5, 10 );
			glVertex2f( 25, 10 );
			glVertex2f( 25, -7 );
			glVertex2f( -5, -7 );
		glEnd();
		if(this->imposter_check)
			glColor3f( 0.8, 0, 0 );
		else
			glColor3f( 0, 0, 0.8 );
	glEndList();

	// creating the body suit -> rectangle + semicircle
	glNewList( Suit, GL_COMPILE );
		float radius = 20;
    	float twicePI = 2 * PI;
		float cx = 20, cy = 40;
		int num_triangles = 100;
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( 0, 0 );
			glVertex2f( 10, 40 );
			glVertex2f( 30, 40 );
			// semicircle
			for(int i = 0; i <= num_triangles/2; i++)
        		glVertex2f(cx + (radius * cos((i) *  twicePI / num_triangles)), cy + (radius * sin((i) * twicePI / num_triangles)));
			glVertex2f( 40, 40 );
			glVertex2f( 40, 0 );
		glEnd();
	glEndList();

}

Astro::Astro(int start_x, int start_y, int imposter_chk)
{
	this->ismoving = false;
	this->maze_exited = 0;
	this->imposter_check = imposter_chk;
	this->clear_check = 0;

	this->walking = 0;
	this->turning = 0;
	this->init_look_dir = look_dir = RIGHT;

	this->prev_x = this->curr_x = 20.0 + 10.0 * start_x;
	this->prev_y = this->curr_y = 20.0 + 10.0 * start_y;

	create_body();
}

// draw body
void Astro::draw()
{
	if(this->clear_check == 0)
	{
		glTranslatef( 30, 50, 0 );
		double rotateAngle = 0;
		
		if(this->init_look_dir == LEFT) 
			rotateAngle = 180.0;
		else if(this->init_look_dir == UP)
			rotateAngle = 90.0;
		else if(this->init_look_dir == DOWN)
			rotateAngle = -90.0;

		// act of turning
		if( this->turning < turn_fract )
		{
			double angle;
			if(this->init_look_dir == LEFT) 
			{
				if( this->look_dir == RIGHT )
					angle = 180.0;
				else if( this->look_dir == UP )
					angle = -90.0;
				else if( this->look_dir == DOWN )
					angle = 90.0;
			}
			else if(this->init_look_dir == RIGHT)
			{
				if( this->look_dir == LEFT )
						angle = 180.0;
				else if( this->look_dir == UP )
					angle = 90.0;
				else if( this->look_dir == DOWN )
					angle = -90.0;
			}
			else if(this->init_look_dir == UP)
			{
				if( this->look_dir == DOWN )
						angle = 180.0;
				else if( this->look_dir == LEFT )
					angle = 90.0;
				else if( this->look_dir == RIGHT )
					angle = -90.0;
			}	
			else if(this->init_look_dir == DOWN)
			{
				if( this->look_dir == LEFT )
						angle = -90.0;
				else if( this->look_dir == UP )
					angle = 180.0;
				else if( this->look_dir == RIGHT )
					angle = 90.0;
			}		
			rotateAngle += angle / turn_fract * this->turning;
		}

		// draw character 
		// draw spacesuit
		if(this->imposter_check)
			glColor3f( 0.8, 0, 0 );
		else
			glColor3f( 0, 0, 0.8 );
		glTranslatef(20, 15, 0);
		glRotatef(rotateAngle, 0, 0, 1);
		glTranslatef(-20, -15, 0);
		if(this->imposter_check)
			glColor3f( 0.8, 0, 0 );
		else
			glColor3f( 0, 0, 0.8 );
		glCallList( Suit );

		// draw bag
		glPopMatrix();
		glPushMatrix();
		glTranslatef( 5, 20, 0 );
		glRotatef( 180, 0 , 1, 0 );
		glCallList( Bag );

		// draw legs
		glPopMatrix();
		glPushMatrix();
		glTranslatef( 8, 0, 0 );
		glRotatef( 180, 0, 1, 0 );
		glCallList( Leg );

		glPopMatrix();
		glPushMatrix();
		glTranslatef( 32, 0, 0 );
		glRotatef( 180, 0, 1, 0 );
		glCallList( Leg );

		// draw glass
		glPopMatrix();
		glPushMatrix();
		glTranslatef( 23, 30, 0 );
		glCallList( Glass );
		glPopMatrix();
	}
}


// function to set direction of character
void Astro::set_dir( DIR new_look_dir )
{
	this->init_look_dir = look_dir;
	this->look_dir = new_look_dir;
	this->ismoving = 1;
	this->turning = 0;
}


void Astro::move()
{
	// character movement
	step_size = 28.5 * fabs(sin(sin(7 * atan(-1) / 180) * walking) - sin(sin(7 * atan(-1) / 180) * (walking - 1)));

	// taking steps after turning
	if( this->turning == turn_fract )
	{
		switch(this->look_dir) 
		{
			case UP:
				this->curr_y += step_size;
				break;
			case DOWN:
				this->curr_y -= step_size;
				break;
			case LEFT:
				this->curr_x -= step_size;
				break;
			case RIGHT:
				this->curr_x += step_size;
				break;
		}
	}

	if(abs(this->prev_x - this->curr_x) >= 10.0) 
	{
		this->curr_x = this->prev_x + ((this->look_dir == RIGHT) ? 10.0 : -10.0);
		this->prev_x = this->curr_x;
		this->ismoving = false;
	}
	else if(abs(this->prev_y - this->curr_y) >= 10.0) 
	{
		this->curr_y = prev_y + ((this->look_dir == UP) ? 10.0 : -10.0);
		this->prev_y = curr_y;
		this->ismoving = false;
	}

	// once turning is over, walk in direction
	if(this->turning == turn_fract) 
	{
		this->walking++;
		if( this->walking > 5 )
			this->walking = -4;
	}
}

// updating status of character
void Astro::updateStatus() 
{
	if (this->ismoving) move();
	if (this->turning < turn_fract) 
	{
		this->turning++;
		// looking after turning
		if (this->turning == turn_fract) 
			this->init_look_dir = this->look_dir;
		// turning over
		if (this->init_look_dir == this->look_dir) 
			this->turning = turn_fract;
	}
	// check if exited maze
	if(this->get_exit == true) 
		this->maze_exited++;
}
