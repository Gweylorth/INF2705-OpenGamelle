#ifndef NUANCEURS_H
#define NUANCEURS_H

#include <string.h>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/glut.h>

int VerifierErreurGL( const std::string endroit );

GLuint compilerNuanceurs( const char *nuanceurSommetsChaine, const char *nuanceurFragmentsChaine );
GLuint initialiserNuanceurs( const char *nuanceurSommetsFich, const char *nuanceurFragmentsFich );

#endif
