#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> // To output the counter to the console if needed
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For srand()
using namespace std;
using namespace sf;

int main() {
    // Initialization of random numbers
    srand(static_cast<unsigned>(time(nullptr)));

    // Creating a window and icon
    RenderWindow window(VideoMode(1920, 1042), "Caterpillar game");
    Image icon;
    if (!icon.loadFromFile("Image/icon.jpg")) {
        return 1;
    }
    window.setIcon(1000, 1000, icon.getPixelsPtr());

    // Set grass background
    Texture Background;
    Background.loadFromFile("Image/Background.jpg");
    Sprite Object3(Background);

    // Create an apple
    Texture Apple;
    Apple.loadFromFile("Image/Apple.png");
    Sprite Object1(Apple);
    Object1.setPosition(866, 445);  // Initial position in the center

    // The speed of the apple
    float moveSpeed = 0.3f;

    // Create the caterpillar
    Texture Ñaterpillar;
    Ñaterpillar.loadFromFile("Image/Caterpillar.png");
    Sprite Object2(Ñaterpillar);
    Object2.setPosition(200.0f, 200.0f); // Initial position in the center

    // Loading sound
    SoundBuffer collisionSoundBuffer;
    if (!collisionSoundBuffer.loadFromFile("Sounds/ukus_yabluka.wav")) {
        return 1;
    }
    Sound collisionSound(collisionSoundBuffer);

    // Background music
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Sounds/Doom Eternal OST - The Only Thing They Fear Is You.mp3")) {
        return 1; // Check for music file
    }
    backgroundMusic.setLoop(true); // The music will repeat
    backgroundMusic.play();

    // Track speed
    Vector2f velocity(0.25f, 0.25f); // Speed ​​in pixels per frame

    // Counter of collected apples
    int appleCounter = 0;
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        return 1; // Check if the font file exists
    }

    // Apple counter
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

    // The text of the final message
    Text finalText;
    finalText.setFont(font);
    finalText.setCharacterSize(100); // Let's make a large text
    finalText.setFillColor(Color::White);
    finalText.setStyle(Text::Bold);

    // The main cycle
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Update the timer
        int remainingTime = 30 - static_cast<int>(gameClock.getElapsedTime().asSeconds());
        timerText.setString("Time: " + to_string(remainingTime));

        // The current coordinates of the apple and the cherry tree
        Vector2f applePosition = Object1.getPosition(); // Renamed the variable to avoid conflicts
        Vector2f ñaterpillarPosition = Object2.getPosition(); //Renamed the variable to avoid conflicts

        // Automatic track movement
        ñaterpillarPosition += velocity;

        // Â³äáèâàííÿ â³ä ìåæ åêðàíó
        if (ñaterpillarPosition.x <= 0 || ñaterpillarPosition.x + Object2.getGlobalBounds().width >= 1920) {
            velocity.x = -velocity.x; // Change direction in x
        }
        if (ñaterpillarPosition.y <= 0 || ñaterpillarPosition.y + Object2.getGlobalBounds().height >= 1042) {
            velocity.y = -velocity.y; // Change direction in y
        }

        Object2.setPosition(ñaterpillarPosition); // Update the position of Object2

        // Handle the keys to move the apple
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (applePosition.y > -18)
                Object1.move(0, -moveSpeed);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (applePosition.y < 899)
                Object1.move(0, moveSpeed);
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (applePosition.x > -28)
                Object1.move(-moveSpeed, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (applePosition.x < 1782)
                Object1.move(moveSpeed, 0);
        }

        // Check for a collision between an apple and a caterpillar
        if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds())) {
            collisionSound.play(); // Play sound
            appleCounter++;
            scoreText.setString("Apple: " + to_string(appleCounter));

            // Move the apple to a random position
            Object1.setPosition(rand() % (1920 - static_cast<int>(Object1.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));

            // Move the caterpillat to a random position
            Object2.setPosition(rand() % (1920 - static_cast<int>(Object2.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));
        }

        // Check for game completion
        if (appleCounter == 10) {
            finalText.setString("              Congratulations!\nYou fed the caterpillar 10 apples!");

            // Center the text
            FloatRect textBounds = finalText.getLocalBounds();
            finalText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            finalText.setPosition(1920 / 2, 1042 / 2);
            window.clear();
            window.draw(Object3);
            window.draw(finalText);
            window.display();
            sleep(seconds(3)); // Wait 3 seconds before closing
            window.close();
        }

        // Timeout check
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

       // Output
        window.clear();
        window.draw(Object3);     // background
        window.draw(Object1);     // apple
        window.draw(Object2);     // caterpillar
        window.draw(scoreText);   // counter
        window.draw(timerText);   // timer
        window.display();
    }

    return 0;
}
