#ifndef Buttons_hpp
#define Buttons_hpp

sf::Color idleColor = sf::Color(32.0f, 32.0f, 32.0f);
sf::Color hoverColor = sf::Color(48.0f, 48.0f, 48.0f);
sf::Color pressedColor = sf::Color(64.0f, 64.0f, 64.0f);

sf::Color spriteIdleColor = sf::Color(192.0f, 192.0f, 192.0f);
sf::Color spriteHoverColor = sf::Color(224.0f, 224.0f, 224.0f);
sf::Color spritePressedColor = sf::Color::White;

enum class buttonState { idle, hover, pressed };

class Btn {
public:

    sf::Vector2f position;
    sf::RectangleShape rect;
    sf::Sprite sprite;
    buttonState state;
    std::function<void()> func;
    sf::Time clickTime;

    Btn(float width = 64, float height = 64, sf::Vector2f position = sf::Vector2f(0, 0)) {

        this->position = position;

        rect = sf::RectangleShape(sf::Vector2f(width, height));
        rect.setOrigin(width / 2, height / 2);
        rect.setPosition(position.x + cam->position.x, position.y + cam->position.y);

        sprite = sf::Sprite();

        state = buttonState::idle;
        func = { };

        clickTime = currentTime;

        changeColor();
    }

    Btn(Btn* btn) {

        this->position = btn->position;

        this->rect = btn->rect;
        this->sprite = btn->sprite;

        state = buttonState::idle;
        this->func = btn->func;

        clickTime = currentTime;

        changeColor();
    }

    Btn(Btn* btn, sf::Vector2f position) {

        this->position = position;

        this->rect = btn->rect;
        this->rect.setPosition(position.x + cam->position.x, position.y + cam->position.y);
        this->sprite = btn->sprite;
        this->sprite.setPosition(position.x + cam->position.x, position.y + cam->position.y);

        state = buttonState::idle;
        this->func = btn->func;

        clickTime = currentTime;

        changeColor();
    }

    Btn(Texture* texture, sf::Vector2f position) {

        this->position = position;

        sf::Vector2f size = sf::Vector2f(texture->texture->getSize().x, texture->texture->getSize().y);
        rect = sf::RectangleShape(size);
        rect.setOrigin(size.x / 2, size.y / 2);
        rect.setPosition(position.x + cam->position.x, position.y + cam->position.y);

        sprite = sf::Sprite();
        sprite.setTexture(*texture->texture);
        sprite.setOrigin(texture->texture->getSize().x / 2.0f, texture->texture->getSize().y / 2.0f);
        sf::Vector2f scale = sf::Vector2f(size.x / texture->texture->getSize().x, size.y / texture->texture->getSize().y);
        sprite.setScale(scale);
        sprite.setPosition(position.x + cam->position.x, position.y + cam->position.y);

        state = buttonState::idle;

        func = { };

        clickTime = currentTime;

        changeColor();
    }

    virtual void setTexture(Texture* texture) {
        sf::Vector2f size = sf::Vector2f(texture->texture->getSize().x, texture->texture->getSize().y);
        rect = sf::RectangleShape(size);
        rect.setOrigin(size.x / 2, size.y / 2);
        rect.setPosition(position.x + cam->position.x, position.y + cam->position.y);

        sprite = sf::Sprite();
        sprite.setTexture(*texture->texture);
        sprite.setOrigin(texture->texture->getSize().x / 2, texture->texture->getSize().y / 2);
        sf::Vector2f scale = sf::Vector2f(size.x / texture->texture->getSize().x, size.y / texture->texture->getSize().y);
        sprite.setScale(scale);
        sprite.setPosition(position.x + cam->position.x, position.y + cam->position.y);

        changeColor();
    }

    virtual void setPosition(sf::Vector2f position) {
        this->position = position;
        rect.setPosition(position.x + cam->position.x, position.y + cam->position.y);
        sprite.setPosition(position.x + cam->position.x, position.y + cam->position.y);
    }

    virtual void changeColor() {

        if (state == buttonState::pressed) {
            rect.setFillColor(pressedColor);
            sprite.setColor(spritePressedColor);
        }
        else if (state == buttonState::hover) {
            rect.setFillColor(hoverColor);
            sprite.setColor(spriteHoverColor);
        }
        else {
            rect.setFillColor(idleColor);
            sprite.setColor(spriteIdleColor);
        }

    }

    virtual void unclick() {
        if ((currentTime - clickTime).asSeconds() > 0.1f) {
            state = buttonState::idle;
            changeColor();
        }

    }

    virtual bool hover() {
        if (state != buttonState::pressed) {

            float w = rect.getSize().x;
            float h = rect.getSize().y;
            float x = rect.getPosition().x;
            float y = rect.getPosition().y;

            if (worldMousePosition.x > x - w / 2.0f && worldMousePosition.x < x + w / 2.0f &&
                worldMousePosition.y > y - h / 2.0f && worldMousePosition.y < y + h / 2.0f) {

                state = buttonState::hover;
                changeColor();
                GUIwasHover = true;
                return true;
            }
        }
        else {
            // GUI WAS PRESSED
            GUIwasHover = true;
            return true;
        }

        return false;
    }

