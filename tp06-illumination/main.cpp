// Prénoms, noms et matricule des membres de l'équipe:
// - Prénom1 NOM1 (matricule1)
// - Prénom2 NOM2 (matricule2)

#include <stdlib.h>
#include <iostream>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <algorithm>
#include "varglob.h"
#include "fctsUtiles.h"
#include "chargertex.h"

////////////////////////////////////////
// déclaration des variables globales //
////////////////////////////////////////

const float freqRotX  = 0.15;     // la fréquence de rotation en X du modèle
const float freqRotY  = 0.1;      // la fréquence de rotation en Y du modèle
const float freqRotZ  = 0.2;      // la fréquence de rotation en Z du modèle

const float frameRate = 50.0;     // le framerate cible de l'application graphique

const float factAxes  = 15.0;     // le facteur multiplicatif des axes unitaires dessinés dans la scène

GLfloat spotAngle       = 15.0;     // l'angle du cône du spot
GLfloat spotExposant    = 1.0;      // l'exposant du cône

int modele = 1;
bool utiliseBlinn = false;       // indique si on veut utiliser Blinn ou Phong dans le nuanceur
bool utiliseDirect = false;      // indique si on utilise un spot style Direct3D
bool localViewer = false;        // la valeur de GL_LIGHT_MODEL_LOCAL_VIEWER

int texnumero = 0;               // numéro de la texture appliquée
int texwrapmode = 0;             // paramètres de répétition de la texture
bool utiliseCouleur = true;      // doit-on utiliser la oculeur de base de l'objet en plus de celle de la texture?
bool noirTransparent = false;    // les pixels noirs doivent-ils être transparents?

GLuint textureDE = 0;
GLuint textureECHIQUIER = 0;

// Définition des lumières OpenGL
GLfloat spotPosition[4]  = { 4, 1, 10, 1.0 };
GLfloat spotDirection[3] = { -5, -2, -10 };
GLfloat lumiAmbiant1[]    = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lumiDiffuse1[]    = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lumiSpeculaire1[] = { 1.0, 1.0, 1.0, 1.0 };


#include <iostream>
#include <array>
#include <string>
#include <vector>


/* Trouve ici : http://stackoverflow.com/questions/19780788/opengl-c-plain-subdivison-in-quads-radiosity-patches-on-arrays
 * Source : http://coliru.stacked-crooked.com/a/b01db8e00f50c872
 */
using Position3 = std::array<float, 3>;
using Plane = std::array<Position3, 4>;

Position3& operator *= (Position3& pos, const float scale)
{
    for (auto& x : pos) x *= scale;
    return pos;
}

Position3 operator * (const Position3& pos, const float scale)
{
    Position3 result = pos;
    result *= scale;
    return result;
}

Position3& operator += (Position3& left, const Position3& right)
{
    for (size_t i = 0; i < 3; i++)
        left[i] += right[i];

    return left;
}

Position3 operator + (const Position3& left, const Position3& right)
{
    Position3 result = left;
    result += right;
    return result;
}

Position3& operator -= (Position3& left, const Position3& right)
{
    for (size_t i = 0; i < 3; i++)
        left[i] -= right[i];

    return left;
}

Position3 operator - (const Position3& left, const Position3& right)
{
    Position3 result = left;
    result -= right;
    return result;
}

