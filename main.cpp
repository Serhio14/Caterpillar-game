#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> // ��� ��������� ��������� � �������, ���� �������
#include <cstdlib>  // ��� rand() � srand()
#include <ctime>    // ��� srand()
using namespace std;
using namespace sf;

int main() {
    // ����������� ���������� �����
    srand(static_cast<unsigned>(time(nullptr)));

    // ��������� ���� � ������
    RenderWindow window(VideoMode(1920, 1042), "Caterpillar game");
    Image icon;
    if (!icon.loadFromFile("Image/icon.jpg")) {
        return 1;
    }
    window.setIcon(1000, 1000, icon.getPixelsPtr());

    // ������������ ���� �����
    Texture Background;
    Background.loadFromFile("Image/Background.jpg");
    Sprite Object3(Background);

    // ��������� ������
    Texture Apple;
    Apple.loadFromFile("Image/Apple.png");
    Sprite Object1(Apple);
    Object1.setPosition(866, 445);  // ��������� ������� � �����

    // �������� ������
    float moveSpeed = 0.3f;

    // ��������� ��������
    Texture �aterpillar;
    �aterpillar.loadFromFile("Image/Caterpillar.png");
    Sprite Object2(�aterpillar);
    Object2.setPosition(200.0f, 200.0f); // ��������� �������

    // ������������ �����
    SoundBuffer collisionSoundBuffer;
    if (!collisionSoundBuffer.loadFromFile("Sounds/ukus_yabluka.wav")) {
        return 1;
    }
    Sound collisionSound(collisionSoundBuffer);

    // ������ ������
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Sounds/Doom Eternal OST - The Only Thing They Fear Is You.mp3")) {
        return 1; // �������� �������� ����� ������
    }
    backgroundMusic.setLoop(true); // ������ ���� �������������
    backgroundMusic.play();

    // �������� ��������
    Vector2f velocity(0.25f, 0.25f); // �������� � ������� �� ����

    // ˳������� ������� �����
    int appleCounter = 0;
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        return 1; // ��������, �� ���� ���� ������
    }

    // ˳������� �����
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setString("Apple: 0");

    // ������
    Clock gameClock;
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 40);

    // ����� ���������� �����������
    Text finalText;
    finalText.setFont(font);
    finalText.setCharacterSize(100); // ������� ������� �����
    finalText.setFillColor(Color::White);
    finalText.setStyle(Text::Bold);

    // �������� ����
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // ��������� �������
        int remainingTime = 30 - static_cast<int>(gameClock.getElapsedTime().asSeconds());
        timerText.setString("Time: " + to_string(remainingTime));

        // ������ ���������� ������ � ��������
        Vector2f applePosition = Object1.getPosition(); // ������������� ����� ��� ��������� ��������
        Vector2f �aterpillarPosition = Object2.getPosition(); // ������������� ����� ��� ��������� ��������

        // ������������ ��� ��������
        �aterpillarPosition += velocity;

        // ³�������� �� ��� ������
        if (�aterpillarPosition.x <= 0 || �aterpillarPosition.x + Object2.getGlobalBounds().width >= 1920) {
            velocity.x = -velocity.x; // ���� �������� �� x
        }
        if (�aterpillarPosition.y <= 0 || �aterpillarPosition.y + Object2.getGlobalBounds().height >= 1042) {
            velocity.y = -velocity.y; // ���� �������� �� y
        }

        Object2.setPosition(�aterpillarPosition); // ��������� ������� Object2

        // ������� ����� ��� ���������� ������
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (applePosition.y > -18)
                Object1.move(0, -moveSpeed); // ��� �����
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (applePosition.y < 899)
                Object1.move(0, moveSpeed); // ��� ����
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (applePosition.x > -28)
                Object1.move(-moveSpeed, 0); // ��� ����
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (applePosition.x < 1782)
                Object1.move(moveSpeed, 0); // ��� ������
        }

        // �������� �� �������� �� ������� �� ���������
        if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds())) {
            collisionSound.play(); // ����������� �����
            appleCounter++;
            scoreText.setString("Apple: " + to_string(appleCounter));

            // ���������� ������ � ��������� �������
            Object1.setPosition(rand() % (1920 - static_cast<int>(Object1.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));

            // ���������� ������ � ��������� �������
            Object2.setPosition(rand() % (1920 - static_cast<int>(Object2.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));
        }

        /// �������� ���������� ���
        if (appleCounter == 10) {
            finalText.setString("              Congratulations!\nYou fed the caterpillar 10 apples!");

            // �������� �����
            FloatRect textBounds = finalText.getLocalBounds();
            finalText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            finalText.setPosition(1920 / 2, 1042 / 2);
            window.clear();
            window.draw(Object3);
            window.draw(finalText);
            window.display();
            sleep(seconds(3)); // ���������� 3 ������� ����� ���������
            window.close();
        }

        // �������� ���������� ����
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

        // ���������
        window.clear();
        window.draw(Object3);     // ���
        window.draw(Object1);     // ������
        window.draw(Object2);     // ��������
        window.draw(scoreText);   // ˳�������
        window.draw(timerText);   // ������
        window.display();
    }

    return 0;
}