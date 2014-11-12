#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/glut.h>
#include "fctsUtiles.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// La fonction glGetError() permet de savoir si une erreur est survenue depuis le dernier appel à cette fonction.
// De plus, la fonction gluErrorString() retourne un message d'erreur qui peut être affiché à l'écran.
// Vous pouvez aller voir dans Aide.cpp (d'INF2990) ou utiliser cette version:

int VerifierErreurGL( const std::string endroit )
{
   int rc = 0;
   GLenum err;
   while ( ( err = glGetError() ) != GL_NO_ERROR )
   {
       const GLubyte *texte = gluErrorString( err );
       std::cerr << "Erreur OpenGL en cet endroit " << endroit
       		 << " erreur = " << err
             	 << " " << texte << std::endl;

      ++rc;
   }
   return( rc );
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// La fonction textFileRead() permet de charger en mémoire le contenu d'un
// fichier en vue de l'utiliser comme nuanceur

static char *lireNuanceur( const char *fn )
{
   char *contenu = NULL;
   if ( fn != NULL )
   {
      FILE *fp = fopen( fn, "r" );
      if ( fp == NULL )
      {
         perror( fn );
      }
      else
      {
         fseek( fp, 0, SEEK_END );
         int taille = ftell(fp);
         if ( taille > 0 )
         {
            rewind(fp);
            contenu = new char[taille+1];
            taille = fread( contenu, sizeof(char), taille, fp );
            contenu[taille] = '\0';
         }
         fclose( fp );
      }
   }
   return contenu;
}

// Fonctions pour les nuanceurs

static void afficherShaderInfoLog( GLuint obj, std::string message )
{
   // afficher le message d'en-tête
   std::cout << message << std::endl;

   // afficher le message d'erreur, le cas échéant
   int infologLength = 0;
   glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

   if ( infologLength > 1 )
   {
      char* infoLog = new char[infologLength+1];
      int charsWritten  = 0;
      glGetShaderInfoLog( obj, infologLength, &charsWritten, infoLog );
      std::cout << infoLog << std::endl;
      delete[] infoLog;
   }
   else
   {
      std::cout << "Aucune erreur :-)" << std::endl;
   }
}

static void afficherProgramInfoLog( GLuint obj, std::string message )
{
   // afficher le message d'en-tête
   std::cout << message << std::endl;

   // afficher le message d'erreur, le cas échéant
   int infologLength = 0;
   glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

   if ( infologLength > 1 )
   {
      char* infoLog = new char[infologLength+1];
      int charsWritten  = 0;
      //glGetShaderInfoLog( obj, infologLength, &charsWritten, infoLog );
      glGetProgramInfoLog( obj, infologLength, &charsWritten, infoLog );
      std::cout << infoLog << std::endl;
      delete[] infoLog;
   }
   else
   {
      std::cout << "Aucune erreur :-)" << std::endl;
   }
}

GLuint compilerNuanceurs( const char *nuanceurSommetsChaine, const char *nuanceurFragmentsChaine )
{
   // créer le programme
   GLuint progNuanceur = glCreateProgram();

   // associer le contenu du fichier de sommets au nuanceur de sommets
   GLuint nuanceurSommetsObj = glCreateShader( GL_VERTEX_SHADER );
   if ( nuanceurSommetsChaine != NULL )
   {
      glShaderSource( nuanceurSommetsObj, 1, &nuanceurSommetsChaine, NULL );
      glCompileShader( nuanceurSommetsObj );
      glAttachShader( progNuanceur, nuanceurSommetsObj );
      afficherShaderInfoLog( nuanceurSommetsObj, "nuanceurSommets" );
   }

   // associer le contenu du fichier de fragments au nuanceur de fragments
   GLuint nuanceurFragmentsObj = glCreateShader( GL_FRAGMENT_SHADER );
   if ( nuanceurFragmentsChaine != NULL )
   {
      glShaderSource( nuanceurFragmentsObj, 1, &nuanceurFragmentsChaine, NULL );
      glCompileShader( nuanceurFragmentsObj );
      glAttachShader( progNuanceur, nuanceurFragmentsObj );
      afficherShaderInfoLog( nuanceurFragmentsObj, "nuanceurFragments" );
   }

   // linker le programme
   glLinkProgram( progNuanceur );
   afficherProgramInfoLog( progNuanceur, "progNuanceur" );

   return( progNuanceur );
}

GLuint initialiserNuanceurs( const char *nuanceurSommetsFich, const char *nuanceurFragmentsFich )
{
   // lire les fichiers de nuanceurs de sommets et de fragments
   const char *nuanceurSommetsChaine = lireNuanceur( nuanceurSommetsFich );
   const char *nuanceurFragmentsChaine = lireNuanceur( nuanceurFragmentsFich );

   return( compilerNuanceurs( nuanceurSommetsChaine, nuanceurFragmentsChaine ) );
}