std::vector<Plane> subdivide(const Plane& plane, size_t iterations = 1)
{
    if (iterations == 0)
    {
        return{ plane };
    }
    else
    {
        Position3 e1 = plane[1] - plane[0];
        Position3 e2 = plane[2] - plane[3];
        Position3 e3 = plane[3] - plane[0];
        Position3 e4 = plane[2] - plane[1];

        Position3 p1 = e1 * 0.5f + plane[0];
        Position3 p2 = e2 * 0.5f + plane[3];
        Position3 p3 = e3 * 0.5f + plane[0];
        Position3 p4 = e4 * 0.5f + plane[1];

        Position3 e5 = p2 - p1;
        Position3 p5 = e5 * 0.5f + p1;

        std::vector<Plane> result{
            { plane[0], p1, p5, p3 },
            { p1, plane[1], p4, p5 },
            { p5, p4, plane[2], p2 },
            { p3, p5, p2, plane[3] }
        };

        if (iterations == 1)
        {
            return result;
        }
        else
        {
            std::vector<Plane> result2;
            for (auto& x : result)
            {
                auto subplanes = subdivide(x, iterations - 1);
                result2.insert(result2.end(), subplanes.begin(), subplanes.end());
            }
            return result2;
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

   // indiquer qu'il faut afficher à nouveau
   glutPostRedisplay();
}

void chargerToutesLesTextures()
{
   ChargerTexture( "textures/de.bmp", textureDE );
   ChargerTexture( "textures/echiquier.bmp", textureECHIQUIER );
}

void chargerNuanceurs()
{
   // charger les nuanceurs
   const char *ns = "nuanceurs/nuanceurSommets.glsl";
   const char *nf = "nuanceurs/nuanceurFragments.glsl";
   progNuanceur = initialiserNuanceurs( ns, nf );
}

// initialisation d'openGL
void initialisation()
{
   dist = 30.0;

   // couleur de l'arrière-plan
   glClearColor( 0.4, 0.2, 0.0, 1.0 );

   // activer les etats openGL
   glEnable( GL_NORMALIZE );
   glEnable( GL_DEPTH_TEST );
   glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

   //glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE ); // un bonus!
   glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );     // "two-side mode" en openGL
   glEnable( GL_VERTEX_PROGRAM_TWO_SIDE );                // "two-side mode" en GLSL

   // charger les textures
   chargerToutesLesTextures();

   // charger les nuanceurs
   chargerNuanceurs();
}

// fonction pour l'éclairage
void definirEclairage()
{
    glLightfv( GL_LIGHT0, GL_AMBIENT, lumiAmbiant1 );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lumiDiffuse1 );
    glLightfv( GL_LIGHT0, GL_SPECULAR, lumiSpeculaire1 );
    glLightfv( GL_LIGHT0, GL_POSITION, spotPosition );
    glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection );
    glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, spotExposant );
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle );

    glEnable( GL_LIGHT0 );
}

