#include "menu.hpp"
// Getters
#include "Utils/getters.hpp"


// ==================== Base Menu ====================

Menu::Menu(sf::RenderWindow& window, const std::string& backgroundPath) :
    centerPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f),
    themeColor(30, 139, 234),
    background(bgTexture) {
        setupBackground(window, backgroundPath);
    }

void Menu::setupBackground(sf::RenderWindow& window, const std::string& backgroundPath) {
    if (!bgTexture.loadFromFile(backgroundPath)) {
        std::cerr << "Failed to load background: " << backgroundPath << "\n";
        return;
    }
    
    background.setTexture(bgTexture, true);
    
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = bgTexture.getSize();
    
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    background.setScale({scaleX, scaleY});
}

void Menu::addButton(const std::string& label, float yOffset) {
    sf::Vector2f position(centerPosition.x, centerPosition.y + yOffset);
    
    buttons.emplace_back(
        sf::Vector2f{BUTTON_WIDTH, BUTTON_HEIGHT},
        BUTTON_TEXT_SIZE,
        position,
        getFont(),
        label,
        themeColor,
        sf::Color::Black,
        sf::Color::White
    );
}

int Menu::getClickedButtonIndex(sf::RenderWindow& window) {
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i].isClicked(window)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

Button* Menu::getClickedButton(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        if (button.isClicked(window)) {
            return &button;
        }
    }
    return nullptr;
}

void Menu::update(sf::RenderWindow& window) {
    bool anyHover = false;
    
    for (auto& button : buttons) {
        button.update(window);
        if (button.isHovered(window)) {
            anyHover = true;
        }
    }
    
    if (anyHover) {
        setMouseAsHand(window);
    } else {
        setMouseAsArrow(window);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(background);
    
    for (auto& button : buttons) {
        button.draw(window);
    }
}


// ==================== MainMenu ====================

MainMenu::MainMenu(sf::RenderWindow& window) :
    Menu(window, getBackgroundTexturePath()),
    title(getFont()),
    copyright(getFont()) {
        // Adicionar botões
        addButton("START", 0.0f);
        addButton("OPTIONS", BUTTON_SPACING);
        addButton("QUIT", BUTTON_SPACING * 2);
        
        setupTitle(window);
        setupCopyright(window);
    }

void MainMenu::setupTitle(sf::RenderWindow& window) {
    (void)window;
    
    title.setString("Runora");
    title.setCharacterSize(80);
    title.setFillColor(themeColor);
    title.setStyle(sf::Text::Bold);
    title.setOrigin(title.getLocalBounds().getCenter());
    title.setPosition({centerPosition.x, centerPosition.y - 160.0f});
}

void MainMenu::setupCopyright(sf::RenderWindow& window) {
    copyright.setString(L"© 2025 huger6. All rights deserved");
    copyright.setCharacterSize(20);
    copyright.setFillColor(sf::Color::Black);
    copyright.setStyle(sf::Text::Italic);
    copyright.setOrigin(copyright.getLocalBounds().getCenter());
    copyright.setPosition({centerPosition.x, window.getSize().y - 30.0f});
}

void MainMenu::draw(sf::RenderWindow& window) {
    Menu::draw(window);
    window.draw(title);
    window.draw(copyright);
}


// ==================== PauseMenu ====================

PauseMenu::PauseMenu(sf::RenderWindow& window) :
    Menu(window, getBackgroundTexturePausePath()) {
        addButton("RESUME", 0.0f);
        addButton("OPTIONS", BUTTON_SPACING);
        addButton("MAIN MENU", BUTTON_SPACING * 2);
    }

