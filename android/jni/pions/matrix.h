#include <iostream>

template<typename T>
class Matrix
{
        public:
            Matrix(int I,int J)
            {
                //values = new T[I*J];
            }
            ~Matrix()
            {

            }
            
            void release()
            {
				if(values){
					//delete [] values;
				}
			}
            void set(T value,int i,int j)
            {
                if(i<NX && j<NY)
                    values[i*NX+j]=value;
                else
                    std::cerr<<"Accès en dehors de la matrice"<<std::endl;
            }
            T get(int i,int j)
            {
                if(i<NX && j<NY)
                    return values[i*NX+j];
                else{
                    std::cerr<<"Accès en dehors de la matrice"<<std::endl;
                    return 0;
                }
            }
        private:
            T values[NX*NY];
};
