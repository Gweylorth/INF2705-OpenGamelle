// Prénoms, noms et matricule des membres de l'équipe:
// - Prénom1 NOM1 (matricule1)
// - Prénom2 NOM2 (matricule2)

#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include "varglob.h"
#include "float3.h"

float gravite = 0.5;

class Particule {
public:
    GLfloat position[3];          // en unités
    GLfloat vitesse[3];           // en unités/seconde
    GLfloat couleur[4];           // RGBA
    GLfloat couleurOriginale[4];
    GLfloat tempsDeVieRestant;    // en secondes

    bool estSelectionee = false;

    GLuint nom;

    Particule(float, float, float, int);
    void avancer(const int);
    void collision();
    void vivre(const int);
    void selectionner();
};

Particule::Particule(float x, float y, float z, int nom)
    : nom(nom)
{
    this->position[0] = x;
    this->position[1] = y;
    this->position[2] = z;

    this->vitesse[0] = static_cast<float>(rand() % 100 - rand() % 100) / 100000;
    this->vitesse[1] = static_cast<float>(rand() % 100 + 40) / 50000;
    this->vitesse[2] = static_cast<float>(rand() % 100 - rand() % 100) / 100000;

    this->couleur[0] = static_cast<float>(rand() % 101) / 100.;
    this->couleur[1] = static_cast<float>(rand() % 101) / 100.;
    this->couleur[2] = static_cast<float>(rand() % 101) / 100.;
    this->couleur[3] = 1.;

    // De 0.1 a 2.1 secondes
    this->tempsDeVieRestant = static_cast<float>(rand() % 20 + 1) / 10.;
}

void Particule::avancer(const int deltaT) {
    // position = vitesse * deltaT
    // vitesse = vitesse - gravite * deltaT

    for (int i = 0; i < 3; i++) {
        this->position[i] += this->vitesse[i] * deltaT;
    }

    this->vitesse[1] -= (gravite / 100000) * deltaT;
    this->vivre(deltaT);
    this->collision();
}

void Particule::collision() {
    // murs
    if ( fabs(this->position[0]) >= 0.95) {
        this->vitesse[0] = - this->vitesse[0];
    }
    if ( fabs(this->position[2]) >= 0.95) {
        this->vitesse[2] = - this->vitesse[2];
    }

    // sol
    if ( this->position[1] <= 0.1) {
        this->vitesse[1] = - this->vitesse[1];
    }
}

void Particule::vivre(const int deltaT) {
    float delta = deltaT / 1000.;
    this->tempsDeVieRestant -= delta;
}

void Particule::selectionner(){
    this->estSelectionee = !this->estSelectionee;
    if (estSelectionee) {
        std::copy(this->couleur, std::end(this->couleur), this->couleurOriginale);
        GLfloat black [] = {0.,0.,0.,1.};
        std::copy(black, std::end(black), this->couleur);
    } else {
        std::copy(this->couleurOriginale, std::end(this->couleurOriginale), this->couleur);
    }
}

const int MAXNPARTICULES = 100000;
const float TAILLEPARTICULES = 3.1416;
int nparticules = 30;                // nombre de particules utilisées (affichées)
std::vector<Particule> particules;
GLuint selBuffer[MAXNPARTICULES];

void avancerParticules( const int deltaT )
{
   for ( std::vector<Particule>::iterator it = particules.begin(); it != particules.end(); it++)
   {
      it->avancer(deltaT);
      if (it->tempsDeVieRestant <= 0.) {
          *it = Particule(0., .5, 0., it->nom);
      }
   }
}

int deltaTemps=0;
static void animer( int tempsPrec )
{
   // obtenir le temps depuis le début du programme, en millisecondes
   int tempsCour = glutGet( GLUT_ELAPSED_TIME );
   if ( tempsPrec == 0 ) tempsPrec = tempsCour;

   // temps d'attente en secondes avant le prochain affichage
   const int FPS = 60;  // en "images/seconde"
   const int delai = 1000/FPS;  // en "millisecondes/image" (= 1000 millisecondes/seconde  /  images/seconde)
   if ( enmouvement ) glutTimerFunc( delai, animer, tempsCour );

   deltaTemps = tempsCour - tempsPrec;

   // déplacer les particules
   avancerParticules( deltaTemps );

   // indiquer qu'il faut afficher à nouveau
   glutPostRedisplay();
}

void initialiserParticules () {
    particules.clear();
    for ( int i = 0 ; i < nparticules ; i++ )
    {
       particules.push_back( Particule(0., .5, 0., i) );
    }
}

void initialisation()
{
    srand (time(NULL));

   enmouvement = true;

   // donner la position de la caméra
   theta = 0.0;
   phi = 1.0;
   dist = 5.0;

   // donner la couleur de fond
   glClearColor( 0.0, 0.0, 0.0, 1.0 );

   // activer les etats openGL
   glEnable( GL_DEPTH_TEST );

   initialiserParticules();

   glewInit();
}

