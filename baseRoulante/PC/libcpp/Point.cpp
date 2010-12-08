#include "Point.h"

/*
 * constructeur
 */

Point::Point(int x,int y){
	m_x=x;
	m_y=y;
}

void Point::print() const{
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

Point Point::operator+(const Point& Point2){
	int x=m_x;
	int y=m_y;
	x+=Point2.m_x;
	y+=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

Point Point::operator-(const Point& Point2){
	int x=m_x;
	int y=m_y;
	x-=Point2.m_x;
	y-=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

bool Point::operator==(const Point& Point2){
	if(m_x==Point2.m_x && m_y==Point2.m_y)
		return true;
	else
		return false;
}

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
	int dx;
	int dy;
	int sensDeRotation;
	Consigne nouvelleConsigne;
	for(int i=0;i<longueur-1;i++){
		
		/*
		 * calcul des coordonnées du vecteur
		 */
		dx = listePoints[i+1].getX() - listePoints[i].getX() ;
		dy = listePoints[i+1].getY() - listePoints[i].getY() ;
		
		/*
		 * mise à jour des paramètres de la consigne
		 */
		angle=(atan2( dx , dy ));
		sensDeRotation = (angle>angleBkp)?1:-1; // 1 : sens horraire, -1 : sens antihorraire.
		if((angle-angleBkp)>M_PI)
			angle+=sensDeRotation*2*M_PI;
		angleBkp=angle;
		angle*=CONVERSION_ANGLE_TICKS;
		rayon+=sqrt(dx*dx + dy*dy)*CONVERSION_DISTANCE_TICKS; //conversion en ticks...
		nouvelleConsigne.setRayon((int)rayon); //on ne caste que maintenant pour ne pas cumuler d'erreur sur un cast implicite précédent.
		nouvelleConsigne.setAngle((int)angle); //angle /(Oy)
		
		resultat.push_back(nouvelleConsigne);
		nouvelleConsigne.print();
	}
	return resultat;
}