void afficherModele()
{
   // parametres de texture
   switch ( texnumero )
   {
   default:
      //std::cout << "Sans texture" << std::endl;
      break;
   case 1:
      std::cout << "Texture DE" << std::endl;
      break;
   case 2:
      std::cout << "Texture ECHIQUIER" << std::endl;
      break;
   }

   if ( texnumero ) // si on utilise une texture
   {
      std::cout << " utiliseCouleur=" << utiliseCouleur << std::endl;
      std::cout << " texwrapmode=" << texwrapmode << std::endl;
   }

   if ( utiliseNuanceurs )
   {
      glUseProgram( progNuanceur );
      //glUniform3f( glGetUniformLocation( progNuanceur, "eyePos" ), x, y, z );
      glUniform1i( glGetUniformLocation( progNuanceur, "utiliseBlinn" ), utiliseBlinn );
      glUniform1i( glGetUniformLocation( progNuanceur, "utiliseDirect" ), utiliseDirect );
      glUniform1i( glGetUniformLocation( progNuanceur, "localViewer" ), localViewer );
      //glActiveTexture( GL_TEXTURE0 ); // activer la texture '0' (valeur de défaut)
      //glUniform1i( glGetUniformLocation( progNuanceur, "laTexture" ), 0 ); // '0' => utilisation de GL_TEXTURE0
      //glUniform1i( glGetUniformLocation( progNuanceur, "utiliseCouleur" ), utiliseCouleur );
      //glUniform1i( glGetUniformLocation( progNuanceur, "texnumero" ), texnumero );
      //glUniform1i( glGetUniformLocation( progNuanceur, "noirTransparent" ), noirTransparent );
   }
   else
   {
      glUseProgram( 0 );
   }

   glEnable( GL_LIGHTING );

   // Appliquer le matériau
   GLfloat mat_ambiant_modele[]  = { 0.1, 0.0, 0.1, 1.0 };
   GLfloat mat_diffuse_modele[]  = { 1.0, 0.1, 1.0, 1.0 };
   GLfloat mat_specular_modele[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_emission_modele[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat mat_shininess_modele[] = { 100.0 };

   glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambiant_modele );
   glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_modele );
   glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_modele );
   glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat_emission_modele );
   glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess_modele );

   // Dessiner le modèle
   glPushMatrix(); {

      // dessiner le modèle (en tournant, si désiré)
      if ( enmouvement )
      {
         angleRotX += 360.0 * ( freqRotX / frameRate );
         angleRotY += 360.0 * ( freqRotY / frameRate );
         angleRotZ += 360.0 * ( freqRotZ / frameRate );
         if ( angleRotX >= 360.0 ) angleRotX -= 360.0;
         if ( angleRotY >= 360.0 ) angleRotY -= 360.0;
         if ( angleRotZ >= 360.0 ) angleRotZ -= 360.0;
      }
      glRotated( angleRotX, 1.0, 0.0, 0.0 );
      glRotated( angleRotY, 0.0, 1.0, 0.0 );
      glRotated( angleRotZ, 0.0, 0.0, 1.0 );

      // mise à l'échelle
      glScalef( 5.0, 5.0, 5.0 );

      switch ( modele )
      {
      default:
      case 1: // cube
         {
            /*         +Y                    */
            /*   3+-----------+2             */
            /*    |\          |\             */
            /*    | \         | \            */
            /*    |  \        |  \           */
            /*    |  7+-----------+6         */
            /*    |   |       |   |          */
            /*    |   |       |   |          */
            /*   0+---|-------+1  |          */
            /*     \  |        \  |     +X   */
            /*      \ |         \ |          */
            /*       \|          \|          */
            /*       4+-----------+5         */
            /*             +Z                */

          GLfloat sommets[3*4*5] =
          {
             -1.0,  1.0, -1.0,    1.0,  1.0, -1.0,    1.0, -1.0, -1.0,   -1.0, -1.0, -1.0,  // P3,P2,P1,P0
              1.0, -1.0,  1.0,   -1.0, -1.0,  1.0,   -1.0, -1.0, -1.0,    1.0, -1.0, -1.0,  // P5,P4,P0,P1
              1.0,  1.0,  1.0,    1.0, -1.0,  1.0,    1.0, -1.0, -1.0,    1.0,  1.0, -1.0,  // P6,P5,P1,P2
             -1.0,  1.0,  1.0,    1.0,  1.0,  1.0,    1.0,  1.0, -1.0,   -1.0,  1.0, -1.0,  // P7,P6,P2,P3
             -1.0, -1.0,  1.0,   -1.0,  1.0,  1.0,   -1.0,  1.0, -1.0,   -1.0, -1.0, -1.0,  // P4,P7,P3,P0
//             -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,    1.0,  1.0,  1.0,   -1.0,  1.0,  1.0   // P4,P5,P6,P7
          };

            // afficher le cube
            glEnableClientState( GL_VERTEX_ARRAY );
            //glEnableClientState( GL_NORMAL_ARRAY );
            //glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glVertexPointer( 3, GL_FLOAT, 0, sommets );
            //glNormalPointer( ... );
            //glTexCoordPointer( ... );
            glDrawArrays( GL_QUADS, 0, 4*5 );
            glDisableClientState( GL_VERTEX_ARRAY );
            //glDisableClientState( GL_NORMAL_ARRAY );
            //glDisableClientState( GL_TEXTURE_COORD_ARRAY );

            Plane p = { { {-1.0, -1.0,  1.0 }, { 1.0, -1.0,  1.0 }, { 1.0,  1.0,  1.0 }, { -1.0,  1.0,  1.0 } } };
            auto subd = subdivide(p, 4);
            for (auto& quad : subd) {
                glBegin(GL_QUADS);
                    glVertex3fv(quad[3].begin());
                    glVertex3fv(quad[2].begin());
                    glVertex3fv(quad[1].begin());
                    glVertex3fv(quad[0].begin());
                glEnd();
            }


// Algo de gamedev : http://gamedev.stackexchange.com/questions/57095/how-to-subdivide-a-quad
//            GLfloat a[3] = {-1.0, -1.0,  1.0};
//            GLfloat b[3] = {1.0, -1.0,  1.0};
//            GLfloat c[3] = {1.0, 1.0,  1.0};
//            GLfloat d[3] = {-1.0, 1.0,  1.0};
//            GLfloat p1[3], p2[3], q1[3], q2[3];
//            GLfloat a2[3], b2[3], c2[3], d2[3];
//            GLfloat face[4*3];
//            for (int i = 0; i < 100; i++) {
//                for (int k = 0; k < 3; k++) {
//                    p1[k] = a[k] + i * (d[k] - a[k]) / 3.;
//                    p2[k] = b[k] + i * (c[k] - b[k]) / 3.;
//                    q1[k] = a[k] + (i + 1) * (d[k] - a[k]) / 3.;
//                    q2[k] = b[k] + (i + 1) * (c[k] - b[k]) / 3.;
//                }

//                for (int j = 0; j < 100; j++) {
//                    for (int k = 0; k < 3; k++) {
//                        a2[k] = p1[k] + j * (p2[k] - p1[k]) / 3.;
//                        b2[k] = p1[k] + (j + 1) * (p2[k] - p1[k]) / 3.;
//                        c2[k] = q1[k] + (j + 1) * (q2[k] - q1[k]) / 3.;
//                        d2[k] = q1[k] + j * (q2[k] - q1[k]) / 3.;
//                    }
//                }

//                for (int k = 0; k < 3; k++) {
//                    face[k] = a2[k];
//                    face[3 + k] = b2[k];
//                    face[6 + k] = c2[k];
//                    face[9 + k] = d2[k];
//                }

//                glEnableClientState( GL_VERTEX_ARRAY );
//                glVertexPointer( 3, GL_FLOAT, 0, face );
//                glDrawArrays( GL_QUADS, 0, 4 );
//                glDisableClientState( GL_VERTEX_ARRAY );
//            }



         }
         break;
      case 2:
         glPushAttrib( GL_POLYGON_BIT ); {
            glFrontFace( GL_CW ); // les polygones de la théière ont leur face avant vers l'intérieur: il faut inverser
            glutSolidTeapot( 1.0 );  // définit automatiquement des coordonnées de texture :)
         } glPopAttrib( );
         break;
      case 3:
         glutSolidTorus( 0.3, 0.6, 32, 32 ); // ne définit aucune coordonnées de texture :(
         break;
      case 4:
         glutSolidSphere( 0.6, 32, 32 ); // ne définit aucune coordonnées de texture :(
         break;
      case 5:
         glPushMatrix(); {
            glScalef( .7, .7, .7 );
            glutSolidDodecahedron(); // ne définit aucune coordonnées de texture :(
         } glPopMatrix();
         break;
      case 6:
         glPushMatrix(); {
            glScalef( 1.2, 1.2, 1.2 );
            glutSolidIcosahedron(); // ne définit aucune coordonnées de texture :(
         } glPopMatrix();
         break;
      }
   } glPopMatrix();

   glDisable( GL_TEXTURE_2D );
   glDisable( GL_LIGHTING );
   glUseProgram( 0 );
}