void afficherParticules( )
{
    glEnable( GL_POINT_SMOOTH );
    glPointSize( TAILLEPARTICULES );
    glInitNames();
    glPushName(0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    for ( Particule& particule : particules )
    {
        glLoadName(particule.nom + 1);
        glVertexPointer(3, GL_FLOAT, 0, particule.position);
        glColorPointer(4, GL_FLOAT, 0, particule.couleur);
        glDrawArrays(GL_POINTS, 0, 1);
        // Okay, c'est de la triche, mais on ne trouve pas comment executer un traitement
        // entre chaque dessin de DrawArray pour donner un nom a la particule
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void selectionParticule(float x, float y) {

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    GLint cloture[4];
    glGetIntegerv( GL_VIEWPORT, cloture );
    gluPickMatrix( x, cloture[3]-y, 2*TAILLEPARTICULES, 2*TAILLEPARTICULES, cloture );
    gluPerspective( 45.0, (GLdouble) g_largeur / (GLdouble) g_hauteur, 0.1, 10.0 );
    glMatrixMode( GL_MODELVIEW );

    // declarer le tampon de selection
    glSelectBuffer(MAXNPARTICULES, selBuffer);

    // Faire la sélection
    glRenderMode( GL_SELECT );
    afficherParticules();

    int nbObjets = glRenderMode( GL_RENDER );

    // Imprimer le résultat de la sélection
    GLuint* ptr = selBuffer;
    ++ptr;
    ++ptr;
    ++ptr;
    for( int iobj = 0; iobj < nbObjets; iobj++ )
    {
        //std::cout << "Nom : " << *ptr << std::endl;

        // garde-fou
        if (*ptr < MAXNPARTICULES) {
            particules[*ptr - 1].selectionner();
        }

        ptr++;
    }

    // Remettre les valeurs initiales
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
}

void afficherScene()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluPerspective( 45.0, (GLdouble) g_largeur / (GLdouble) g_hauteur, 0.1, 10.0 );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   gluLookAt( dist*sin(phi)*sin(theta), dist*cos(phi), dist*sin(phi)*cos(theta), 0, 1, 0, 0, 2, 0 );

   glBegin( GL_QUADS ); {
      glColor3f( 1.0, 1.0, 1.0 );
      glVertex3f( -1,  0,  1 );
      glVertex3f(  1,  0,  1 );
      glVertex3f(  1,  0, -1 );
      glVertex3f( -1,  0, -1 );

      glColor3f( 1.0, 1.0, 0.0 );
      glVertex3f(  1, 2,  1 );
      glVertex3f(  1, 2, -1 );
      glVertex3f(  1, 0, -1 );
      glVertex3f(  1, 0,  1 );

      glColor3f( 1.0, 0.0, 1.0 );
      glVertex3f( -1, 2,  1 );
      glVertex3f( -1, 2, -1 );
      glVertex3f( -1, 0, -1 );
      glVertex3f( -1, 0,  1 );

      glColor3f( 0.0, 1.0, 1.0 );
      glVertex3f(  1, 2, -1 );
      glVertex3f( -1, 2, -1 );
      glVertex3f( -1, 0, -1 );
      glVertex3f(  1, 0, -1 );
   } glEnd();

   afficherParticules( );

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
   case ' ':
      enmouvement = !enmouvement;
      if ( enmouvement ) animer( 0 );
      glutPostRedisplay();
      break;
   case '[':
      nparticules /= 1.2;
      if ( nparticules < 5 ) nparticules = 5;
      initialiserParticules();
      std::cout << " nparticules=" << nparticules << std::endl;
      glutPostRedisplay();
      break;
   case ']':
      nparticules *= 1.2;
      if ( nparticules > MAXNPARTICULES ) nparticules = MAXNPARTICULES;
      initialiserParticules();
      std::cout << " nparticules=" << nparticules << std::endl;
      glutPostRedisplay();
      break;
   case 'a':
      gravite += 0.05;
      std::cout << " gravite=" << gravite << std::endl;
      glutPostRedisplay();
      break;
   case 'd':
      gravite -= 0.05;
      if ( gravite < 0.0 ) gravite = 0.0;
      std::cout << " gravite=" << gravite << std::endl;
      glutPostRedisplay();
      break;
   case 'p':
       for ( std::vector<Particule>::iterator it = particules.begin(); it != particules.end(); it++)
       {
          if (it->estSelectionee) {
              *it = Particule(0., .5, 0., it->nom);
          }
       }
      glutPostRedisplay();
      break;
   }
}

void clavierSpecial( int touche, int x, int y )
{
}

int lastx=0, lasty=0;
float mouseSelX = 0, mouseSelY = 0;
bool mouvementActif = true;
void sourisClic( int button, int state, int x, int y )
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
   {
      lastx = x;
      lasty = y;
      mouvementActif = true;
   } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
   {
      selectionParticule(x, y);
      mouvementActif = false;
      glutPostRedisplay();
   }
}

void sourisMouvement( int x, int y )
{
    if (!mouvementActif) {
        return;
    }

   theta += (lastx-x) / 100.0;
   phi += (lasty-y) / 50.0;
   lastx = x;
   lasty = y;

   if ( phi >= M_PI )
      phi = M_PI - 0.001;
   else if ( phi <= 0 )
      phi = 0.001;

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
   if ( enmouvement ) animer( 0 );

   // boucle principale de gestion des evenements
   glutMainLoop();

   // le programme n'arrivera jamais jusqu'ici
   return EXIT_SUCCESS;
}
