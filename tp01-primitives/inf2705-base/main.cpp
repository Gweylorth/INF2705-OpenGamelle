// Prénoms, noms et matricule des membres de l'équipe:
// - Raoul Pythagore Deffo (1635142)
// - Gwenegan Hudin (1756642)

#include <iostream>
#include <GL/glut.h>
#include "varglob.h"
#include "float3.h"

#if 1
// pour un W
float p1[3] = { -4.0,  2.0,  0.0 };
float p2[3] = { -3.0, -3.0,  0.0 };
float p3[3] = { -1.0, -3.0,  0.0 };
float p4[3] = {  0.0,  0.0,  0.0 };
float p5[3] = {  1.0, -3.0,  0.0 };
float p6[3] = {  3.0, -3.0,  0.0 };
float p7[3] = {  4.0,  2.0,  0.0 };
#else
// pour une flèche (Voir apprentissage supplémentaire)
float p1[3] = { -3.0,  1.0,  0.0 };
float p2[3] = { -3.0, -1.0,  0.0 };
float p3[3] = {  0.0, -1.0,  0.0 };
float p4[3] = { -0.5, -2.5,  0.0 };
float p5[3] = {  3.0,  0.0,  0.0 };
float p6[3] = { -0.5,  2.5,  0.0 };
float p7[3] = {  0.0,  1.0,  0.0 };
#endif

bool line = false;

void initialisation()
{
   // donner la couleur de fond
   glClearColor( 0.0, 0.0, 0.0, 1.0 );

   // définir le pipeline graphique
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( -12, 12, -8, 8, -10, 10 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   // activer le mélange de couleur pour bien voir les possibles plis à l'affichage
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void afficherScene()
{
   glLoadIdentity();
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Tracer les lignes blanches
   glColor3f( 1, 1, 1 );
   glBegin(GL_LINES);
		glVertex3f(-12, 0, 0.0);
		glVertex3f(12, 0, 0.0);
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(-4, 8, 0.0);
		glVertex3f(-4, -8, 0.0);
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(4, 8, 0.0);
		glVertex3f(4, -8, 0.0);
	glEnd();

   // utiliser du rouge
   glColor4f( 1, 0, 0, 0.8);
   
   glTranslatef(-8.0, 4.5, 0.0);
   
   glBegin(GL_QUADS);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p3[0], p3[1], p3[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p5[0], p5[1], p5[2]);
		glVertex3f(p6[0], p6[1], p6[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
	glEnd();
	
	// utiliser du vert
   glColor4f( 0, 1, 0, 0.8);
   
   glTranslatef(8.0, 0.0, 0.0);
   
   glBegin(GL_TRIANGLES);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p3[0], p3[1], p3[2]);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p5[0], p5[1], p5[2]);
		glVertex3f(p6[0], p6[1], p6[2]);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f(p6[0], p6[1], p6[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
		glVertex3f(p1[0], p1[1], p1[2]);
	glEnd();
	
	// utiliser du bleu
   glColor4f( 0, 0, 1, 0.8);
   
   glTranslatef(8.0, 0.0, 0.0);
   
   glBegin(GL_POLYGON);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p3[0], p3[1], p3[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
   glEnd();
   glBegin(GL_QUADS);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p5[0], p5[1], p5[2]);
		glVertex3f(p6[0], p6[1], p6[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
	glEnd();
	
	// utiliser du cyan
   glColor4f( 0, 1, 1, 0.8);
   
   glTranslatef(-16, -8.0, 0.0);
   glBegin(GL_QUAD_STRIP);
		glVertex3f(p3[0], p3[1], p3[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p5[0], p5[1], p5[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
   glEnd();
   
   glBegin(GL_TRIANGLES);
		glVertex3f(p5[0], p5[1], p5[2]);
		glVertex3f(p6[0], p6[1], p6[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
	glEnd();

   // utiliser du magenta
   glColor4f( 1, 0, 1, 0.8);
   
   glTranslatef(8, 0.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(p3[0], p3[1], p3[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
   glEnd();
   
   glBegin(GL_TRIANGLE_FAN);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
		glVertex3f(p6[0], p6[1], p6[2]);
		glVertex3f(p5[0], p5[1], p5[2]);
   glEnd();
   
   // utiliser du jaune
   glColor4f( 1, 1, 0, 0.8);
   
   glTranslatef(8, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p3[0], p3[1], p3[2]);
		glVertex3f(p4[0], p4[1], p4[2]);
   glEnd();
   
   glBegin(GL_TRIANGLE_FAN);
		glVertex3f(p4[0], p4[1], p4[2]);
		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p7[0], p7[1], p7[2]);
		glVertex3f(p6[0], p6[1], p6[2]);
		glVertex3f(p5[0], p5[1], p5[2]);
   glEnd();


   glutSwapBuffers();
}

void redimensionnement( GLsizei w, GLsizei h )
{
   g_largeur = w;
   g_hauteur = h;
   glViewport( 0, 0, w, h );
   glutPostRedisplay();
}

void clavier( unsigned char touche, int x, int y )
{
   switch ( touche )
   {
   case '\e': // escape
      glutDestroyWindow( g_feneID );
      exit( 0 );
      break;
   case 'g':
	  line = !line;
	  glPolygonMode( GL_FRONT_AND_BACK, (line) ? GL_LINE : GL_FILL);
      glutPostRedisplay(); // indiquer que la fenêtre a besoin d'être réaffichee
      break;
   }
}

void clavierSpecial( int touche, int x, int y )
{
}

void sourisClic( int button, int state, int x, int y )
{
}

void sourisMouvement( int x, int y )
{
}

int main( int argc, char *argv[] )
{
   // initialisation de GLUT
   glutInit( &argc, argv );
   // paramétrage de l'affichage
   glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
   // taille de la fenetre
   glutInitWindowSize( g_largeur, g_hauteur );
   // création de la fenêtre
   g_feneID = glutCreateWindow( "INF2705" );

   // référencement de la fonction de rappel pour l'affichage
   glutDisplayFunc( afficherScene );
   // référencement de la fonction de rappel pour le redimensionnement
   glutReshapeFunc( redimensionnement );
   // référencement de la fonction de gestion des touches
   glutKeyboardFunc( clavier );
   // référencement de la fonction de gestion des touches spéciales
   glutSpecialFunc( clavierSpecial );
   // référencement de la fonction de rappel pour le mouvement de la souris
   glutMotionFunc( sourisMouvement );
   // référencement de la fonction de rappel pour le clic de la souris
   glutMouseFunc( sourisClic );

   initialisation();

   // boucle principale de gestion des événements
   glutMainLoop();

   // le programme n'arrivera jamais jusqu'ici
   return EXIT_SUCCESS;
}
