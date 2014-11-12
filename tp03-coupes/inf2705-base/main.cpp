// Prénoms, noms et matricule des membres de l'équipe:
// - Raoul Pythagore Deffo 1635142
// - Gwenegan Hudin 1756642

#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>
#include "varglob.h"
#include "float3.h"

enum Figure {TORE, SPHERE, CUBE, THEIERE};
Figure figure = TORE;

GLdouble profondeur = 0.0; // Profondeur de placement du plan de coupe


static void animer( int tempsPrec )
{
   // obtenir le temps depuis le début du programme, en millisecondes
   int tempsCour = glutGet( GLUT_ELAPSED_TIME );
   if ( tempsPrec == 0 ) tempsPrec = tempsCour;

   // temps d'attente en secondes avant le prochain affichage
   const int FPS = 60;  // en "images/seconde"
   const int delai = 1000/FPS;  // en "millisecondes/image" (= 1000 millisecondes/seconde  /  images/seconde)
   if ( enmouvement ) glutTimerFunc( delai, animer, tempsCour );

   // indiquer qu'il faut afficher à nouveau
   glutPostRedisplay();
}

void initialisation()
{
   enmouvement = true;

   // donner la couleur de fond
   glClearColor( 0.2, 0.2, 0.2, 1.0 );

   // activer les etats openGL
   glEnable( GL_NORMALIZE );
   glEnable( GL_DEPTH_TEST );
}

void definirCamera()
{
   static double angle = DEG2RAD(150);
   angle += .01;

   // Placement de la caméra de manière à ce qu'elle suive la théière autour d'une sphère
   gluLookAt( 10*cos(angle), 10*sin(angle), 5,  0, 0, 0,  0, 0, 1 );
}

void afficherModele()
{
   static GLUquadric* sphere = gluNewQuadric();
   glPushMatrix(); {
      glTranslatef( 0, 0, 1 );
      glRotatef( 90, 1, 0, 0 );
      //glColor3f( 1.0, 1.0, 0.0 );
      switch(figure) {
		case TORE:
			glutSolidTorus( 0.3, 0.6, 32, 32 );
			break;
		case SPHERE:
			gluSphere( sphere, 0.6, 32, 32 );
			break;
		case CUBE:
			glutSolidCube( 1.2 );
			break;
		case THEIERE:
			glutSolidTeapot( 1.0 );
			break;
		default:
			break;
	  }
   } glPopMatrix();
}

void afficherSol()
{
   glColor3f( 1., .5, .5 );
   glBegin( GL_QUADS );
      glVertex3f( -4.0,  4.0, 0.0 );
      glVertex3f(  4.0,  4.0, 0.0 );
      glVertex3f(  4.0, -4.0, 0.0 );
      glVertex3f( -4.0, -4.0, 0.0 );
   glEnd();
}

void afficherScene()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluPerspective( 30.0, (GLdouble) g_largeur / (GLdouble) g_hauteur, 0.1, 300.0 );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   definirCamera();

   	// Plan de coupe
	const GLdouble equation[] = { 1, 0, 0, profondeur };
	glPushMatrix();
		glClipPlane( GL_CLIP_PLANE0, equation );
		glTranslatef(0.0, 0.0, 1.0);
		glBegin(GL_LINE_LOOP);
			glColor3f(0.0,0.0,0.0);
			glVertex3d(-profondeur, -1, -1);
			glVertex3d(-profondeur, -1, 1);
			glVertex3d(-profondeur, 1, 1);
			glVertex3d(-profondeur, 1, -1);
		glEnd();
    glPopMatrix();
    glEnable( GL_CLIP_PLANE0 );
   
   // Stencil
	glEnable( GL_STENCIL_TEST );
	
	// Dessiner l'objet complet en incremetant le buffer
	// (buffer a 2 sur passage en Front et Back, 1 pour seulement Back)
	// Seuls les fragments situes dans le plan de coupe sont a 1
	glStencilOp( GL_KEEP, GL_INCR, GL_INCR );
	glStencilFunc( GL_ALWAYS, 1, 1 );
	glColor3f( 1.0, 1.0, 0.0 );
	afficherModele();
	
	glDisable(GL_CLIP_PLANE0);
	
	// Sur les zones a 1, on garde les fragments d'un quadrilatere vert
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	glStencilFunc( GL_EQUAL, 1, 1 );
	glColor3f( 0.0, 1.0, 0.0 );
	glPushMatrix();
		glTranslatef(0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glVertex3d(-profondeur, -1, -1);
			glVertex3d(-profondeur, -1, 1);
			glVertex3d(-profondeur, 1, 1);
			glVertex3d(-profondeur, 1, -1);
		glEnd();
    glPopMatrix();

   glDisable(GL_STENCIL_TEST);
   

   // tracer le sol
   afficherSol();

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
	case '1':
		figure = TORE;
		break;
	case '2':
		figure = SPHERE;
		break;
	case '3':
		figure = CUBE;
		break;
	case '4':
		figure = THEIERE;
		break;
   case ' ':
      enmouvement = !enmouvement;
      if ( enmouvement ) animer( 0 );
      break;
   case '+':
	   if ( profondeur < 2.0 )
			 profondeur += 0.05;
		  break;
   case '-':
      if ( profondeur > -2.0 )
         profondeur -= 0.05;
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
   glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL );
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
   if ( enmouvement ) animer( 0 );

   // boucle principale de gestion des evenements
   glutMainLoop();

   // le programme n'arrivera jamais jusqu'ici
   return EXIT_SUCCESS;
}
