#include "Point.h"

/*
 * constructeur
 */

Point::Point(int x,int y){
	m_x=x;
	m_y=y;
}

void Point::print() {
	cout << "x= " << m_x << " ; y= " << m_y << endl;
}


/*
 * Accesseurs
 */

void Point::setX(int x){
	m_x=x;
}

void Point::setY(int y){
	m_y=y;
}

int Point::getX() const{
	return m_x;
}

int Point::getY() const{
	return m_y;
}

/*
 * différents opérateurs
 */

Point Point::operator+(Point Point2) const{
	int x=m_x;
	int y=m_y;
	x+=Point2.m_x;
	y+=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

Point Point::operator-(Point Point2) const{
	int x=m_x;
	int y=m_y;
	x-=Point2.m_x;
	y-=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

bool Point::operator==(Point Point2) const{
	if(m_x==Point2.m_x && m_y==Point2.m_y)
		return true;
	else
		return false;
}


double Point::rayon(Point Point2) const{
	int dx=(Point2.m_x-m_x);
	int dy=(Point2.m_y-m_y);
	return sqrt(dx*dx + dy*dy);
}

double Point::angle(Point Point2) const{
	int dx=(Point2.m_x-m_x);
	int dy=(Point2.m_y-m_y);
	return atan2(dx,dy);
}


/*
 * opérateur de flux sortant pour les points
 */

ostream &operator<<(ostream &out, Point point){
	point.print();
	return out;
}

/*
 * Toutes les fonctions qui agissent sur une liste de point
 */

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) {
	vector<Point> resultat;
	double t;
	for(int k=0;k<=nbPointsBezier;k++){
		t=(double)k/nbPointsBezier;
		vector<Point> listeBarycentres(pointsDeControle);
		int longueur=pointsDeControle.size();
		for(int i=0;i<longueur-1;i++){
			for(int j=0;j<longueur-1-i;j++){
				listeBarycentres[j]=listeBarycentres[j]*(1-t)+listeBarycentres[j+1]*t;
			}
		}
		resultat.push_back(listeBarycentres[0]);
	}
	return resultat;
}

vector<Consigne> convertirEnConsignes(const vector<Point>& listePoints){
	vector<Consigne> resultat;
	int longueur=listePoints.size();
	double rayon;
	double angle;
	double angleBkp; //nécéssaire pour les modulos

	int sensDeRotation;
	Consigne nouvelleConsigne;
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
		nouvelleConsigne.setRayon((int)rayon); //on ne caste que maintenant pour ne pas cumuler d'erreur sur un cast implicite précédent.
		nouvelleConsigne.setAngle((int)angle); //angle /(Oy)
		
		resultat.push_back(nouvelleConsigne);
		nouvelleConsigne.print();
	}
	return resultat;
}

bool estDansListe(const vector<Point>& listePoints, Point point){
	for(int i=0;i<listePoints.size();i++){
		(listePoints[i]==point)?return true;
	}
	return false
}