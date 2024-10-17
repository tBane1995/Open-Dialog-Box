#ifndef Mouse_hpp
#define Mouse_hpp

sf::Vector2i mousePosition;
sf::Vector2f worldMousePosition;

sf::Time startClickTime;				// TO-DO selecting

sf::Vector2i startMousePosition;
sf::Vector2f startWorldMousePosition;
bool selection_state;
sf::RectangleShape selectArea;

void mouseSelection() {
    float start_x = std::min(startWorldMousePosition.x, worldMousePosition.x);
    float start_y = std::min(startWorldMousePosition.y, worldMousePosition.y);
    float end_x = std::max(startWorldMousePosition.x, worldMousePosition.x);
    float end_y = std::max(startWorldMousePosition.y, worldMousePosition.y);

    selectArea = sf::RectangleShape(sf::Vector2f(end_x - start_x, end_y - start_y));
    selectArea.setOrigin((end_x - start_x) / 2, (end_y - start_y) / 2);
    selectArea.setPosition((start_x + end_x) / 2, (start_y + end_y) / 2);
    selectArea.setFillColor(sf::Color(128, 48, 48, 128));

}
#endif
