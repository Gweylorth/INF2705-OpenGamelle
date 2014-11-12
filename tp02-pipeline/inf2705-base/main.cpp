// Prénoms, noms et matricule des membres de l'équipe:
// - Pythagore Deffo (1635142)
// - Gwenegan Hudin (1756642)

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "varglob.h"
#include "float3.h"
#include "teapot_data.h"

// partie 1:
double thetaBras = 0.0;   // angle de rotation du bras
double phiBras = 0.0;     // angle de rotation du bras

// partie 2:
GLdouble MAXPHI = M_PI - 0.001, MAXTHETA = M_PI - 0.001;
GLdouble MINPHI = 0.001, MINTHETA = 0.001;

GLuint g_VBOsommets = 0;
GLuint g_VBOconnec = 0;

bool modeLookAt = true;
bool modePlein = true;

void creerVBO()
{
   // créer le VBO pour les sommets
   glGenBuffers(1, &g_VBOsommets);
   glBindBuffer(GL_ARRAY_BUFFER, g_VBOsommets);
   glBufferData(GL_ARRAY_BUFFER, sizeof(gTeapotSommets), gTeapotSommets, GL_STATIC_DRAW);

   // créer le VBO la connectivité
   glGenBuffers(1, &g_VBOconnec);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_VBOconnec);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gTeapotConnec), gTeapotConnec, GL_STATIC_DRAW);
}

void initCamValues () 
{
	theta = 45.;
	phi = 75.;
	dist = 15.;
}

void initialisation()
{
   initCamValues();

   // donner la couleur de fond
   glClearColor( 0.0, 0.0, 0.0, 1.0 );

   // activer les etats openGL
   glEnable( GL_DEPTH_TEST );

   glewInit();

   creerVBO();
}

// (partie 2) Vous modifierez cette fonction pour utiliser les VBOs
void afficherTheiere()
{
   glColor3f( 0.0, 1.0, 0.0 );

   // glut offre une fonction pour tracer une théière, mais ce n'est pas ce qu'on veut utiliser
   glPushMatrix();
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 0, 0);
		glDrawElements( GL_TRIANGLES, 1024*3, GL_UNSIGNED_INT, 0 );
		glDisableClientState( GL_VERTEX_ARRAY );
   glPopMatrix();
}

// Vous devez vous servir de ces fonctions pour tracer les quadriques.
void afficherCylindre( )
{
   // affiche un cylindre de rayon 1 et de hauteur 1
   static GLUquadric* q = 0;
   if ( !q ) q = gluNewQuadric();
   const GLint slices = 16, stack = 2;
   glColor3f( 0, 0, 1 );
   gluCylinder( q, 1.0, 1.0, 1.0, slices, stack );
}
void afficherSphere( )
{
   // affiche une sphere de rayon 1
   static GLUquadric* q = 0;
   if ( !q ) q = gluNewQuadric();
   const GLint slices = 16, stack = 32;
   glColor3f( 1, 0, 0 );
   gluSphere( q, 1.0, slices, stack );
}

