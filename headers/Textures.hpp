#ifndef Textures_hpp
#define Textures_hpp

using namespace std;

class Texture {
public:
	string name;
	sf::Texture* texture;
	float cx, cy;	// coordinates of center on the texture

	Texture(string pathfile, float cx, float cy) {
		
		name = "";

		short i = int(pathfile.size()) - 5;
		while ( i>=0)
			name = pathfile[i--] + name;

		this->cx = cx;
		this->cy = cy;

		texture = new sf::Texture;
		texture->loadFromFile("assets/" + pathfile);

		//cout << "load texture: " << pathfile << " as: " << name << endl;
	}

};

std::vector < Texture* > textures;

void loadTexture(string pathfile, float cx, float cy) {
	textures.push_back( new Texture(pathfile, cx, cy) );
}

void loadTextures() {

	textures.clear();

	loadTexture("GUI/scrollbar/button_bottom.png", 8, 8);
	loadTexture("GUI/scrollbar/button_top.png", 8, 8);
	loadTexture("GUI/scrollbar/bar_bottom.png", 8, 8);
	loadTexture("GUI/scrollbar/bar_top.png", 8, 8);
	loadTexture("GUI/scrollbar/bar_center.png", 8, 8);
	loadTexture("GUI/scrollbar/scroll_bottom.png", 8, 8);
	loadTexture("GUI/scrollbar/scroll_top.png", 8, 8);
	loadTexture("GUI/scrollbar/scroll_center.png", 8, 8);

	loadTexture("GUI/icons/dictionary.png", 16, 16);
	loadTexture("GUI/icons/file.png", 16, 16);

}

Texture* getTexture(string name) {

	for (auto& t : textures) {
		if (t->name == name) {
			return t;
		}
	}

	std::cout << "error - get Texture - Texture: \"" << name << "\" not exists\n";
	return nullptr;
}

#endif