void afficherAxes()
{
   // Dessiner les axes pour se retrouver
   glPushAttrib( GL_LINE_BIT ); {

      glLineWidth( 3.0 );

      // tracer les trois axes ET une ligne vers la source lumineuse
      GLfloat coords[] =
      {
         0.0, 0.0, 0.0,  factAxes, 0.0, 0.0,
         0.0, 0.0, 0.0,  0.0, factAxes, 0.0,
         0.0, 0.0, 0.0,  0.0, 0.0, factAxes,
         spotPosition[0], spotPosition[1], spotPosition[2],
         spotPosition[0]+spotDirection[0], spotPosition[1]+spotDirection[1], spotPosition[2]+spotDirection[2]
      };
      GLfloat couleurs[] =
      {
         1.0, 0.0, 0.0,  1.0, 0.0, 0.0,
         0.0, 1.0, 0.0,  0.0, 1.0, 0.0,
         0.0, 0.0, 1.0,  0.0, 0.0, 1.0,
         1.0, 1.0, 1.0,  1.0, 1.0, 1.0
      };
      glEnableClientState( GL_VERTEX_ARRAY );
      glEnableClientState( GL_COLOR_ARRAY );
      glVertexPointer( 3, GL_FLOAT, 0, coords  );
      glColorPointer( 3, GL_FLOAT, 0, couleurs );
      glDrawArrays( GL_LINES, 0, 8 );
      glDisableClientState( GL_VERTEX_ARRAY );
      glDisableClientState( GL_COLOR_ARRAY );

      // tracer la source lumineuse
      glPushMatrix(); {
         glTranslatef( spotPosition[0], spotPosition[1], spotPosition[2] );
         glColor3f( 1.0, 1.0, 0.5 );
         glutSolidSphere( 0.5, 10, 10 );
      } glPopMatrix();

   } glPopAttrib();
}

