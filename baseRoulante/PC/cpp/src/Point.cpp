#include "Point.h"

/*!coefficients de conversion */

#define CONVERSION_ANGLE_TICKS 1356.00012
#define CONVERSION_DISTANCE_TICKS 9.59732592

/*
 * constructeur
 */

Point::Point(double x,double y){
	m_x=x;
	m_y=y;
	CHECK_INVARIANTS;
}

void Point::print() {
	CHECK_INVARIANTS;
	cout << "x= " << m_x << " ; y= " << m_y << endl;
}


/*
 * Accesseurs
 */

void Point::setX(double x){
	CHECK_INVARIANTS;
	REQUIRE(0<=x && x<=3000, "Réactualisation de l'abscisse d'un point avec une abscisse dans les limites du terrain");
	m_x=x;
	CHECK_INVARIANTS;
	
}

void Point::setY(double y){
	CHECK_INVARIANTS;
	REQUIRE(0<=y && y<=2100, "Réactualisation de l'ordonnée d'un point avec une dans les limites du terrain");
	m_y=y;
}

double Point::getX(){
	CHECK_INVARIANTS;
	return m_x;
}

double Point::getY(){
	CHECK_INVARIANTS;
	return m_y;
}

/*
 * différents opérateurs
 */

Point Point::operator+(Point Point2){
	CHECK_INVARIANTS;
	double x=m_x;
	double y=m_y;
	x+=Point2.m_x;
	y+=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

Point Point::operator-(Point Point2){
	CHECK_INVARIANTS;
	REQUIRE(Point2.m_x>=0, "Abscisse du point à soustraire positive");
	REQUIRE(Point2.m_y>=0, "Ordonnée du point à soustraire positive");
	double x=m_x;
	double y=m_y;
	x-=Point2.m_x;
	y-=Point2.m_y;
	Point resultat(x,y);
	ENSURE(resultat.m_x>=0, "Abscisse de la différence des deux points positive");
	ENSURE(resultat.m_y>=0, "Ordonnée de la différence des deux points positive");
	return resultat;
}

bool Point::operator==(Point Point2){
	CHECK_INVARIANTS;
	return(m_x==Point2.m_x && m_y==Point2.m_y);
}

bool Point::operator!=(Point Point2){
	CHECK_INVARIANTS;
	return(m_x!=Point2.m_x || m_y!=Point2.m_y);
}

void Point::round(){
	CHECK_INVARIANTS;
	m_x=floor(m_x+0.5);
	m_y=floor(m_y+0.5);
}

double Point::rayon(Point Point2){
	CHECK_INVARIANTS;
	REQUIRE(Point2.m_x>=0, "Abscisse du point dont on veut trouver la distance est positive");
	REQUIRE(Point2.m_y>=0, "Ordonnée du point dont on veut trouver la distance est positive");
	double dx=(Point2.m_x-m_x);
	double dy=(Point2.m_y-m_y);
	return sqrt(dx*dx + dy*dy);
}

double Point::angle(Point Point2){
	CHECK_INVARIANTS;
	REQUIRE(Point2.m_x>=0, "Abscisse du point avec lequel on détermine l'angle est positive");
	REQUIRE(Point2.m_y>=0, "Ordonnée du point avec lequel on détermine l'angle est positive");
	double dx=(Point2.m_x-m_x);
	double dy=(Point2.m_y-m_y);
	return (atan2(dx,dy)+M_PI_2);
}


/*
 * opérateur de flux sortant pour les points et les vecteurs de points
 */

ostream &operator<<(ostream &out, Point point){
	point.print();
	return out;
}

ostream &operator<<(ostream &out, vector<Point> listePoints){
	for(unsigned int i=0;i<listePoints.size();i++)
		listePoints[i].print();
	return out;
}

/*
 * Toutes les fonctions qui agissent sur une liste de point
 */

vector<Point> ListePoints::lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) {
	vector<Point> resultat;
	vector<Point> listeBarycentres;
	double t;
	/*si la liste de points reçue est nulle, une exception est générée*/
// 	try{
		for(int k=0;k<=nbPointsBezier;k++){
			t=(double)k/nbPointsBezier;
			listeBarycentres.clear();
			listeBarycentres = pointsDeControle;
			int longueur=pointsDeControle.size();
			for(int i=0;i<longueur-1;i++){
				for(int j=0;j<longueur-1-i;j++){
					listeBarycentres[j]=listeBarycentres[j]*(1-t)+listeBarycentres[j+1]*t;
				}
			}
			resultat.push_back(listeBarycentres[0]);
		}
// 	}
	return resultat;
}

vector<Consigne> ListePoints::convertirEnConsignes(vector<Point>& listePoints){
	vector<Consigne> resultat;
	int longueur=listePoints.size();
	double rayon=0;
	double angle=0;
	double angleBkp=0; //nécéssaire pour les modulos

	int sensDeRotation;
	Consigne nouvelleConsigne;
	
		/*si la liste de points reçue est nulle, une exception est générée*/
// 	try{
		for(int i=0;i<longueur-1;i++){
			
			
			/*
			* mise à jour des paramètres de la consigne
			*/
			angle=listePoints[i].angle(listePoints[i+1]);
			sensDeRotation = (angle>angleBkp)?1:-1; // 1 : sens horraire, -1 : sens antihorraire.
			if((angle-angleBkp)>M_PI)
				angle+=sensDeRotation*2*M_PI;
			angleBkp=angle;
			angle*=CONVERSION_ANGLE_TICKS;
			rayon+=listePoints[i].rayon(listePoints[i+1])*CONVERSION_DISTANCE_TICKS; //conversion en ticks...
			nouvelleConsigne.setRayon(floor(rayon+0.5)); //on ne caste que maintenant pour ne pas cumuler d'erreur sur un cast implicite précédent.
			nouvelleConsigne.setAngle(floor(angle+0.5)); //angle /(Oy)
			resultat.push_back(nouvelleConsigne);
		}
	//}
	return resultat;
}

