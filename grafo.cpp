#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct grafo_simple
{
	map< char , vector<char> > aristas;	
	
	vector<char> vecinos(char id)
	{
		return aristas[id];
	}

};

grafo_simple ejemplo_1 {{
    {'A', {'B'}},
    {'B', {'A', 'C', 'D'}},
    {'C', {'A'}},
    {'D', {'E', 'A'}},
    {'E', {'B'}}
  }};
/*ESTE FUE UN PRIMER EJEMPLO DE PRUEBA DE UN GRAFO SIMPLE*/  
/*
void breadth_fist_search(grafo_simple grafo, char raiz)
{
	queue <char> frontera;
	set <char> visitados;
	
	frontera.push(raiz);
	visitados.insert(raiz);
	
	while (!frontera.empty())
	{
		char continuo = frontera.front();
		frontera.pop();
		
		cout<<"VISITADOS -->>>"<< continuo <<"\n";
		
		for(char next:grafo.vecinos(continuo))
		{
			if (visitados.find(next)==visitados.end())
			{
				frontera.push(next);
				visitados.insert(next);
			}	
		}
	}
}*/  



struct grilla_ubicacion //LO CREO PARA PODER TENER LA UBICACION X E Y DE MI GRILLA 
{
	int x,y ;
};


struct Parrilla //CREO EL AMBIENTE DEL GRAFO
{
	static array <grilla_ubicacion,4> direccion;//TOMA SOLO LAS DE ARRIBA ABAJO DERECHA IZQUIERDA
	set <grilla_ubicacion>paredes;//LOS OBSTACULOS

	int altura , ancho ;//LAS DIMESIONES DE LA PARRILLA

	Parrilla(int ancho_ ,int  altura_ )
		: altura(altura_ ) ,ancho(ancho_ ) {}

	bool limites_correctos(grilla_ubicacion id) const
		{
			return 0 <= id.x && id.x <=ancho && 0 <= id.y && id.y <=altura; 
		}
	bool se_encuentra(grilla_ubicacion id) const
		{
			return paredes.find(id) == paredes.end();
		}
	vector<grilla_ubicacion> vecinos  (grilla_ubicacion id ) const //FUNCION UE RECORRE TODOS LOS VECINOS DE MI GRAFO  
		{
			vector<grilla_ubicacion> resultados;
	
			for(grilla_ubicacion  dir:direccion)
				{
					grilla_ubicacion next{dir.x +id.x ,dir.y+id.y }; 				
					if (limites_correctos(next) && se_encuentra(next))
						{
							resultados.push_back(next);			
						}
				}
			if (!((id.x+id.y)%2))
				{
					reverse(resultados.begin(),resultados.end());
				}
			return resultados;			
		}
};
  
array <grilla_ubicacion,4> Parrilla::direccion =//DEFINO LAS DIRECCIONES BASICAS
{grilla_ubicacion{1,0},grilla_ubicacion{0,-1},grilla_ubicacion{-1,0},grilla_ubicacion{0,1}};


//ALGUNAS OPERACIONES BASICAS QUE NECESITARE PARA LA COMPARICION DE MIS UBICACIONES

bool operator ==(grilla_ubicacion a, grilla_ubicacion b )
	{
		return a.x==b.x && a.y==b.y; 
	}    
bool operator !=(grilla_ubicacion a, grilla_ubicacion b)
	{
		return !(a==b);
	}	
bool operator <(grilla_ubicacion a, grilla_ubicacion b)
	{
		return tie(a.x,a.y) < tie(b.x,b.y);
	}
basic_iostream<char>::basic_ostream& operator<<(basic_iostream<char>::basic_ostream& out, const grilla_ubicacion & loc) 
	{
	  out << '(' << loc.x << ',' << loc.y << ')';
	  return out;
	}

