#ifndef OpenDialogBox_hpp
#define OpenDialogBox_hpp

bool sortkey(std::filesystem::directory_entry first, std::filesystem::directory_entry second) {

    if (first.is_directory() && second.is_directory()) {
        string name_1 = first.path().filename().string();
        string name_2 = second.path().filename().string();

        std::transform(name_1.begin(), name_1.end(), name_1.begin(), [](unsigned char c) { return std::tolower(c); });
        std::transform(name_2.begin(), name_2.end(), name_2.begin(), [](unsigned char c) { return std::tolower(c); });

        if (name_1 < name_2)
            return true;
        else
            return false;
    }
    else if (first.is_directory() && !second.is_directory()) {
        return true;
    }
    else if (!first.is_directory() && second.is_directory()) {
        return false;
    }
    else {
        // both are file
        string name_1 = first.path().filename().string();
        string name_2 = second.path().filename().string();

        std::transform(name_1.begin(), name_1.end(), name_1.begin(), [](unsigned char c) { return std::tolower(c); });
        std::transform(name_2.begin(), name_2.end(), name_2.begin(), [](unsigned char c) { return std::tolower(c); });

        if (name_1 < name_2)
            return true;
        else
            return false;
    }

}


class OpenDialogBox {
public:
    sf::Vector2f position = sf::Vector2f(300, 300);
    sf::RectangleShape rect;        // main big panel
    sf::RectangleShape titlebar;    // 
    TextArea* titleText;            //  
    sf::RectangleShape submitbar;   // 
    TextArea* filenameText;         // "Filename" text
    TextArea* selectedFilenameText; // selected filename
    TextButton* submitButton;       // 

    std::filesystem::path current_path;                     // current path of directory  
    std::vector < std::filesystem::directory_entry > paths; // list of paths

    sf::RectangleShape textField[7];    // text Field - rects
    TextArea* texts[7];                 // texts - filenames
    sf::Sprite icons[7];                // icons

    Scrollbar* scrollbar;   // scrollbar          
    bool fileSelected;      // if "submit button" pressed is true

    OpenDialogBox(string title) {
        rect = sf::RectangleShape(sf::Vector2f(512, 256 + 32));
        rect.setFillColor(sf::Color(64, 64, 64));
        rect.setPosition(position.x - 256, position.y - 128 - 32);

        titlebar = sf::RectangleShape(sf::Vector2f(512, 32));
        titlebar.setFillColor(sf::Color(48, 48, 48));
        titlebar.setPosition(position.x - 256, position.y - 128 - 32);

        titleText = new TextArea(title);
        titleText->setCharacterSize(24);
        titleText->generateRect();
        titleText->setRectColor(sf::Color::Transparent);
        titleText->setTextColor(textColor);
        titleText->setPosition(sf::Vector2f(position.x - 256, position.y - 128 - 32));

        clearTexts();
        current_path = std::filesystem::current_path();
        loadDirectory();
        loadScrollbar();

        sf::Vector2f pos;

        filenameText = new TextArea("File name: ");
        filenameText->setCharacterSize(24);
        filenameText->generateRect();
        filenameText->setRectColor(sf::Color::Transparent);
        filenameText->setTextColor(textColor);
        pos.x = position.x - 256;
        pos.y = position.y + 256 - 128 - filenameText->getSize().y + 4;
        filenameText->setPosition(pos);
        
        selectedFilenameText = new TextArea("");
        selectedFilenameText->setCharacterSize(24);
        selectedFilenameText->setRectColor(sf::Color(32, 32, 32));
        selectedFilenameText->setTextColor(textColor);
        selectedFilenameText->setRectSize(sf::Vector2f(256 + 48 - 4, filenameText->rect.getSize().y));
        pos.x = position.x - 256 + filenameText->getSize().x;
        pos.y = position.y + 256 - 128 - selectedFilenameText->rect.getSize().y + 4;
        selectedFilenameText->setPosition(pos);

        submitButton = new TextButton("submit");
        submitButton->rect.setSize(sf::Vector2f(submitButton->rect.getSize().x, selectedFilenameText->getSize().y));
        pos.x = position.x + 256 - submitButton->rect.getSize().x / 2 - 4;
        pos.y = position.y + 256 - 128 - 16 -1 + 4;
        submitButton->setPosition(pos);

        submitbar = sf::RectangleShape(sf::Vector2f(512, selectedFilenameText->rect.getSize().y + 8));
        submitbar.setFillColor(sf::Color(48, 48, 48));
        submitbar.setPosition(position.x - 256, position.y + 256 - 128 - 32 + 1);
        
        fileSelected = false;
    }

