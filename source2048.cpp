#include<iostream>
#include<unistd.h>
#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>

using namespace std;

int flag=1;

void clsr ()
{
	cout<<"\E[2J\E[H";
}

long double highScore;

int stack[20][4][4];     // stack to hold states
int top=0;

long double score;             //global scope

void push ( int grid[4][4] , int stack[20][4][4] )
{
	if ( (top) <20 )
	{
		for ( int i=0 ; i<4 ; i++ )
			for ( int j=0 ; j<4 ; j++ )
				stack[top][i][j] = grid[i][j];
	top++;
	}

	else
	{
		for ( int g=0 ; g<19 ; g++ )
			for ( int i=0; i<4; i++ )
				for ( int j=0 ; j<4; j++)
					stack[g][i][j] = stack[g+1][i][j];

		for ( int i=0 ; i<4 ; i++ )
			for ( int j=0 ; j<4 ; j++ )
				stack[top-1][i][j] = grid[i][j];
	
	}
}

long double scoreStack[20];
void pushScore()
{
		scoreStack[top-1]=score;
}

long double popScore ()
{
	return scoreStack[top];
}


void pop (int  grid[4][4] ,  int stack[20][4][4])
{
		(top)--;
		for ( int i=0 ; i<4 ; i++ )
			for ( int j=0 ; j<4 ; j++ )
				grid[i][j] = stack[top][i][j];
}



void print ( int grid [4][4]  )
{
//	clsr();
cout<<"Best : "<<highScore<<endl;
	for ( int i=0 ; i<4 ; i++)
	{
		for ( int j=0 ; j<4; j++ )
		{
			if (grid[i][j]==-1)
				cout<<'-'<<'\t';
			else
			cout<<grid[i][j]<<'\t';
		}
		cout<<endl;
	}
	cout<<"SCORE:  "<<int(score)<<endl;
	cout<<"Press u for undo"<<endl;
	cout<<"No of undos left : "<<top<<endl;
}


void up ( int grid [4][4] )
{
	int flag=0; int p;
	for ( int i=0 ; i<4; i++ )
	{
		for ( int a=0; a<4 ; a++ )
		{
			if ( grid[a][i] < 0 )           // blank cell => move row up
			{
				for ( int x=a ; x<3; x++ )
				{
					grid[x][i] = grid[x+1][i];
					if( x==2 )
						grid [x+1][i] = -1;
				}
				 if ( a!=3 && grid[a][i] < 0 )
				 {
						for ( int x=a ; x<3; x++ )
						{
							grid[x][i] = grid[x+1][i];
							if( x==2 )
								grid [x+1][i] = -1;
						}
				 }
				 if ( a!=3 && grid[a][i] < 0 )
				 {
						for ( int x=a ; x<3; x++ )
						{
							grid[x][i] = grid[x+1][i];
							if( x==2 )
								grid [x+1][i] = -1;
						}
				 }
  			if( a!=3 && grid[a][i]==grid[a-1][i] && grid[a-1][i]>0 )
  			{
					grid[a-1][i]=2*(grid[a-1][i]);
					score+=grid[a][i];
					for ( int x=a ; x<3; x++ )
						{
							grid[x][i] = grid[x+1][i];
							if( x==2 )
								grid [x+1][i] = -1;
	  				 }
			   }
  			if( a!=3 && grid[a][i]==grid[a+1][i] && grid[a+1][i]>0 )
  			{
					grid[a][i]=2*(grid[a][i]);
					score+= grid[a][i];
					for ( int x=a+1 ; x<3; x++ )
						{
							grid[x][i] = grid[x+1][i];
							if( x==2 )
								grid [x+1][i] = -1;
	  				 }
			   }
  	 }
	
			else if ( grid[a][i] != grid[a+1][i] )
				continue; 															  //do nothing if consecutive elements are unequeal
			else if ( grid [a][i] == grid[a+1][i] )
				{
					grid[a][i] = 2 * grid[a][i];               //double upper element and move rest of the row up
					score+=grid[a][i];
						grid [a+1][i]=-1;
					for ( int x=a+1 ; x<3; x++ )
					{
						grid[x][i] = grid[x+1][i];
						if( x==2 )
							grid [x+1][i] = -1;
					}
				}
		}
	}
}