template <class Grafo>
void dibujar_grilla(const Grafo &graf, int ancho_campo,
               map<grilla_ubicacion, double>* distancias=nullptr,
               map<grilla_ubicacion, grilla_ubicacion>* apunta_a=nullptr,
               vector<grilla_ubicacion>* camino=nullptr)//FUNCION QUE ME PERMITE DIBUJAR EL GRAFO , PUEDO DIBUJAR LAS DISTANCIAS, SUS HIJOS Y EL CAMINO QUE RECOORO
{
	for ( int y = 0; y < graf.altura; ++y)//RECORRO TOA MI PARRILLA 
	{
		for (int x = 0; x < graf.ancho; ++x)
		{
			grilla_ubicacion id{x,y};
			cout<<left<<setw(ancho_campo);
			if(graf.paredes.find(id) != graf.paredes.end())
			{
				cout << string(ancho_campo, '#');
		    } else if (apunta_a != nullptr && apunta_a->count(id)) {//PARTE DE LOS HIJOS DE MI INICIO
				grilla_ubicacion siguiente = (*apunta_a)[id];
				if (siguiente.x == x + 1) { cout << "> "; }
				else if (siguiente.x == x - 1) { cout << "< "; }
				else if (siguiente.y == y + 1) { cout << "v "; }
				else if (siguiente.y == y - 1) { cout << "^ "; }
				else { cout << "* "; }
			} else if (distancias != nullptr && distancias->count(id)) {//LOS COSTOS 
					cout << (*distancias)[id];
			} else if (camino != nullptr && find(camino->begin(), camino->end(), id) != camino->end()) {//CAMINO CORRECTO
					cout << '@';
			} else {
					cout << '.';
			}
		}
		cout << '\n';
	}
}

template<typename Locacion, typename Grafo> 
map<Locacion, Locacion> breadth_first_search(Grafo grafo, Locacion inicio, Locacion meta) {//FUNCION PARA breadth_first_search
  queue<Locacion> fronteras;
  fronteras.push(inicio);
 
  map<Locacion, Locacion> viene_de;
  viene_de[inicio] = inicio;
  
  while (!fronteras.empty()) {
    Locacion continuo = fronteras.front();
    fronteras.pop();
	cout<<continuo<<"\n";	
	
	if (meta==continuo)
	{
		break;
	}
    for (Locacion next : grafo.vecinos(continuo)) {
      if (viene_de.find(next) == viene_de.end()) {
        fronteras.push(next);
        viene_de[next] = continuo;
      }
    }
  }
  cout<<"TERMINADO"<<"\n";
  return viene_de;
}

void agregar_recta(Parrilla& grilla, int x1, int y1, int x2, int y2) {//AGREGO OBSTACULOS
  for (int x = x1; x < x2; ++x) {
    for (int y = y1; y < y2; ++y) {
      grilla.paredes.insert(grilla_ubicacion{x, y});
    }
  }
}

Parrilla diagrama1() {//FUNCION QUE ME RETRONA UN PRIMER ENTORNO
  Parrilla grilla(30, 15);
  agregar_recta(grilla, 3, 3, 5, 12);
  agregar_recta(grilla, 13, 4, 15, 15);
  agregar_recta(grilla, 21, 0, 23, 7);
  agregar_recta(grilla, 23, 5, 26, 7);
  return grilla;
}		

struct Parrilla_con_pesos: Parrilla {//MODIFICO MI ESTRUCTURA PARRILLA POR UN A QUE AHORA TENDRA PESOS
  set<grilla_ubicacion> foraneos;
  Parrilla_con_pesos(int an, int al): Parrilla(an, al) {}
  double costo(grilla_ubicacion de_nodo, grilla_ubicacion hacia_nodo) const {
    return foraneos.find(hacia_nodo) != foraneos.end()? 5 : 1;
  }
};

template<typename T, typename prioridad_t>
struct PrioridadCola {//CREO UN COLA DE PRIORIDAD PARA RE ASEGURAR QUE LA COLA DE C++ ESTE BIEN
  typedef pair<prioridad_t, T> PQElemento;
  priority_queue<PQElemento, vector<PQElemento>,greater<PQElemento>> elementos;

  inline bool vacio() const {
     return elementos.empty();
  }

  inline void poner(T objeto, prioridad_t prioridad) {
    elementos.emplace(prioridad, objeto);
  }

  T conseguir() {
    T mejor_objeto = elementos.top().second;
    elementos.pop();
    return mejor_objeto;
  }
};

template<typename Locacion, typename Grafo>
void dijkstra_search(Grafo grafo,Locacion inicio, Locacion meta , map<Locacion, Locacion>& viene_de, map<Locacion, double>& costo_hasta_aqui)//ESTE ES LA FUNCION DEL ALGORITMO DE DIJSTRA QUE RECIBE LOS 3 VARIABLES QUE NECESITO PARA IMPRIMIR
{
  PrioridadCola<Locacion, double> fronteras;
  fronteras.poner(inicio, 0);

  viene_de[inicio] = inicio;
  costo_hasta_aqui[inicio] = 0;
  
  while (!fronteras.vacio()) {
    Locacion continuo = fronteras.conseguir();

    if (continuo == meta) {
      break;
    }

    for (Locacion next : grafo.vecinos(continuo)) {
      double nuevo_costo = costo_hasta_aqui[continuo] + grafo.costo(continuo, next);
      if (costo_hasta_aqui.find(next) == costo_hasta_aqui.end() || nuevo_costo < costo_hasta_aqui[next]) {
        costo_hasta_aqui[next] = nuevo_costo;
        viene_de[next] = continuo;
        fronteras.poner(next, nuevo_costo);
      }
    }
  }
}