    virtual bool click() {
        float w = rect.getSize().x;
        float h = rect.getSize().y;
        float x = rect.getPosition().x;
        float y = rect.getPosition().y;

        if (worldMousePosition.x > x - w / 2.0f && worldMousePosition.x < x + w / 2.0f &&
            worldMousePosition.y > y - h / 2.0f && worldMousePosition.y < y + h / 2.0f) {

            state = buttonState::pressed;
            changeColor();
            GUIwasClicked = true;
            clickTime = currentTime;

            if (func)
                func();

            return true;
        }

        return false;
    }

    virtual void update(float dt) {
        rect.setPosition(cam->position.x + position.x, cam->position.y + position.y);
        sprite.setPosition(cam->position.x + position.x, cam->position.y + position.y);
    }

    virtual void draw() {
        window->draw(rect);
        if (sprite.getTexture() != nullptr)
            window->draw(sprite);
    }
};


class TextButton : public Btn {
public:
    sf::Text text;

    //std::vector < Option > options;

    TextButton(string text) : Btn() {
        this->text = sf::Text();
        this->text.setFont(basicFont);
        this->text.setCharacterSize(16);
        this->text.setFillColor(textColor);
        this->text.setString(text);

        sf::Vector2f textsize;
        textsize.x = this->text.getLocalBounds().getSize().x;
        textsize.y = this->text.getLocalBounds().getSize().y;

        this->text.setOrigin(textsize.x / 2, textsize.y / 2);

        sf::Vector2f size;
        size.x = textsize.x + 24;
        size.y = textsize.y + 16;

        rect.setSize(size);
        rect.setOrigin(size.x / 2, size.y / 2);

    }

    virtual void setPosition(sf::Vector2f position) {
        Btn::setPosition(position);
        text.setPosition(position.x + cam->position.x, position.y + cam->position.y);
    }

    virtual void update(float dt) {
        Btn::update(dt);
        text.setPosition(position.x + cam->position.x, position.y + cam->position.y);
    }

    virtual void draw() {
        Btn::draw();
        window->draw(text);
    }
};

// TO-DO
class TextButtonWithLeftAlignment {
public:
    sf::Vector2f position;      // left, top
    sf::RectangleShape rect;
    sf::Text text;
    float margin = 14;
    buttonState state;
    std::function<void()> func;
    sf::Time clickTime;

    TextButtonWithLeftAlignment(string text) {

        this->text = sf::Text();
        this->text.setFont(basicFont);
        this->text.setCharacterSize(16);
        this->text.setFillColor(textColor);
        this->text.setPosition(margin, margin);
        this->text.setString(text);
        
        rect = sf::RectangleShape();
        sf::Vector2f size;
        size.x = this->text.getLocalBounds().getSize().x + 2*margin;
        size.y = this->text.getLocalBounds().getSize().y + 2*margin;
        rect.setSize(size);
        rect.setPosition(cam->position.x, cam->position.y);

        this->text.setPosition(cam->position.x + margin, cam->position.y+margin);

        state = buttonState::idle;
        changeColor();

        std::function<void()> func;

        clickTime = currentTime;
    }

    virtual void setPosition(sf::Vector2f position) {
        this->position = position;
        rect.setPosition(position.x + cam->position.x, position.y + cam->position.y);
        text.setPosition(position.x + cam->position.x+margin, position.y + cam->position.y + margin);
    }

    virtual void changeColor() {

        if (state == buttonState::pressed) {
            rect.setFillColor(pressedColor);
        }
        else if (state == buttonState::hover) {
            rect.setFillColor(hoverColor);
        }
        else {
            rect.setFillColor(idleColor);
        }

    }

    virtual void unclick() {
        if ((currentTime - clickTime).asSeconds() > 0.1f) {
            state = buttonState::idle;
            changeColor();
        }

    }

    virtual bool hover() {
        if (state != buttonState::pressed) {

            float x1 = rect.getPosition().x;
            float y1 = rect.getPosition().y;
            float x2 = x1 + rect.getSize().x;
            float y2 = y1 + rect.getSize().y;

            if (worldMousePosition.x > x1 && worldMousePosition.x < x2 &&
                worldMousePosition.y > y1 && worldMousePosition.y < y2) {

                state = buttonState::hover;
                changeColor();
                GUIwasHover = true;
                return true;
            }
        }
        else {
            // GUI WAS PRESSED
            GUIwasHover = true;
            return true;
        }

        return false;
    }

    virtual bool click() {
        float x1 = rect.getPosition().x;
        float y1 = rect.getPosition().y;
        float x2 = x1 + rect.getSize().x;
        float y2 = y1 + rect.getSize().y;

        if (worldMousePosition.x > x1 && worldMousePosition.x < x2 &&
            worldMousePosition.y > y1 && worldMousePosition.y < y2) {

            state = buttonState::pressed;
            changeColor();
            GUIwasClicked = true;
            clickTime = currentTime;

            if (func)
                func();

            return true;

        }

        return false;

    }

    virtual void update(float dt) {
        rect.setPosition(position.x + cam->position.x, position.y + cam->position.y);
        text.setPosition(position.x + cam->position.x + margin, position.y + cam->position.y+margin);
        
    }

    virtual void draw() {
        window->draw(rect);
        window->draw(text);
    }
};

#endif