// Prénoms, noms et matricule des membres de l'équipe:
// - Pythagore Deffo (1635142)
// - Gwenegan Hudin (1756542)

#include <iostream>
#include "fctsUtiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "float3.h"
#include "matrice.h"
#include "graph.h"
#include <math.h>
#include <string.h>
#include "varglob.h"

// Paramètres:            soleil, terre, lune, jupiter, europa, callisto
float Rayon[6]          = { 4.0,    0.5,    0.2,   1.2,   0.2,  0.3 };
float Distance[6]       = { 0.0,    8.0,    1.0,  14.0,   1.5,  2.0 };
float Rotation[6]       = { 5.0,   20.0,   20.0,  10.0,   5.0, 10.0 };
float Revolution[6]     = { 0.0,   45.0,   30.0,  30.0,  15.0,  2.0 };
float IncrRotation[6]   = { 0.01,   0.5,    0.5,   0.03,  0.7,  0.8 };
float IncrRevolution[6] = { 0.0,    0.005,  0.07,  0.003, 0.09, 0.7 };

Etoile soleil (Rayon[0], 
			Distance[0], 
			Rotation[0], 
			Revolution[0], 
			IncrRotation[0], 
			IncrRevolution[0]);
			
Planete terre (Rayon[1], 
			Distance[1], 
			Rotation[1], 
			Revolution[1], 
			IncrRotation[1], 
			IncrRevolution[1]);
			
Satellite lune (Rayon[2], 
			Distance[2], 
			Rotation[2], 
			Revolution[2], 
			IncrRotation[2], 
			IncrRevolution[2]);
			
Planete jupiter (Rayon[3], 
			Distance[3], 
			Rotation[3], 
			Revolution[3], 
			IncrRotation[3], 
			IncrRevolution[3]);
			
Satellite europa (Rayon[4], 
			Distance[4], 
			Rotation[4], 
			Revolution[4], 
			IncrRotation[4], 
			IncrRevolution[4]);
			
Satellite callisto (Rayon[5], 
			Distance[5], 
			Rotation[5], 
			Revolution[5], 
			IncrRotation[5], 
			IncrRevolution[5]);

const float MAXPHI = M_PI - 0.001, MINPHI = 0.001;

GLUquadric *sphere = NULL;

bool vuePole = false; // 0 pour la vue exoplanétaire; 1 pour la vue à partir du pôle Nord

float facteurRechauffement = 0.2; // la facteur qui servira à calculer la couleur des pôles (0.0=froid, 1.0=chaud)

static void animer( int tempsPrec )
{
	
   // obtenir le temps depuis le début du programme, en millisecondes
   int tempsCour = glutGet( GLUT_ELAPSED_TIME );
   if ( tempsPrec == 0 ) tempsPrec = tempsCour;

   // temps d'attente en secondes avant le prochain affichage
   const int FPS = 60;  // en "images/seconde"
   const int delai = 1000/FPS;  // en "millisecondes/image" (= 1000 millisecondes/seconde  /  images/seconde)
   glutTimerFunc( delai, animer, tempsCour );

   // Faire tourner la camera Pole
   Revolution[1] += IncrRevolution[1];
   Rotation[1] += IncrRotation[1];

   // indiquer qu'il faut afficher à nouveau
   glutPostRedisplay();
}

void chargerNuanceurs()
{
   // charger les nuanceurs
   const char *ns = "nuanceurs/nuanceurSommets.glsl";
   const char *nf = "nuanceurs/nuanceurFragments.glsl";
   progNuanceur = initialiserNuanceurs( ns, nf );
}

void initialisation()
{
    soleil.definirCouleur(1, 1, 0, 0.5);
    terre.definirCouleur(0.3, 0.5, 0.7);
    lune.definirCouleur(0.6, 0.6, 0.6);
    jupiter.definirCouleur(1, 0.5, 0.5);
    europa.definirCouleur(0.4, 0.4, 0.8);
    callisto.definirCouleur(0.5, 0.5, 0.1);

	// La Terre a 1 satellite
	terre.Satellites.push_back(lune);
									
	// Jupiter a 2 satellites
	jupiter.Satellites.push_back(europa);
	jupiter.Satellites.push_back(callisto);
	
	// Le Soleil a 2 planetes
	soleil.Systeme.push_back(terre);
	soleil.Systeme.push_back(jupiter);
															
   phi = DEG2RAD(75.0), theta = DEG2RAD(60.0);
   dist = 13.0;
   utiliseNuanceurs = false;
   sphere = gluNewQuadric();

   // donner la couleur de fond
   glClearColor( 0.0, 0.0, 0.0, 1.0 );

   // activer les etats openGL
   glEnable( GL_NORMALIZE );
   glEnable( GL_DEPTH_TEST );

   // charger les nuanceurs
   chargerNuanceurs();
}

