#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
	anchoPantalla = 0;
	altoPantalla = 0;
	renderer = nullptr;
}

MapGenerator::MapGenerator(SDL_Renderer* _renderer, int _anchoPantalla, int _altoPantalla, TilesGraph* _tilesGraph):
	anchoPantalla(_anchoPantalla),
	altoPantalla(_altoPantalla),
	renderer(_renderer),
	tilesGraph(_tilesGraph)
{
	
}

bool MapGenerator::crearObjetosJuego(string _path)
{
	// Crea un flujo de archivo
	fstream file;

	// Abrir el archivo
	file.open(_path.c_str(), ios::in);

	if (file.is_open() == false) {
		std::cout << "No se pudo abrir el archivo de mapa" << std::endl;
		return false;
	}

	string line;
	
	Texture* texturaBomberman;
	texturaBomberman = new Texture();
	Texture* texturaBomberwoman;
	texturaBomberwoman = new Texture();
	Texture* texturaMuroMetal;
	texturaMuroMetal = new Texture();
	Texture* texturaMuroCeramica;
	texturaMuroCeramica = new Texture();
	Texture* texturaSueloCesped;
	texturaSueloCesped = new Texture();
	
	Texture::renderer = renderer;

	texturaBomberman->loadFromImage("resources/bman.png");
	texturaBomberwoman->loadFromImage("resources/bman.png");
	texturaMuroCeramica->loadFromImage("resources/muro_ceramica.jpg");
	texturaMuroMetal->loadFromImage("resources/muro_metal.jpg");
	texturaSueloCesped->loadFromImage("resources/suelo_cesped.jpg");
	
	int x = 0;
	int y = 0;
	int bombermanPosicionX = -1;
	int bombermanPosicionY = -1;
	int bomberwomanPosicionX = -1;
	int bomberwomanPosicionY = -1;

	while (getline(file, line)) {
		vector<char> chars(line.begin(), line.end());
		x = 0;

		for (int i = 0; i < chars.size(); i++) {
			if (chars[i] != ' ') {
				GameObject* objetoNuevo = nullptr;
				Tile* tileNuevo = tilesGraph->getTileEn(x, y);
				switch (chars[i]) {
				case '0':
					objetoNuevo = new SueloCesped(texturaSueloCesped, tileNuevo);
					if (x > bombermanPosicionX && bombermanPosicionX == -1) {
						bombermanPosicionX = x;
						bombermanPosicionY = y;
					}

					if (x > bomberwomanPosicionX || y > bomberwomanPosicionY) {
						bomberwomanPosicionX = x;
						bomberwomanPosicionY = y;
					}

				// pilaObjetosJuegoMurosMetal.Insertar((GameActor*)objetoNuevo);
                //  creación de los tiles MuroCeramica  y MuroMetal a partir de su textura, arreglado 
					break;
				case '1':
					objetoNuevo = new MuroCeramica(texturaMuroCeramica, tileNuevo);

					// Implementación de contenedor avanzado map que almacena los objetos Muro Ceramica con fila y columna par
					if (x % 2 == 0 && y % 2 == 0)
					{
						int key = 0;
						key = y * TilesGraph::anchoTilesGraph + x;
						//Key se inserta a mObjetoMCPar
						mObjetoMCPar.insert({ key,1 });	
					}
					
					break;
				case '2':
					objetoNuevo = new MuroMetal(texturaMuroMetal, tileNuevo);
					break;
					/*case 'B':
						objetoNuevo = new Bomberman(texturaBomberman, tileNuevo);
						break;*/
				}
				
				if (objetoNuevo != nullptr) {
					((GameActor*)objetoNuevo)->setPosicionX(x * 34);
					((GameActor*)objetoNuevo)->setPosicionY(y * 34);
					vectorObjectosJuego.push_back(objetoNuevo);
					x++;
				}
			}
		}
		
		y++;
	}
	
	//Impresión de mObjetoMCPar
	cout << "KEY\tCASO CERAMICA\n";
	for (auto itr = mObjetoMCPar.begin(); itr != mObjetoMCPar.end(); ++itr) {
		cout << itr->first
			<< '\t' << itr->second << endl;
	}
	cout << " "<< endl;
	
	GameObject* objetoBomberman = nullptr;
	Tile* tileNuevo = tilesGraph->getTileEn(bombermanPosicionX, bombermanPosicionY);
	objetoBomberman = new Bomberman(texturaBomberman, tileNuevo);
	if (objetoBomberman != nullptr) {
		((GameActor*)objetoBomberman)->setPosicionX(bombermanPosicionX * 34);
		((GameActor*)objetoBomberman)->setPosicionY(bombermanPosicionY * 34);
		vectorObjectosJuego.push_back(objetoBomberman);
	}

	GameObject* objetoBomberwoman = nullptr;
	tileNuevo = tilesGraph->getTileEn(bomberwomanPosicionX, bomberwomanPosicionY);
	objetoBomberwoman = new Bomberman(texturaBomberwoman, tileNuevo);
	if (objetoBomberwoman != nullptr) {
		((GameActor*)objetoBomberwoman)->setPosicionX(bomberwomanPosicionX * 34);
		((GameActor*)objetoBomberwoman)->setPosicionY(bomberwomanPosicionY * 34);
		((GamePawn*)objetoBomberwoman)->setBotomBomba(SDLK_o);
		((GamePawn*)objetoBomberwoman)->setBotomArriba(SDLK_w);
		((GamePawn*)objetoBomberwoman)->setBotomAbajo(SDLK_s);
		((GamePawn*)objetoBomberwoman)->setBotomIzquierda(SDLK_a);
		((GamePawn*)objetoBomberwoman)->setBotomDerecha(SDLK_d);

		vectorObjectosJuego.push_back(objetoBomberwoman);
	}


	return false;
}

void MapGenerator::transferirObjetosJuego(vector<GameObject*>& _vectorObjetosJuegoDestino)
{
	for (int i = 0; i < vectorObjectosJuego.size(); i++) {
		_vectorObjetosJuegoDestino.push_back(vectorObjectosJuego[i]);
	}
}
