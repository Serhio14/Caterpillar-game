#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> // Для виведення лічильника в консоль, якщо потрібно
#include <cstdlib>  // Для rand() і srand()
#include <ctime>    // Для srand()
using namespace std;
using namespace sf;

int main() {
    // Ініціалізація випадкових чисел
    srand(static_cast<unsigned>(time(nullptr)));

    // Створення вікна і іконки
    RenderWindow window(VideoMode(1920, 1042), "Caterpillar game");
    Image icon;
    if (!icon.loadFromFile("Image/icon.jpg")) {
        return 1;
    }
    window.setIcon(1000, 1000, icon.getPixelsPtr());

    // Встановлення фону трави
    Texture Background;
    Background.loadFromFile("Image/Background.jpg");
    Sprite Object3(Background);

    // Створення яблука
    Texture Apple;
    Apple.loadFromFile("Image/Apple.png");
    Sprite Object1(Apple);
    Object1.setPosition(866, 445);  // Початкова позиція в центрі

    // Швидкість яблука
    float moveSpeed = 0.3f;

    // Створення гусениці
    Texture Сaterpillar;
    Сaterpillar.loadFromFile("Image/Caterpillar.png");
    Sprite Object2(Сaterpillar);
    Object2.setPosition(200.0f, 200.0f); // Початкова позиція

    // Завантаження звуку
    SoundBuffer collisionSoundBuffer;
    if (!collisionSoundBuffer.loadFromFile("Sounds/ukus_yabluka.wav")) {
        return 1;
    }
    Sound collisionSound(collisionSoundBuffer);

    // Фонова музика
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("Sounds/Doom Eternal OST - The Only Thing They Fear Is You.mp3")) {
        return 1; // Перевірте наявність файлу музики
    }
    backgroundMusic.setLoop(true); // Музика буде повторюватися
    backgroundMusic.play();

    // Швидкість гусениці
    Vector2f velocity(0.25f, 0.25f); // Швидкість у пікселях за кадр

    // Лічильник зібраних яблук
    int appleCounter = 0;
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        return 1; // Перевірте, чи існує файл шрифту
    }

    // Лічильник яблук
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setString("Apple: 0");

    // Таймер
    Clock gameClock;
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(10, 40);

    // Текст фінального повідомлення
    Text finalText;
    finalText.setFont(font);
    finalText.setCharacterSize(100); // Зробимо великий текст
    finalText.setFillColor(Color::White);
    finalText.setStyle(Text::Bold);

    // Основний цикл
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Оновлення таймера
        int remainingTime = 30 - static_cast<int>(gameClock.getElapsedTime().asSeconds());
        timerText.setString("Time: " + to_string(remainingTime));

        // Поточні координати яблука і цесениці
        Vector2f applePosition = Object1.getPosition(); // Перейменовано змінну для уникнення конфліктів
        Vector2f сaterpillarPosition = Object2.getPosition(); // Перейменовано змінну для уникнення конфліктів

        // Автоматичний рух гусениці
        сaterpillarPosition += velocity;

        // Відбивання від меж екрану
        if (сaterpillarPosition.x <= 0 || сaterpillarPosition.x + Object2.getGlobalBounds().width >= 1920) {
            velocity.x = -velocity.x; // Зміна напрямку по x
        }
        if (сaterpillarPosition.y <= 0 || сaterpillarPosition.y + Object2.getGlobalBounds().height >= 1042) {
            velocity.y = -velocity.y; // Зміна напрямку по y
        }

        Object2.setPosition(сaterpillarPosition); // Оновлення позиції Object2

        // Обробка клавіш для переміщення яблука
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (applePosition.y > -18)
                Object1.move(0, -moveSpeed); // Рух вгору
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (applePosition.y < 899)
                Object1.move(0, moveSpeed); // Рух вниз
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (applePosition.x > -28)
                Object1.move(-moveSpeed, 0); // Рух вліво
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (applePosition.x < 1782)
                Object1.move(moveSpeed, 0); // Рух вправо
        }

        // Перевірка на зіткнення між яблуком та гусеницею
        if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds())) {
            collisionSound.play(); // Програвання звуку
            appleCounter++;
            scoreText.setString("Apple: " + to_string(appleCounter));

            // Переміщення яблука в випадкову позицію
            Object1.setPosition(rand() % (1920 - static_cast<int>(Object1.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));

            // Переміщення яблука в випадкову позицію
            Object2.setPosition(rand() % (1920 - static_cast<int>(Object2.getGlobalBounds().width)),
                rand() % (1042 - static_cast<int>(Object2.getGlobalBounds().height)));
        }

        /// Перевірка завершення гри
        if (appleCounter == 10) {
            finalText.setString("              Congratulations!\nYou fed the caterpillar 10 apples!");

            // Центруємо текст
            FloatRect textBounds = finalText.getLocalBounds();
            finalText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            finalText.setPosition(1920 / 2, 1042 / 2);
            window.clear();
            window.draw(Object3);
            window.draw(finalText);
            window.display();
            sleep(seconds(3)); // Очікування 3 секунди перед закриттям
            window.close();
        }

        // Перевірка завершення часу
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

        // Малювання
        window.clear();
        window.draw(Object3);     // Фон
        window.draw(Object1);     // Яблуко
        window.draw(Object2);     // Гусениця
        window.draw(scoreText);   // Лічильник
        window.draw(timerText);   // Таймер
        window.display();
    }

    return 0;
}