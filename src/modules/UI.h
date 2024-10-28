#pragma once

class UI {
public:
    // Запускаем менюшку
    static void initialize();

    // Закрываем менюшку
    static void finalize();

    // Обновление ПЕРЕД имгуи кодом
    static void update_begin();

    // Обновление ПОСЛЕ имгуи кода
    static void update_end();
};

