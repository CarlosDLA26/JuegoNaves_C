// posibilidad de usar printf y usar formatos y caracteres del teclado diferentes del codigo ASCII (standard input output)
#include<stdio.h> 
#include<windows.h> // libreria para conexion con windows
#include<conio.h>
#include<stdlib.h> // Libreria para llamar la funcion rand()
#include<list>// poder añadir listas en c++

// numeros asociados a las flechas del teclado
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

using namespace std; 

HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); // identificador de la pantalla que queremos manipular (es la pantalla de la consola)
bool game_over = false;
//hCon: obtenemos el identificador de la consola  
void gotoxy(int x, int y){ // recibe el parametro a donde queremos ir
	COORD dwPos; // Estructura para darle coordenadas al cursor en la ventana
	dwPos.X = x; // posicion en X
	dwPos.Y = y; // posicion en Y
	SetConsoleCursorPosition(hCon, dwPos); // funcion de window.h que nos permite dar una posicion al cursor o imprimir donde queramos
}

void OcultarCursor(){
	CONSOLE_CURSOR_INFO cci; // Controlar caracteristicas del cursor
	cci.dwSize = 25; //Estilo del cursor de pantalla SOLO DE 0 A 100
	cci.bVisible = FALSE; // Quitar el cursor 
	SetConsoleCursorInfo(hCon, &cci);
}

void pintarLimites(){
	// Coordenadas en x
	for(int i = 2; i<78; i++){
		gotoxy(i,2); printf("%c",205);
		gotoxy(i,33); printf("%c",205);
	}
	// Coordenadas en y
	for(int i = 3; i<33; i++){
		gotoxy(2,i); printf("%c",186);
		gotoxy(77,i); printf("%c",186);
	}
	// Esquinas del escenario
	gotoxy(2,2);   printf("%c",201);
	gotoxy(2,33);  printf("%c",200);
	gotoxy(77,2);  printf("%c",187);
	gotoxy(77,33); printf("%c",188);
	gotoxy(1,1); printf("Dispara con la tacla A");
	
}


class NAVE{
	private:
		int x,y; // atributos de la clase, posicion de la nave
		int corazones;
		int vidas;
	public:
		NAVE(int _x, int _y, int _corazones, int _vidas): x(_x), y(_y),corazones(_corazones),vidas(_vidas){} // Contructor de la clase: x = _x y y = _y
		void pintar(); // Los gotoxy son la posicion de la nave
		void borrar();
		void mover();
		void pintarCorazones();
		void morir();
		void COR(){
			corazones--;
		}
		int X(){
			return x;
		}
		int Y(){
			return y;
		}
};

void NAVE::pintar(){
	gotoxy(x,y);   printf("   I"); // %c significa caracter
	gotoxy(x,y+1); printf("--(T)--");
	gotoxy(x,y+2); printf(" WW WW");
}

void NAVE::borrar(){
	gotoxy(x,y);   printf("       ");
	gotoxy(x,y+1); printf("       ");
	gotoxy(x,y+2); printf("       ");
}

void NAVE::mover(){
	if(kbhit()){ // Saber si se ha presionado una tecla
			char tecla = getch(); // funcion que atrapa el valor de una tecla 
			borrar(); //imprimir espacio en blanco para borrar el rastro
			if(tecla == IZQUIERDA && x>3) x--; 
			if(tecla == DERECHA && x+7<77) x++; // Se pone + 6 porque la nave es mas grande 
			if(tecla == ARRIBA && y>3) y--; // Arriba porque las coordenadas y estan invertidas
			if(tecla == ABAJO && y+3<33) y++; // Se pone mas tres
			if(tecla == 'e') corazones--;
			pintar();
			pintarCorazones();
		}
}

void NAVE::pintarCorazones(){
	gotoxy(50,1); printf("VIDAS: %d",vidas);
	gotoxy(63,1); printf("SALUD:");
	gotoxy(70,1); printf("       ");
	for(int i=0; i<corazones; i++){
		gotoxy(70+i,1); printf("%c",3);
	}
}

void NAVE::morir(){
	// Animacion de muerte de la nave
	if(corazones == 0){
		borrar();
		gotoxy(x,y);  printf("  ***  ");
		gotoxy(x,y+1);printf(" ***** ");
		gotoxy(x,y+2);printf("  ***  ");
		Sleep(200);
		
		borrar();
		gotoxy(x,y);  printf(" * * * ");
		gotoxy(x,y+1);printf("** * **");
		gotoxy(x,y+2);printf(" * * * ");
		Sleep(200);
		
		borrar();
		gotoxy(x,y);  printf("*  *  *");
		gotoxy(x,y+1);printf("*  *  *");
		gotoxy(x,y+2);printf("*  *  *");
		Sleep(200);
		borrar();
		
		vidas--;
		if(vidas >= 0){
			corazones = 3;
			pintarCorazones();
			pintar();
		}
		else {
			game_over = true;
		}
	}
}


