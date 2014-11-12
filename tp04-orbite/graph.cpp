#include "graph.h"

Celeste::Celeste(float rayon, 
			float distance, 
			float rotation, 
			float revolution, 
			float incrRotation, 
			float incrRevolution) : 
			Rayon(rayon),
			Distance(distance),
			Rotation(rotation),
			Revolution(revolution),
			IncrRotation(incrRotation),
			IncrRevolution(incrRevolution){}

void Celeste::afficher() {
    glColor4f(this->r, this->g, this->b, this->a);
    this->Revolution += this->IncrRevolution;
    this->Rotation += this->IncrRotation;
    gluSphere( this->sphere, this->Rayon, 16, 16 );

}

void Celeste::definirCouleur(float r, float g, float b, float a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Etoile::afficher(bool utiliserNuanceur, GLuint prog, float facteur){
    for (Planete &cel : this->Systeme) {
        glPushMatrix();
            glColor3f(cel.r, cel.g, cel.b);
            glUseProgram(0);
            glutSolidTorus( 0.1, cel.Distance, 4, 64 );
            if ( utiliserNuanceur )
            {
               glUseProgram( prog );
               glUniform1f( glGetUniformLocation( prog, "facteurRechauffement" ), facteur );
            }
            cel.afficher();
        glPopMatrix();
	}
    glEnable( GL_BLEND );
    glDepthMask( GL_FALSE );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glUseProgram(0);
    Celeste::afficher();
    glDepthMask( GL_TRUE );
    glDisable( GL_BLEND );
}
		
void Planete::afficher(){
    glRotatef(this->Revolution, 0, 0, 1);
    glTranslatef( this->Distance, 0, 0 );
    glPushMatrix();
        glRotatef(this->Rotation, 0, 0, 1);
        //glGetDoublev (GL_MODELVIEW_MATRIX, this->drawMatrix);
        Celeste::afficher();
    glPopMatrix();
    for (Satellite &cel : this->Satellites) {
        glPushMatrix();
            cel.afficher();
        glPopMatrix();
    }
}
		
void Satellite::afficher(){
    glRotatef(this->Revolution, 0, 0, 1);
    glTranslatef( this->Distance, 0, 0 );
    glPushMatrix();
        glRotatef(this->Rotation, 0, 0, 1);
        Celeste::afficher();
    glPopMatrix();
	Celeste::afficher();
}