void left ( int grid [4][4] )
{
	int temp[4][4];
	for ( int i=0; i<4 ; i++ )
		for ( int j=0 ; j<4 ; j++ )
			temp [j][3-i] = grid [i][j];
	up ( temp );
	for ( int i=0; i<4 ; i++ )
		for ( int j=0 ; j<4 ; j++ )
			grid [i][j] = temp [j][3-i];
}
	

void down  ( int grid[4][4] )
{
	int temp [4][4];
		for ( int i=0 ; i<4 ; i++ )
			for ( int j=0 ; j<4; j++ )
				temp[3-i][3-j] = grid[i][j];
		up (temp );
		for ( int i=0 ; i<4 ; i++ )
			for ( int j=0 ; j<4; j++ )
				grid[3-i][3-j] = temp[i][j];
}

void right ( int grid [4][4] )
{
	int temp[4][4];
	for ( int i=0; i<4 ; i++ )
		for ( int j=0 ; j<4 ; j++ )
			temp [j][3-i] = grid [i][j];
	down ( temp );
	for ( int i=0; i<4 ; i++ )
		for ( int j=0 ; j<4 ; j++ )
			grid [i][j] = temp [j][3-i];
}
	
int newNum()
{
	int a;
	a= ((rand()%2)+1)%3;
	return 2*a;
}
int xplace=0,yplace=0;


void locUpdate ( int grid[4][4] , int* locX , int* locY )
{	
	xplace=0,yplace=0;
	for ( int i=0 ; i<16 ; i++ )
	{
		locX[i]=9;
		locY[i]=9;
	}
	
	for ( int i=0 ; i<4 ; i++ )
		for ( int j=0 ; j<4 ; j++ )
			if (grid[i][j]>0)
			{
				locX[xplace++] = i;
				locY[yplace++] = j;
			}
}


char getch( int grid[4][4])
{
    
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
//    printf("%c\n",buf);
	switch ( buf)
	{
		  case 117:
			if(top==0)
			{
				flag=0;
				getch(grid);
				break;
			}
			else
				flag=0;
			  pop(grid , stack );
				score = popScore();
				break;
	    case 119:
        flag=1;
				push(grid,stack);
				pushScore();
				 up(grid);
			 		break;
      case 97: 
			 flag=1;
				push(grid,stack);
				pushScore();
				left (grid);
					 break;
      case 100:
			 flag=1;
				push(grid,stack);
				pushScore();
				 right(grid);
					 break;
      case 115:
			 flag=1;
				push(grid,stack);
				pushScore();
				 down (grid);
					 break;
      default:
      {
		   cout<<"error"<<endl;
		   getch(grid);
		   break;
	   }
    }
    }

int writeFlag=1;
ifstream in ("highScore2048.txt");
ofstream out ("highScore2048.txt");

int checkLoc ( int x, int y , int* locX , int* locY )
{
	if ( writeFlag==1);
	{
		in>>locX[0]>>locY[0];
		writeFlag++;
		in>>highScore;
	}

	int flag=0;
	for ( int i=0 ; i<10 ; i++ )
	{
		if ( locX[i] == x )
			if ( locY[i] == y )
				flag++;
		if (writeFlag==2)
		{
			out<<x<<endl<<y<<endl;
			writeFlag++;
		}
	}
		if (!flag)
			return 1;
		else 
			return 0;
}


void newLoc ( int*a , int*b , int* locX , int* locY)
{
int x,y;
	int flag = 0;
	while(! flag )
	{
		x = rand()%4;
		y = rand()%4;
		flag = checkLoc(x,y,locX,locY);
	}
	*a=x;
	*b=y;
}
	

int main()
{
	score =0;
    int grid [4][4] = { {-1,-1,-1,-1} , { -1,-1,-1,-1} , {-1,-1,-1,-1} , {-1,-1,-1,-1} };
	int* locX = new int[16]{9};
	int* locY = new int[16]{9};
	
	int x,y;
	int game = 1;
	
	while ( game )
	{
		clsr();
		newLoc ( &x , & y ,locX , locY);
		if (flag)
			grid[x][y] = newNum();
		print ( grid );
		getch(grid);
		locUpdate ( grid , locX , locY );
		if ( xplace == 16 )
		{
			game--;
			cout<<"gameover"<<endl;
			sleep(3);
			out<<score;
		}
	}
	return 0;
}