void afficherModele()
{
    glUseProgram( 0 );

   // afficher le système solaire
   glPushMatrix( ); {

      soleil.afficher(utiliseNuanceurs, progNuanceur, facteurRechauffement); // soleil

   } glPopMatrix( );
}

void afficherScene( )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity( );
   gluPerspective( 70.0, (GLdouble) g_largeur / (GLdouble) g_hauteur, 0.1, 100.0 );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity( );

   if (vuePole) {
       // Idealement, devrait etre gere avec la matrice drawMatrix attribut de Planete
       GLdouble inv[16], m[16];
       glPushMatrix();
           glRotatef(Revolution[1], 0, 0, 1);
           glTranslatef(terre.Distance, 0, 0 );
           glRotatef(Rotation[1], 0, 0, 1);
           glRotatef(90, 1, 0, 0);
           glTranslatef(0, terre.Rayon * 1.1, 0);
           glGetDoublev (GL_MODELVIEW_MATRIX, m);
       glPopMatrix();
       Matrice::inverse(inv, m);
       glLoadMatrixd(inv);


//       GLdouble inv[16];
//       inverse(inv, terre.drawMatrix);
//       glLoadMatrixd(inv);

   } else {
        glTranslatef( 0, 0, -dist );
        gluLookAt(
            dist*sin((phi))*cos((theta)),
            dist*sin((phi))*sin((theta)),
            dist*cos((phi)),
            0, 0, 0,
            0, 0, 1 );
   }

   // dessiner la scène
   afficherModele();

   glutSwapBuffers( );
}

void redimensionnement( GLsizei w, GLsizei h )
{
   g_largeur = w;
   g_hauteur = h;
   glViewport( 0, 0, w, h );
   glutPostRedisplay();
}

void verifierAngles()
{
   if ( theta > 2*M_PI )
      theta -= 2*M_PI;
   else if ( theta < -2*M_PI )
      theta += 2*M_PI;
   if ( phi > MAXPHI )
      phi = MAXPHI;
   else if ( phi < MINPHI )
      phi = MINPHI;
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

   case 'x':
       utiliseNuanceurs = !utiliseNuanceurs;
      break;

   case 'v':
       chargerNuanceurs();
      break;

   case 'c':
      facteurRechauffement += 0.05; if ( facteurRechauffement > 1.0 ) facteurRechauffement = 1.0;
      std::cout << " facteurRechauffement=" << facteurRechauffement << " " << std::endl;
      break;
   case 'f':
      facteurRechauffement -= 0.05; if ( facteurRechauffement < 0.0 ) facteurRechauffement = 0.0;
      std::cout << " facteurRechauffement=" << facteurRechauffement << " " << std::endl;
      break;

   case '0':
       vuePole = false;
       break;
   case '1':
       vuePole = true;
       break;
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      vuePole = !vuePole;
      break;

   case 'g':
      {
         static bool modePlein = true;
         modePlein = !modePlein;
         if ( modePlein )
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
         else
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      }
      break;
   }
}

void clavierSpecial( int touche, int x, int y )
{
}

bool sourisMouvementActif = false;
int lastx = 0, lasty = 0;
void sourisClic( int button, int state, int x, int y )
{
   if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !vuePole)
   {
      lastx = x;
      lasty = y;
      sourisMouvementActif = true;
   }
   else
      sourisMouvementActif = false;
}

void sourisMouvement( int x, int y )
{
   if ( sourisMouvementActif )
   {
      theta += (x-lastx) / 30.0;
      phi += (y-lasty) / 25.0;
      lastx = x;
      lasty = y;
      verifierAngles();
   }
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

   glewInit();

   initialisation();
   animer( 0 );

   // boucle principale de gestion des evenements
   glutMainLoop();

   // le programme n'arrivera jamais jusqu'ici
   return EXIT_SUCCESS;
}