    ~OpenDialogBox() {

        delete titleText;
        delete filenameText;
        delete selectedFilenameText;
        delete submitButton;
        
        for (auto& t : texts)
            delete t;

        delete scrollbar;
        
    }

    void clearTexts() {
        for (int i = 0; i < 7; i++) {

            textField[i] = sf::RectangleShape(sf::Vector2f(512, 32));
            textField[i].setFillColor(sf::Color::Transparent);
            textField[i].setPosition(position.x - 256, position.y - 128 + i * 32);

            texts[i] = new TextArea("");
            texts[i]->setCharacterSize(20);
            texts[i]->setRectColor(sf::Color::Transparent);
            texts[i]->setTextColor(textColor);
            texts[i]->setPosition(sf::Vector2f(position.x - 256 + 32 + 4, position.y - 128 + i * 32 + 4));

            icons[i] = sf::Sprite();
        }
    }

    void loadScrollbar() {
        sf::Vector2f scrollbarSize = sf::Vector2f(16, 256 - 32 + 1);
        sf::Vector2f scrollbarPosition = sf::Vector2f(position.x + 256 - scrollbarSize.x, position.y - 128);
        scrollbar = new Scrollbar(scrollbarSize, scrollbarPosition, 0, paths.size() - 1, 0, 7);
    }

    void loadDirectory() {
        paths.clear();
        if (filesystem::exists(current_path) && filesystem::is_directory(current_path)) {

            for (auto& entry : filesystem::directory_iterator(current_path)) {
                paths.push_back(entry);
            }
        }
        std::sort(paths.begin(), paths.end(), sortkey);

        paths.emplace(paths.begin(), current_path.parent_path());

    }

    std::string getFilename() {

        return selectedFilenameText->text.getString();
    }

    void update(sf::Event& event, float dt) {
        scrollbar->update(event);

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {

                submitButton->click();

                for (short i = 0; i < 7;i++) {
                    if (texts[i]->rect.getGlobalBounds().contains(worldMousePosition)) {
                        
                        // LOAD THE DIRECTORY
                        if (i + short(scrollbar->scrollValue) < paths.size()) {
                            
                            if(!paths[i + short(scrollbar->scrollValue)].is_directory())
                                selectedFilenameText->setString(texts[i]->s);

                            else {
                                current_path = std::filesystem::path(paths[i + short(scrollbar->scrollValue)].path().string());
                                selectedFilenameText->setString("");
                                clearTexts();
                                loadDirectory();
                                loadScrollbar();

                            }
                                
                        }
                        
                     }
                }

                if (submitButton->state == buttonState::pressed) {
                    fileSelected = true;
                }
            }
        }

        for (short i = 0; i < 7; i++) {

            if (i + short(scrollbar->scrollValue) < paths.size()) {
                if (i + short(scrollbar->scrollValue) == 0) {
                    texts[i]->setString("..");
                }
                else {
                    texts[i]->setString(paths[i + short(scrollbar->scrollValue)].path().filename().string());
                }
                texts[i]->generateRect();
                icons[i] = sf::Sprite();
                string extension = paths[i + short(scrollbar->scrollValue)].path().extension().string();
                if (extension == "")
                    icons[i].setTexture(*getTexture("GUI/icons/dictionary")->texture);
                else
                    icons[i].setTexture(*getTexture("GUI/icons/file")->texture);
                icons[i].setPosition(position.x - 256, position.y - 128 + i * 32);
            }
        }

        submitButton->update(dt);
    }

    void draw() {
        window->draw(rect);
        window->draw(titlebar);
        titleText->draw();
        window->draw(submitbar);
        filenameText->draw();
        selectedFilenameText->draw();
        submitButton->draw();
        scrollbar->draw();

        for (short i = 0; i < 7; i++) {
            window->draw(textField[i]);
            texts[i]->draw();
            window->draw(icons[i]);
        }

    }
};

#endif