class ASTEROIDE{
	private:
		int x, y;
	public:
		ASTEROIDE(int _x, int _y):x(_x), y(_y){}
		void pintar();
		void mover();
		void choque(class NAVE &N);
		int X(){ return x; }
		int Y(){ return y; }
};

void ASTEROIDE::pintar(){
	gotoxy(x,y); printf("%c",184);
}

void ASTEROIDE::mover(){
	gotoxy(x,y); printf(" ");
	y++;
	if(y > 32){
		x = rand()%74 + 3; //numero aleatoria entre 3 y 77
		y = 4;
	}
	pintar(); // se ejecuta el metodo de asteroide porque esta dentro de la clase ASTEROIDE
		
}

void ASTEROIDE::choque(class NAVE &N){
	if(x >= N.X() && x < N.X()+7 && y >= N.Y() && y < N.Y()+3){
		N.COR();
		N.borrar();
		N.pintar();
		N.pintarCorazones();
		x = rand()%74 + 3; //numero aleatoria entre 3 y 77
		y = 4;
	}
}


class BALA{
	private:
		int x, y;
	public:
		BALA(int _x, int _y):x(_x), y(_y){}
		void mover();
		bool fuera();
		int X(){ return x; }
		int Y(){ return y; }
};

void BALA::mover(){
	gotoxy(x,y); printf(" ");
	if(y > 3 ) y--;
	gotoxy(x,y); printf("*");
}

bool BALA::fuera(){
	if (y == 3) return true;
	return false;
}

int main(){
	
	int puntos = 0;
	
	pintarLimites();
	OcultarCursor();
	
	list<ASTEROIDE*> AST;
	list<ASTEROIDE*>::iterator itA;
	
	// Asterorides en posiciones aleatorias
	for(int i=0; i<5; i++){
		AST.push_back(new ASTEROIDE(rand()%74 + 3, 4));
	}
	
	NAVE N(35,30,3,3); // Ponemos la nave en la posicion 7,7
	N.pintar(); // debemos dibujarlo primero para que salga en pantalla sin mover las flechas
	N.pintarCorazones(); 
	
	list<BALA*> B; // El nombre de la lista es B y le pasamos punteros de los objetos de la clase bala
	list<BALA*>::iterator it; // iterador para el for each de la lista B (BALA)
	
	while(!game_over){
		gotoxy(65, 34); printf("Puntos: %d",puntos);
		if(kbhit()){
			char tecla = getch();
			if(tecla == 'a' || tecla == 'A'){
				B.push_back(new BALA(N.X()+3, N.Y()-1));
			}
		}
		
		for(it = B.begin(); it != B.end(); it++){ // it = al primer elemento de la lista hasta que sea diferente del primer elemento de la lista
			(*it)->mover(); 
			// (*it) desreferenciar el elemento de la list
			// -> acceder a los meodos de la clase por referencia
			if((*it)->fuera()){
				gotoxy((*it)->X(), (*it)->Y()); printf(" ");  
				delete(*it); // se borra el elemento de la lista
				it = B.erase(it); // se pasa el it al siguiente elemento de la lista porque el anterior se invalida: memoria dinamica
			}
		}
		
		for(itA = AST.begin(); itA != AST.end(); itA++){ 
			(*itA) -> mover();
			(*itA) -> choque(N);
		}
		
		for(itA = AST.begin(); itA != AST.end(); itA++){
			for(it = B.begin(); it != B.end(); it++){
				// Detectar las coordenadas de la bala y el asteroide: si choca bala con asteroide
				if((*itA) -> X() == (*it) -> X() && ((*itA) -> Y()+1 == (*it) -> Y() || (*itA) -> Y() == (*it) -> Y())){
					gotoxy((*it)->X(), (*it)->Y()); printf(" ");
					delete(*it);
					it = B.erase(it);
					
					AST.push_back(new ASTEROIDE(rand()%74 + 3, 4));
					gotoxy((*itA)->X(), (*itA)->Y()); printf(" ");
					delete(*itA);
					itA = AST.erase(itA);
					
					puntos += 5;
				}
			}
		}
		
		N.mover();
		Sleep(30); // Evitar que el bucle se ejecute muchas veces para evtar la saturacion del procesador	
		N.morir();		
	}
	gotoxy(1,36); printf("Fin del juego. Puntuacion obtenida: %d puntos",puntos);
	Sleep(2000);
	return 0;
}
