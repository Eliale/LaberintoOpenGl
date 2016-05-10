#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <stack>
#include <windows.h>

using namespace std;


#define N 20

void cargaMatriz(int matriz[N][N]);
void pared(int x, int y, int r, int g, int b);
void estado(int x, int y, int tX, int tY);
int matriz[N][N];
int inicio[2] = {0,0};// el inicio estara´ en el origen del laberinto
int salida[2] = {N-1,N-1}; //la salida estara´ en la esquina que se encuentra en la diagonal
int tx = 0;
int ty = 0;
int v[N][N];

bool Primera_Pasada = true;

void display() //Funcion de dibujado
{
	int i;
	int j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (matriz[i][j] == 1)
				pared(j,i,0,0,0); else
					pared(j,i,1,1,1);
		}
	}
	
	//en esta parte deben insertar el estado en el que se encuentran y cuyas coordenadas corresponden al vector de traslacio´n cada vez que se llegue a un nuevo estado valido**==============================================================**/
	estado(inicio[0],inicio[1],tx,ty);/**============================**/
	/**==============================================================**/
	
	glFlush(); //Forza el limpiado de pantalla
}

int Valor_Absoluto( int x )
{
	if( x < 0 ) {
		
		return -x;
	}
	return x;
}


int DFS2( pair<int, int > nodoActual )
{
	int x = nodoActual.first;
	int y = nodoActual.second;
	printf("X :%d   Y:  %d\n",x,y);
	Sleep(250);
	tx = x;
	ty = y;
	display();
	
	if( nodoActual.first == salida[0] && nodoActual.second == salida[1] ){
		return true;
		}
	
	for( int i = -1; i <= 1; i++ )
	{
		for( int j = -1; j <= 1; j++)
		{
			if( Valor_Absoluto(j + i) == 1 )
			{
				
				int nx = x + i;
				int ny = y + j;
				if( (0 <= nx && nx < N) && (0 <= ny && ny < N) )
				{
					// Si es un nodo vecino (4 adyacencias) y no se sale del tablero
					if( !matriz[ny][nx] && !v[nx][ny] )
					{
						//printf("->%d %d\n",nx,ny);
						// Si existe un pasillo y no he pasado por este nodo
						v[nx][ny] = 1;
						bool ret = DFS2( make_pair(nx,ny) );
						if( ret ){   
						return ret;
						}
						Sleep(250);
						tx = x;
						ty = y;
						display();
						
					}
				}
			}
		}
	}
	return false;
}

void DFSrecursivo ()
{
	if( Primera_Pasada )
	{
		int x = inicio[0];
		int y = inicio[1];
		v[x][y] = 1;
		DFS2(make_pair(x,y));
		Primera_Pasada = false;
	}
}

void DFS( )
{
	pair<int, int > ini;
	pair<int, int > sal;
	
	ini = make_pair(inicio[0], inicio[1]);
	sal = make_pair(salida[0], salida[1]);
	
	stack<pair<int, int> > pila;
	pila.push(ini);
	v[ini.first][ini.second] = 1;
	
	while( !pila.empty() )
	{
		pair<int,int> nodoActual = pila.top();  pila.pop();
		int x = nodoActual.first;
		int y = nodoActual.second;
		//printf("%d %d\n",x,y);
		Sleep(250);
		tx = x;
		ty = y;
		display();
		
		if( sal == nodoActual )
			break;
		
		for( int i = -1; i <= 1; i++ )
		{
			for( int j = -1; j <= 1; j++)
			{
				if( Valor_Absoluto(j + i) == 1 )
				{
					
					int nx = x + i;
					int ny = y + j;
					if( (0 <= nx && nx < N) && (0 <= ny && ny < N) )
					{
						// Si es un nodo vecino (4 adyacencias) y no se sale del tablero
						if( !matriz[ny][nx] && !v[nx][ny] )
						{
							//printf("->%d %d\n",nx,ny);
							// Si existe un pasillo y no he pasado por este nodo
							pila.push(make_pair(nx,ny));
							v[nx][ny] = 1;
						}
					}
				}
			}
		}
	}
	puts("Salida");
}

void init()
{
	glClearColor(1.0,1.0,1.0,0.0); //Limpiar Pantalla
}
void reshape(int width, int height)
{
	glClear(GL_COLOR_BUFFER_BIT); //Buffer
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION); //Genera proyeccion
	glLoadIdentity();
	glOrtho(0.0,N,0.0,N,-1.0,1.0); //Perspectiva
	glMatrixMode(GL_MODELVIEW);//Tipo de Proyeccion
}

int main(int argc, char ** argv)
{
	for(int i = 0; i < N; i++){
		for( int j = 0; j < N; j++){
			v[i][j] = 0;
			}
	}
	cargaMatriz(matriz);//se llena la forma del laberinto mediante una matriz binaria
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); //Funcion Main
	glutInitWindowPosition(100,100); //Posicion de Ventana
	glutInitWindowSize(500,500); //Cambiar Tamaño de Ventana
	glutCreateWindow("Inteligencia Artificial - Laberinto - Instituto Tecnologico de Oaxaca"); //Crea y cambia el nombre de la ventana
	glutDisplayFunc(display); //Funcion de Dibujado anteriormente creada
	glutReshapeFunc(reshape);
	glutIdleFunc(DFSrecursivo);
	glutMainLoop(); //Loop
	return 0;
}
void cargaMatriz(int matriz[N][N])
{
	int i = 0 ;
	int j = 0 ;
	int matriz1[N][N] = {
		//el laberinto esta´ dado por una matriz binaria en donde 0 indica que es un
		//pasillo y 1 que es un muro.
		{0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
			{0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1},
			{0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0},
			{1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},
			{0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0},
			{0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1},
			{0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0},
			{1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,0,0,0,1,0},
			{0,0,0,0,0,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0},
			{0,1,1,1,1,1,0,1,0,1,1,1,1,0,0,0,1,0,0,0},
			{0,1,0,0,0,1,0,1,0,0,1,1,1,1,1,1,1,0,1,1},
			{0,1,1,1,0,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0},
			{0,0,0,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0},
			{0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0},
			{0,0,0,0,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,1},
			{0,1,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,0,0},
			{0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0}
	};
	for (i=0;i<N;i++)
		for (j=0;j<N;j++)
		{
			matriz[j][i]=matriz1[j][i];
		}
}
void pared(int x, int y, int r, int g, int b)
{
	glBegin(GL_QUADS); //Modelo a Dibujar
	glColor3f(r,g,b);
	glVertex3f(x,y,0.0);
	glVertex3f(x + 1,y,0.0);
	glVertex3f(x + 1,y + 1,0.0);
	glVertex3f(x,y + 1,0.0);
	glEnd(); //Finaliza el dibujo
}
void estado(int x, int y, int tX, int tY)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glVertex2f(x +0.5 + tX,y + 0.5 +tY);
	glEnd();
	glTranslatef(tX,tY,0.0f);
	glPopMatrix();
}
