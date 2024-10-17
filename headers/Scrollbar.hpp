#ifndef Scrollbar_hpp
#define Scrollbar_hpp

class Scrollbar {
public:
    sf::Vector2f size;
    sf::Vector2f position;
    float minValue;
    float maxValue;
    float scrollValue;
    float scrollLength;

    sf::RectangleShape bar;
    sf::Sprite barTop;
    sf::Sprite barBottom;
    sf::Sprite barCenter;

    sf::RectangleShape scroll;
    sf::Sprite scrollTop;
    sf::Sprite scrollBottom;
    sf::Sprite scrollCenter;

    //////////////////////////////////////
    bool pressed;
    sf::Vector2f mouseStartPos;
    sf::Vector2f mouseOffset;
    //////////////////////////////////////

    Scrollbar(sf::Vector2f size, sf::Vector2f position, float minValue, float maxValue, float scrollValue, float scrollLength) {
        this->size = size;
        this->position = position;

        this->minValue = minValue;
        this->maxValue = maxValue;
        this->scrollValue = scrollValue;

        this->scrollLength = scrollLength;

        pressed = false;
        
        // create bar
        bar = sf::RectangleShape(size);
        bar.setFillColor(sf::Color::Blue);
        bar.setPosition(position);

        barTop = sf::Sprite();
        barTop.setTexture(*getTexture("GUI/scrollbar/bar_top")->texture);
        barTop.setPosition(position.x, position.y);
        barTop.setScale(bar.getSize().x / 16.0f, bar.getSize().x / 16.0f);

        barCenter = sf::Sprite();
        barCenter.setTexture(*getTexture("GUI/scrollbar/bar_center")->texture);
        barCenter.setPosition(position.x, position.y + scroll.getSize().x);
        barCenter.setScale(bar.getSize().x / 16.0f, (bar.getSize().y - 2.0f * bar.getSize().x) / 16.0f);

        barBottom = sf::Sprite();
        barBottom.setTexture(*getTexture("GUI/scrollbar/bar_bottom")->texture);
        barBottom.setPosition(position.x, position.y + bar.getSize().y - bar.getSize().x);
        barBottom.setScale(bar.getSize().x / 16.0f, bar.getSize().x / 16.0f);
        
        // create scroll
        sf::Vector2f scrollPosition;
        scrollPosition.x = position.x;
        scrollPosition.y = position.y + (scrollValue - minValue) / (maxValue - scrollLength + 1) * (size.y - getScrollHeight());

        scroll = sf::RectangleShape(sf::Vector2f(size.x, getScrollHeight()));
        scroll.setOrigin(0, 0);
        scroll.setFillColor(sf::Color::Red);
        scroll.setPosition(scrollPosition);

        scrollTop = sf::Sprite();
        scrollTop.setTexture(*getTexture("GUI/scrollbar/scroll_top")->texture);
        scrollTop.setPosition(scrollPosition.x, scrollPosition.y);
        scrollTop.setScale(scroll.getSize().x / 16.0f, scroll.getSize().x / 16.0f);

        scrollCenter = sf::Sprite();
        scrollCenter.setTexture(*getTexture("GUI/scrollbar/scroll_center")->texture);
        scrollCenter.setPosition(scrollPosition.x, scrollPosition.y + scroll.getSize().x);
        scrollCenter.setScale(scroll.getSize().x / 16.0f, (getScrollHeight() - 2.0f * scroll.getSize().x) / 16.0f);

        scrollBottom = sf::Sprite();
        scrollBottom.setTexture(*getTexture("GUI/scrollbar/scroll_bottom")->texture);
        scrollBottom.setPosition(scrollPosition.x, scrollPosition.y + getScrollHeight() - scroll.getSize().x);
        scrollBottom.setScale(scroll.getSize().x / 16.0f, scroll.getSize().x / 16.0f);
        
    }

    ~Scrollbar() {

    }

    void setValue(int value) {

        scrollValue = value;

        if (scrollValue > maxValue-scrollLength+1)
            scrollValue = maxValue-scrollLength+1;

        if (scrollValue < minValue)
            scrollValue = minValue;

    }

    float getScrollHeight() {
        if (maxValue - minValue + 1 >= scrollLength) {
            float height = scrollLength / maxValue * size.y;
            if (height < size.x*3)
                height = size.x*3;

            return height;
        }
        else
            return size.y;
    }

    void update(sf::Event& event) {
        
        // bar positioning
        bar.setPosition(position);
        barTop.setPosition(position.x, position.y);
        barCenter.setPosition(position.x, position.y + scroll.getSize().x);
        barBottom.setPosition(position.x, position.y + bar.getSize().y - bar.getSize().x);

        sf::Vector2f scrollPosition;
        scrollPosition.x = position.x;
        scrollPosition.y = position.y + (scrollValue - minValue) / (maxValue - scrollLength + 1) * (size.y - getScrollHeight());

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                if (scroll.getGlobalBounds().contains(worldMousePosition)) {
                    pressed = true;
                    mouseStartPos = mousePos;
                    mouseOffset = scrollPosition - mousePos;
                }
            }
        }

        if( event.type == sf::Event::MouseButtonReleased ) {
            if( event.mouseButton.button == sf::Mouse::Left ) {
                pressed = false;
            }
        }

        if (event.type == sf::Event::MouseMoved) {
            if (pressed) {
                sf::Vector2f mousePos = window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

                if (fabs(mousePos.x - mouseStartPos.x) > 100) // Te zachwoanie scrolla w Windowsie jak odjedziesz za daleko w bok
                    mousePos = mouseStartPos;

                float value = (mousePos.y + mouseOffset.y - position.y) / (size.y - getScrollHeight()) * (maxValue - minValue) + minValue;

                setValue(value);
            }
        }

        // scroll positioning
        scroll.setPosition(scrollPosition);
        scrollTop.setPosition(scrollPosition.x, scrollPosition.y);
        scrollCenter.setPosition(scrollPosition.x, scrollPosition.y + scroll.getSize().x);
        scrollBottom.setPosition(scrollPosition.x, scrollPosition.y + getScrollHeight() - scroll.getSize().x);
    }

    void draw() {
        window->draw(bar);
        window->draw(barTop);
        window->draw(barBottom);
        window->draw(barCenter);

        window->draw(scroll);
        window->draw(scrollTop);
        window->draw(scrollCenter);
        window->draw(scrollBottom);
        
    }
};


#endif