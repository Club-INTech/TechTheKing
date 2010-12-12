#include "Point.h"

/*
 * constructeur
 */

Point::Point(double x,double y){
	m_x=x;
	m_y=y;
}

void Point::print() {
	cout << "x= " << m_x << " ; y= " << m_y << endl;
}


/*
 * Accesseurs
 */

void Point::setX(double x){
	m_x=x;
}

void Point::setY(double y){
	m_y=y;
}

double Point::getX() const{
	return m_x;
}

double Point::getY() const{
	return m_y;
}

/*
 * différents opérateurs
 */

Point Point::operator+(Point Point2) const{
	double x=m_x;
	double y=m_y;
	x+=Point2.m_x;
	y+=Point2.m_y;
	Point resultat(x,y);
	return resultat;
}

Point Point::operator-(Point Point2) const{
	double x=m_x;
	double y=m_y;
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

void Point::round(){
	m_x=floor(m_x+0.5);
	m_y=floor(m_y+0.5);
}

double Point::rayon(Point Point2) const{
	double dx=(Point2.m_x-m_x);
	double dy=(Point2.m_y-m_y);
	return sqrt(dx*dx + dy*dy);
}

double Point::angle(Point Point2) const{
	double dx=(Point2.m_x-m_x);
	double dy=(Point2.m_y-m_y);
	return atan2(dx,dy);
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

vector<Point> lissageBezier(const vector<Point>& pointsDeControle,int nbPointsBezier) {
	vector<Point> resultat;
	vector<Point> listeBarycentres;
	double t;
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
		cout << listeBarycentres[0] << endl;
		resultat.push_back(listeBarycentres[0]);
	}
	return resultat;
}

vector<Consigne> convertirEnConsignes(const vector<Point>& listePoints){
	vector<Consigne> resultat;
	int longueur=listePoints.size();
	double rayon=0;
	double angle=0;
	double angleBkp=0; //nécéssaire pour les modulos

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
		nouvelleConsigne.setRayon(floor(rayon+0.5)); //on ne caste que maintenant pour ne pas cumuler d'erreur sur un cast implicite précédent.
		nouvelleConsigne.setAngle(floor(angle+0.5)); //angle /(Oy)
		
		resultat.push_back(nouvelleConsigne);
	}
	return resultat;
}

