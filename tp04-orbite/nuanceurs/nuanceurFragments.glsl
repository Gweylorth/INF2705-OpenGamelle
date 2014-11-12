// les étudiants peuvent utiliser l'exemple du cours pour démarrer:
//    http://www.cours.polymtl.ca/inf2705/nuanceurs/exempleSimple/

// couleur du pôle sans réchauffement
vec4 couleurPole = vec4( 1.0, 1.0, 1.0, 1.0 );
uniform float facteurRechauffement;
varying vec3 vecN;

void main( void )
{
   gl_FragColor = gl_Color*(1.0 - vec4( (abs(vecN.z) * (1.0 - facteurRechauffement)))) +
           (vec4(1.0) * (vec4( (abs(vecN.z) * 1.0 - facteurRechauffement) )));
}