// fonction d'affichage
void afficherScene()
{
   // effacer l'ecran et le tampon de profondeur
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // mettre à jour la projection
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   if ( enPerspective )
   {
      gluPerspective( 40.0, (GLdouble)g_largeur / (GLdouble)g_hauteur,
                      0.1, 60.0 );
   }
   else
   {
      const GLfloat d = 10.0;
      if ( g_largeur <= g_hauteur )
         glOrtho( -d, d,
                  -d*(GLdouble)g_hauteur / (GLdouble)g_largeur,
                  d*(GLdouble)g_hauteur / (GLdouble)g_largeur,
                  0.1, 60.0 );
      else
         glOrtho( -d*(GLdouble)g_largeur / (GLdouble)g_hauteur,
                  d*(GLdouble)g_largeur / (GLdouble)g_hauteur,
                  -d, d,
                  0.1, 60.0 );
   }

   // mettre à jour la caméra
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   gluLookAt( -0.1*dist, 0.1*dist, 1.0*dist,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0 );

   // mettre à jour l'éclairage
   definirEclairage();

   // dessiner la scène
   afficherModele();
   afficherAxes();

   glutSwapBuffers();
}

// fonction de redimensionnement de la fenêtre graphique
void redimensionnement( GLsizei w, GLsizei h )
{
   g_largeur = w;
   g_hauteur = h;
   glViewport( 0, 0, w, h );
   glutPostRedisplay();
}