void afficherBras()
{
   const GLfloat cylhauteur = 2.0;
   const GLfloat sphererayon = 0.25;
   
   // ajouter une ou des transformations afin de tracer un cylindre de rayon "sphererayon" et de hauteur "cylhauteur"
   // Cylindre 1
   glPushMatrix();
	   glScalef(sphererayon, sphererayon, cylhauteur);
	   afficherCylindre();
   glPopMatrix();

   // ajouter une ou des transformations afin de tracer une sphère de rayon "sphererayon"
   // Sphere 1
   glPushMatrix();
	   glTranslatef(0.0, 0.0, cylhauteur);
	   glScalef(sphererayon, sphererayon, sphererayon);
	   afficherSphere();
   glPopMatrix();
   
   // Cylindre 2
   glPushMatrix();
		glRotatef(thetaBras, 0.0, 0.0, 1.0);
	   glTranslatef(0.0, 0.0, cylhauteur);
	   glRotatef(90.0, 0.0, 1.0, 0.0);
	   glScalef(sphererayon, sphererayon, cylhauteur);
	   afficherCylindre();
   glPopMatrix();
   
   // Sphere 2
   glPushMatrix();
		glRotatef(thetaBras, 0.0, 0.0, 1.0);
	   glTranslatef(cylhauteur, 0.0 , cylhauteur );
	   glScalef(sphererayon, sphererayon, sphererayon);
	   afficherSphere();
   glPopMatrix();
   
   // A partir d'ici, on decompose les translations en 2 : 
   // translation de Sphere 2 (se placer dans le bon repere pour 
   // rotation Phi), puis translation complementaire pour placer l'objet
   
   // Cylindre 3
   glPushMatrix();
		glRotatef(thetaBras, 0.0, 0.0, 1.0);
	   glTranslatef(cylhauteur, 0.0 , cylhauteur );
	   glRotatef(phiBras, 0.0, 0.0, 1.0);
	   glRotatef(90.0, 0.0, 1.0, 0.0);
	   glScalef(sphererayon, sphererayon, cylhauteur);
	   afficherCylindre();
   glPopMatrix();
   
   // Sphere 3
   glPushMatrix();
		glRotatef(thetaBras, 0.0, 0.0, 1.0);
		glTranslatef(cylhauteur, 0.0 , cylhauteur );
		glRotatef(phiBras, 0.0, 0.0, 1.0);
	   glTranslatef(cylhauteur, 0.0 , 0.0 );
	   glScalef(sphererayon, sphererayon, sphererayon);
	   afficherSphere();
   glPopMatrix();
   
   // Théière
   glPushMatrix();
		glRotatef(thetaBras, 0.0, 0.0, 1.0);
		glTranslatef(cylhauteur, 0.0 , cylhauteur );
		glRotatef(phiBras, 0.0, 0.0, 1.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glTranslatef(1.75 * cylhauteur, 0.0, 0.0);
		glScalef(0.5,0.5,0.5);
		glTranslatef(1.5, -2.0, 0.0);
		afficherTheiere();
   glPopMatrix();
}

void definirCamera()
{
	if ( modeLookAt ) {
		gluLookAt(
			dist*sin(DEG2RAD(phi))*cos(DEG2RAD(theta)),
			dist*sin(DEG2RAD(phi))*sin(DEG2RAD(theta)),
			dist*cos(DEG2RAD(phi)), 
			0, 0, 0,  
			0, 0, 1 );
	} else {
		glTranslatef(0.0, 0.0, -dist );
		glRotatef(phi, -1.0, 0.0, 0.0 );
		glRotatef(theta + 90, 0.0, 0.0, -1.0 );
	}
	
}

void afficherScene()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluPerspective( 45.0, (GLdouble) g_largeur / (GLdouble) g_hauteur, 0.1, 300.0 );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   definirCamera();

   glColor3f( 1., .5, .5 );
   glBegin( GL_QUADS );
   glVertex3f( -4.0,  4.0, 0.0 );
   glVertex3f(  4.0,  4.0, 0.0 );
   glVertex3f(  4.0, -4.0, 0.0 );
   glVertex3f( -4.0, -4.0, 0.0 );
   glEnd();

   afficherBras();

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
   case '-':
   case '_':
      dist += 0.1;
      break;
   case '+':
   case '=':
      if ( dist > 1.0 )
         dist -= 0.1;
      break;
   case 'r':
	  initCamValues();
      break;
   case 'l':
      modeLookAt = !modeLookAt;
      std::cout << " modeLookAt=" << modeLookAt << std::endl;
      break;
   case 'g':
      {
         modePlein = !modePlein;
         glPolygonMode( GL_FRONT_AND_BACK, (modePlein) ? GL_FILL : GL_LINE);
		 glutPostRedisplay(); 
      }
      break;
   }
   glutPostRedisplay();
}

void clavierSpecial( int touche, int x, int y )
{
   switch ( touche )
   {
   case GLUT_KEY_LEFT:
      if (phiBras > -90) phiBras -= 2.0;
      break;
   case GLUT_KEY_RIGHT:
      if (phiBras < 90) phiBras += 2.0;
      break;
   case GLUT_KEY_DOWN:
      thetaBras -= 2.0;
      break;
   case GLUT_KEY_UP:
      thetaBras += 2.0;
      break;
   }
   glutPostRedisplay();
}

int lastx, lasty;
void sourisClic( int button, int state, int x, int y )
{
   // button est un parmi { GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON }
   // state  est un parmi { GLUT_DOWN, GLUT_UP }
   // Pour référence et quelques exemples, voir http://www.lighthouse3d.com/opengl/glut/index.php?9
   if ( state == GLUT_DOWN )
   {
		lastx = x;
		lasty = y;
   }
}

void sourisMouvement( int x, int y )
{
	double deltaPhi = (y - lasty)/2;
	if ((phi + deltaPhi) > 0 && (phi + deltaPhi) < 180){
		phi += deltaPhi;
	}
	theta += (x - lastx)/2;
	
	lastx = x;
	lasty = y;
	
   glutPostRedisplay();
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

   // boucle principale de gestion des evenements
   glutMainLoop();

   // le programme n'arrivera jamais jusqu'ici
   return EXIT_SUCCESS;
}
