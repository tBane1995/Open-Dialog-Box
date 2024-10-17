#ifndef TextArea_hpp
#define TextArea_hpp

class TextArea {
public:
	string s;
	sf::Vector2f position;
	sf::Text text;
	sf::RectangleShape rect;
	
	TextArea(string s) {
		this->s = s;

		text = sf::Text();
		text.setFont(basicFont);
		text.setCharacterSize(16);
		text.setString(s);

		sf::Vector2f textPos;
		textPos.x = position.x + float(text.getCharacterSize())*0.2f;
		textPos.y = position.y;
		text.setPosition(textPos);

		generateRect();
	}

	~TextArea() {

	}

	void generateRect() {
		sf::Vector2f size;
		size.x = float(text.getLocalBounds().getSize().x) + float(text.getCharacterSize())*0.5f;
		size.y = float(text.getCharacterSize()) * 1.3f;
		rect.setSize(size);
	}

	void setRectSize(sf::Vector2f size) {
		rect.setSize(size);
	}

	void setString(string s) {
		this->s = s;
		text.setString(s);
	}

	void setPosition(sf::Vector2f position) {
		this->position = position;

		rect.setPosition(position);

		sf::Vector2f textPos;
		textPos.x = position.x + float(text.getCharacterSize()) * 0.15f;
		textPos.y = position.y;
		text.setPosition(textPos);
	}

	void setCharacterSize(short val) {
		
		text.setCharacterSize(val);
	}

	void setRectColor(sf::Color color) {
		rect.setFillColor(color);
	}

	void setTextColor(sf::Color color) {
		text.setFillColor(color);
	}

	sf::Vector2f getSize() {
		return rect.getSize();
	}

	void update(float dt) {
		
	}

	void draw() {
		window->draw(rect);
		window->draw(text);
	}
};

#endif