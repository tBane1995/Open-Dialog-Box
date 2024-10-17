#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <conio.h>

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

#include <algorithm>
#include <numeric>
#include <functional>

#include <string>
#include <windows.h>

#include "headers/Window.hpp" 
#include "headers/Time.hpp"
#include "headers/Camera.hpp"
#include "headers/Mouse.hpp"

#include "headers/Fonts.hpp"
#include "headers/Textures.hpp"

#include "headers/GUI.hpp"
#include "headers/TextArea.hpp"
#include "headers/Buttons.hpp"
#include "headers/Scrollbar.hpp"
#include "headers/OpenDialogBox.hpp"


void testOpenDialogBox() {
    
    cam = new Camera();
    
    OpenDialogBox* openDial = new OpenDialogBox("Load Map");
    
    while (window->isOpen())
    {
        prevTime = currentTime;
        currentTime = timeClock.getElapsedTime();
        dt = currentTime.asSeconds() - prevTime.asSeconds();

        mousePosition = sf::Mouse::getPosition(*window);	// Pobierz aktualną pozycję myszy względem bieżącego okna
        worldMousePosition = window->mapPixelToCoords(mousePosition);

        GUIwasHover = false;
        GUIwasClicked = false;

        sf::Event event;
        while (window->pollEvent(event)) {

            if (openDial) {
                openDial->update(event, dt);

                
                if (openDial->fileSelected) {
                    cout << openDial->getFilename() << "\n";
                    delete openDial;
                    openDial = nullptr;
                }
                
            }
        }

        // RENDER
        window->clear();
        if(openDial)
            openDial->draw();
        window->display();
    }

}

int main()
{

    // LOADS
	loadFonts();
	loadTextures();
    
    // PROGRAM
    testOpenDialogBox();
    return 0;
}