// fonction de gestion du clavier
void clavier( unsigned char touche, int x, int y )
{
   // traitement des touches q et echap
   switch ( touche )
   {
   case '\e': // escape
      glutDestroyWindow( g_feneID );
      exit( 0 );
      break;

   case 'p':
      enPerspective = !enPerspective;
      glutPostRedisplay();
      break;

   case 'x': // permutation de l'activation des nuanceurs
      utiliseNuanceurs = !utiliseNuanceurs;
      std::cout << "// Utilisation des nuanceurs ? " << ( utiliseNuanceurs ? "OUI" : "NON" ) << std::endl;
      glutPostRedisplay();
      break;

   case 'v': // Recharger les nuanceurs
      chargerNuanceurs();
      std::cout << "// Recharger nuanceurs" << std::endl;
      glutPostRedisplay();
      break;

   case 'b': // Phong ou Blinn
      utiliseBlinn = !utiliseBlinn;
      std::cout << " utiliseBlinn=" << utiliseBlinn << std::endl;
      glutPostRedisplay();
      break;

   case 'd': // permutation du modèle de spot Direct3D
      utiliseDirect = !utiliseDirect;
      std::cout << " utiliseDirect=" << utiliseDirect << std::endl;
      glutPostRedisplay();
      break;

   case 'l': // local viewer
      localViewer = !localViewer;
      std::cout << " localViewer=" << localViewer << std::endl;
      glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer );
      glutPostRedisplay();
      break;

   case '-':
   case '_':
      spotAngle -= 2.0;
      if ( spotAngle < 0.0 ) spotAngle = 0.0;
      std::cout <<  " spotAngle=" << spotAngle << std::endl;
      glutPostRedisplay();
      break;
   case '=':
   case '+':
      spotAngle += 2.0;
      if ( spotAngle > 90.0 ) spotAngle = 90.0;
      std::cout <<  " spotAngle=" << spotAngle << std::endl;
      glutPostRedisplay();
      break;

   case '.':
      spotExposant -= 0.3;
      if ( spotExposant < 0.0 ) spotExposant = 0.0;
      std::cout <<  " spotExposant=" << spotExposant << std::endl;
      glutPostRedisplay();
      break;
   case '/':
      spotExposant += 0.3;
      if ( spotExposant > 89.0 ) spotExposant = 89.0;
      std::cout <<  " spotExposant=" << spotExposant << std::endl;
      glutPostRedisplay();
      break;

   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
      modele = touche - '0';
      glutPostRedisplay();
      break;

   case '0': // placer les choses afin d'avoir une belle vue
      angleRotX = angleRotY = angleRotZ = 0.0;
      glutPostRedisplay();
      break;

   case 't':
      texnumero++;
      if ( texnumero > 2 ) texnumero = 0;
      std::cout << " texnumero=" << texnumero << std::endl;
      glutPostRedisplay();
      break;

   case 'w':
      texwrapmode++;
      texwrapmode = texwrapmode%4;
      std::cout << " texwrapmode=" << texwrapmode << std::endl;
      glutPostRedisplay();
      break;

   case 'c':
      utiliseCouleur = !utiliseCouleur;
      std::cout << " utiliseCouleur=" << utiliseCouleur << std::endl;
      glutPostRedisplay();
      break;

   case 'n':
      noirTransparent = !noirTransparent;
      std::cout << " noirTransparent=" << noirTransparent << std::endl;
      glutPostRedisplay();
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
      glutPostRedisplay();
      break;

   case ' ' :
      // permuter la rotation automatique du modèle
      enmouvement = !enmouvement;
      if ( enmouvement )
         animer( 0 );
      else
      {
         angleRotX = angleRotY = angleRotZ = 0.0;
      }
      glutPostRedisplay();
      break;

   }
}

// fonction de gestion du clavier spécial
void clavierSpecial( int touche, int x, int y )
{
}

// fonction callback pour un clic de souris
int dernierX = 0; // la dernière valeur en X de position de la souris
int dernierY = 0; // la derniere valeur en Y de position de la souris
static enum { deplaceCam, deplaceSpotDirection, deplaceSpotPosition } deplace = deplaceCam;
void sourisClic( int button, int state, int x, int y )
{
   if ( state == GLUT_DOWN )
   {
      // on vient de presser la souris
      dernierX = x;
      dernierY = y;
      switch ( button )
      {
      case GLUT_LEFT_BUTTON:
         deplace = deplaceCam;
         break;
      case GLUT_MIDDLE_BUTTON:
         deplace = deplaceSpotDirection;
         break;
      case GLUT_RIGHT_BUTTON:
         deplace = deplaceSpotPosition;
         break;
      }
   }
   else
   {
      // on vient de relacher la souris
   }
}

// fonction de mouvement de la souris
void sourisMouvement( int x, int y )
{
   int dx = x - dernierX;
   int dy = y - dernierY;
   switch ( deplace )
   {
   case deplaceCam:
      angleRotX += dy;
      angleRotY += dx;
      break;
   case deplaceSpotDirection:
      spotDirection[0] += 0.05 * dx;
      spotDirection[1] -= 0.05 * dy;
      break;
   case deplaceSpotPosition:
      spotPosition[0] += 0.05 * dx;
      spotPosition[1] -= 0.05 * dy;
      break;
   }

   dernierX = x;
   dernierY = y;

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

   glewInit();

   initialisation();
   if ( enmouvement ) animer( 0 );

   // boucle principale de gestion des evenements
   glutMainLoop();

   // le programme n'arrivera jamais jusqu'ici
   return EXIT_SUCCESS;
}