template<typename Locacion>
vector<Locacion> reconstruccion_camino( Locacion inicio, Locacion meta,map<Locacion, Locacion> viene_de)//ME PERMITE SABER CUAL ES CAMNIO DESDE MI META HASTA MI  INICIO 
{
  vector<Locacion> camino;
  Locacion continuo = meta;
  while (continuo != inicio) {
    camino.push_back(continuo);
    continuo = viene_de[continuo];
  }
  camino.push_back(inicio); // optional
  reverse(camino.begin(), camino.end());
  return camino;
}

Parrilla_con_pesos diagrama4() {
  Parrilla_con_pesos grilla(10, 10);
  agregar_recta(grilla, 1, 7, 4, 9);
  typedef grilla_ubicacion L;
  grilla.foraneos = set<grilla_ubicacion> {
    L{3, 4}, L{3, 5}, L{4, 1}, L{4, 2},
    L{4, 3}, L{4, 4}, L{4, 5}, L{4, 6},
    L{4, 7}, L{4, 8}, L{5, 1}, L{5, 2},
    L{5, 3}, L{5, 4}, L{5, 5}, L{5, 6},
    L{5, 7}, L{5, 8}, L{6, 2}, L{6, 3},
    L{6, 4}, L{6, 5}, L{6, 6}, L{6, 7},
    L{7, 3}, L{7, 4}, L{7, 5}
  };
  return grilla;
}

inline double heuristico(grilla_ubicacion a, grilla_ubicacion b) {//LA FUNCION HEURISTICA DE CADA MOVIMEINTO 
  return abs(a.x - b.x) + abs(a.y - b.y);
}

template<typename Locacion, typename Grafo>
void a_star_search//MI FUNCION A* PARECIDA A UN DIJSTRA SOLO QUE AUMENTADO EL HEURISTIO QUE ME DA MAS RAPIDES
  (Grafo grafo,
   Locacion inicio,
   Locacion meta,
   map<Locacion, Locacion>& viene_de,
   map<Locacion, double>& costo_hasta_aqui)
{
  PrioridadCola<Locacion, double> fronteras;
  fronteras.poner(inicio, 0);

  viene_de[inicio] = inicio;
  costo_hasta_aqui[inicio] = 0;
  
  while (!fronteras.vacio()) {
    Locacion continuo = fronteras.conseguir();

    if (continuo == meta) {
      break;
    }

    for (Locacion next : grafo.vecinos(continuo)) {
      double nuevo_costo = costo_hasta_aqui[continuo] + grafo.costo(continuo, next);
      if (costo_hasta_aqui.find(next) == costo_hasta_aqui.end() || nuevo_costo < costo_hasta_aqui[next]) {
        costo_hasta_aqui[next] = nuevo_costo;
        double prioridad = nuevo_costo + heuristico(next, meta);
        fronteras.poner(next, prioridad);
        viene_de[next] = continuo;
      }
    }
  }
}
int main ()
{
	//breadth_fist_search( ejemplo_1 , 'A' );
    
    /*Parrilla grilla = diagrama1();
    grilla_ubicacion inicio{8,7};
    grilla_ubicacion meta{17,2};
    auto padres=breadth_first_search(grilla , inicio, meta);
  	dibujar_grilla(grilla,2, nullptr,&padres);*/
	
	Parrilla_con_pesos grilla= diagrama4(); 
	grilla_ubicacion inicio{0,2};//CREO MI NODO INICIO
	grilla_ubicacion meta{9,8};//CREO MIS DOS NODO META
	//CREO MIS DATOS PARA IMPRIMIR
	map<grilla_ubicacion, grilla_ubicacion> viene_de;
 	map<grilla_ubicacion, double> costo_hasta_aqui;
  	a_star_search(grilla, inicio, meta, viene_de, costo_hasta_aqui);//DOY LA OPERACION DE MI A*
    dibujar_grilla(grilla, 2, nullptr, &viene_de);//IMPRIMO MI PARRILLA
    cout << '\n';
    dibujar_grilla(grilla, 3, &costo_hasta_aqui, nullptr);//IMPRIMO LOS COSTOS DE MI PARRILLA
    cout << '\n';
    vector<grilla_ubicacion> camino = reconstruccion_camino(inicio, meta, viene_de);//IPRIMO MO CAMINO
    dibujar_grilla(grilla, 3, nullptr, nullptr, &camino);

	return 0;
}

