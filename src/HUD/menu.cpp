#include <menu.hpp>
#include <Utils/getters.hpp>


// Public


MainMenu::MainMenu(sf::RenderWindow& window) :
    name(getFont()), 
    copyright(getFont()), 
    bgColor(30, 139, 234), 
    centerPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f), 
    bgTexture(getBackgroundTexture()),
    startBtn({200.0f, 60.0f}, 40, centerPosition, getFont(), "START", bgColor, sf::Color::Black, sf::Color::White),
    optionsBtn({200.0f, 60.0f}, 40, {centerPosition.x, centerPosition.y + 80.0f}, getFont(), "OPTIONS", bgColor, sf::Color::Black, sf::Color::White),
    quitBtn({200.0f, 60.0f}, 40, {centerPosition.x, centerPosition.y + 160.0f}, getFont(), "QUIT", bgColor, sf::Color::Black, sf::Color::White), 
    background(bgTexture) {
        bgTexture = getBackgroundTexture();

        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = bgTexture.getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        background.setScale({scaleX, scaleY});

        name.setString("Runora"); //L before for ascii chars
        name.setCharacterSize(80);
        name.setFillColor(bgColor);
        name.setStyle(sf::Text::Bold); // Could use '|' for bitwise OR and add other properties
        name.setOrigin(name.getLocalBounds().getCenter());
        name.setPosition({centerPosition.x, centerPosition.y - 160.0f});

        copyright.setString(L"© 2025 huger6. All rights deserved");
        copyright.setCharacterSize(20);
        copyright.setFillColor(sf::Color::Black);
        copyright.setStyle(sf::Text::Italic);
        copyright.setOrigin(copyright.getLocalBounds().getCenter());
        copyright.setPosition({centerPosition.x, window.getSize().y - 30.0f});
}

Button* MainMenu::getClickedButton(sf::RenderWindow& window) {
    if (startBtn.isClicked(window)) return &startBtn;
    else if (optionsBtn.isClicked(window)) return &optionsBtn;
    else if (quitBtn.isClicked(window)) return &quitBtn;
    return nullptr;
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(name);
    window.draw(copyright);
    startBtn.draw(window);
    optionsBtn.draw(window);
    quitBtn.draw(window);
}

void MainMenu::update(sf::RenderWindow& window) {
    bool anyHover = false;

    startBtn.update(window);
    optionsBtn.update(window);
    quitBtn.update(window);

    if (startBtn.isHovered(window) || optionsBtn.isHovered(window) || quitBtn.isHovered(window)) {
        anyHover = true;
    }
    
    if (anyHover) setMouseAsHand(window);
    else setMouseAsArrow(window);
}


PauseMenu::PauseMenu(sf::RenderWindow& window) : 
    MainMenu(window),
    resumeBtn(startBtn),
    returnToMenuBtn(quitBtn) {
        std::string resume = "RESUME";
        resumeBtn.changeLabel(resume);

        std::string exit = "MAIN MENU";
        returnToMenuBtn.changeLabel(exit);

        bgTexture = getBackgroundTexturePause();
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = bgTexture.getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        background.setScale({scaleX, scaleY}); 
}

void PauseMenu::update(sf::RenderWindow& window) {
    bool anyHover = false;

    resumeBtn.update(window);
    optionsBtn.update(window);
    returnToMenuBtn.update(window);

    if (resumeBtn.isHovered(window) || optionsBtn.isHovered(window) || returnToMenuBtn.isHovered(window)) {
        anyHover = true;
    }
    
    if (anyHover) setMouseAsHand(window);
    else setMouseAsArrow(window);
}

void PauseMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    resumeBtn.draw(window);
    optionsBtn.draw(window);
    returnToMenuBtn.draw(window);
}
