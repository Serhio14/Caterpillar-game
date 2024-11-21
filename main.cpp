#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For srand()
using namespace std;
using namespace sf;

int main() {
    // Initialize random numbers
    srand(static_cast<unsigned>(time(nullptr)));

    // Create a window and set an icon
    RenderWindow window(VideoMode(1920, 1042), "Caterpillar game");
    Image icon;
    if (!icon.loadFromFile("Image/icon.jpg")) {
        return 1;
    }
    window.setIcon(1000, 1000, icon.getPixelsPtr());

    // Set background texture
    Texture Background;
    Background.loadFromFile("Image/Background.jpg");
    Sprite Object3(Background);

    // Create the apple
    Texture Apple;
    Apple.loadFromFile("Image/Apple.png");
    Sprite Object1(Apple);
    Object1.setPosition(866, 445);  // Initial position at the center

    // Speed of the apple
    float moveSpeed = 0.3f;

    // Create the caterpillar
    Texture Сaterpillar;
    Сaterpillar.loadFromFile("Image/Caterpillar.png");
    Sprite Object2(Сaterpillar);
    Object2.setPosition(200.0f, 200.0f); // Initial position

    // Speed of the caterpillar
    Vector2f velocity(0.35f, 0.35f); // Speed in pixels per frame

    // Load collision sound
    SoundBuffer collisionSoundBuffer;
    if (!collisionSoundBuffer.loadFromFile("Sounds/ukus_yabluka.wav")) {
        return 1;
    }
    Sound collisionSound(collisionSoundBuffer);

    // Background music
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Sounds/Doom Eternal OST - The Only Thing They Fear Is You.mp3")) {
        return 1; // Check if the music file exists
    }
    backgroundMusic.setLoop(true); // The music will repeat
    backgroundMusic.play();

    // Apple counter
    int appleCounter = 0;
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        return 1; // Check if the font file exists
    }

    // Display the apple counter
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setString("Apple: 0");

    // Timer
    Clock gameClock;
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 40);

    // Final message text
    Text finalText;
    finalText.setFont(font);
    finalText.setCharacterSize(100); // Make the text large
    finalText.setFillColor(Color::White);
    finalText.setStyle(Text::Bold);

    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Update the timer
        int remainingTime = 45 - static_cast<int>(gameClock.getElapsedTime().asSeconds());
        timerText.setString("Time: " + to_string(remainingTime));

        // Current positions of the apple and caterpillar
        Vector2f applePosition = Object1.getPosition(); // Renamed variable to avoid conflicts
        Vector2f сaterpillarPosition = Object2.getPosition(); // Renamed variable to avoid conflicts

        // Automatic movement of the caterpillar
        сaterpillarPosition += velocity;

        // Reflect the sprite when hitting the screen boundaries
        if (сaterpillarPosition.x <= 0 || сaterpillarPosition.x + Object2.getGlobalBounds().width >= 1920) {
            velocity.x = -velocity.x; // Change direction on X-axis
            Vector2f currentScale = Object2.getScale();
            Object2.setScale(-currentScale.x, currentScale.y); // Flip horizontally
        }
        if (сaterpillarPosition.y <= 0 || сaterpillarPosition.y + Object2.getGlobalBounds().height >= 1042) {
            velocity.y = -velocity.y; // Change direction on Y-axis (no vertical flipping needed)
        }

        Object2.setPosition(сaterpillarPosition); // Update Object2's position

        // Handle keyboard input for moving the apple
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (applePosition.y > -18)
                Object1.move(0, -moveSpeed); // Move up
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (applePosition.y < 928)
                Object1.move(0, moveSpeed); // Move down
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (applePosition.x > 0)
                Object1.move(-moveSpeed, 0); // Move left
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (applePosition.x < 1822)
                Object1.move(moveSpeed, 0); // Move right
        }

        // Check for collision between the apple and caterpillar
        if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds())) {
            collisionSound.play(); // Play collision sound
            appleCounter++;
            scoreText.setString("Apple: " + to_string(appleCounter));

            // Move the apple to a random position
            Object1.setPosition(rand() % (1920 - static_cast<int>(Object1.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));

            // Move the caterpillar to a random position
            Object2.setPosition(rand() % (1920 - static_cast<int>(Object2.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));
        }

        // Check for game completion
        if (appleCounter == 10) {
            finalText.setString("              Congratulations!\nYou feed the caterpillar 10 apples!");

            // Center the text
            FloatRect textBounds = finalText.getLocalBounds();
            finalText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            finalText.setPosition(1920 / 2, 1042 / 2);
            window.clear();
            window.draw(Object3);
            window.draw(finalText);
            window.display();
            sleep(seconds(3)); // Wait for 3 seconds before closing
            window.close();
        }

        // Check for time expiration
        if (remainingTime <= 0) {
            finalText.setString("Game Over! Time's up!");
            FloatRect textBounds = finalText.getLocalBounds();
            finalText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            finalText.setPosition(1920 / 2, 1042 / 2);
            window.clear();
            window.draw(Object3);
            window.draw(finalText);
            window.display();
            sleep(seconds(3));
            window.close();
        }

        // Render everything
        window.clear();
        window.draw(Object3);     // Background
        window.draw(Object1);     // Apple
        window.draw(Object2);     // Caterpillar
        window.draw(scoreText);   // Apple counter
        window.draw(timerText);   // Timer
        window.display();
    }

    return 0;
}
