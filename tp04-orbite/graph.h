#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

class Celeste {
	
protected :
	GLUquadric *sphere = gluNewQuadric();
	
public :
	float Rayon;
	float Distance;
	float Rotation;
	float Revolution;
	float IncrRotation;
	float IncrRevolution;

    float r, g, b;
    float a = 1;
	
	Celeste(float rayon, 
			float distance, 
			float rotation, 
			float revolution, 
			float incrRotation, 
			float incrRevolution);

    void definirCouleur(float r, float g, float b, float a = 1);

    virtual void afficher();
};

class Satellite : public Celeste {
public :
    using Celeste::Celeste;
	void afficher();
};

class Planete : public Celeste {
public :
	std::vector<Satellite> Satellites;
    GLdouble drawMatrix[16];
	
    using Celeste::Celeste;
	void afficher();
};

class Etoile : public Celeste {
public :
	std::vector<Planete> Systeme;
	
    using Celeste::Celeste;
    void afficher(bool utiliserNuanceur, GLuint prog, float facteur);
};


#endif
