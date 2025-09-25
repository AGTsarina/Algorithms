/**************************************************************
 * Упрощенный алгоритм RSA с использованием типа long
 * 
 * Ограничения упрощенной версии:
 * 1. Безопасность:
 *   НЕ криптографически безопасно
 *   Маленькие простые числа
 *   Уязвимо к факторизации
 * 2. Практические ограничения:
 *   Максимальное сообщение: < n
 *   Ограниченный размер блока
 * 3. Только для демонстрационных целей
 ***************************************************************/

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

class SimpleRSA {
private:
    long p, q;          // Простые числа
    long n;             // Модуль: n = p * q
    long phi;           // Функция Эйлера: phi = (p-1) * (q-1)
    long e;             // Открытая экспонента
    long d;             // Закрытая экспонента
    
    // Проверка на простоту числа (упрощенная)
    bool isPrime(long num) {
        if (num <= 1) return false;
        if (num <= 3) return true;
        if (num % 2 == 0 || num % 3 == 0) return false;
        
        for (long i = 5; i * i <= num; i += 6) {
            if (num % i == 0 || num % (i + 2) == 0)
                return false;
        }
        return true;
    }
    
    // Наибольший общий делитель
    long gcd(long a, long b) {
        while (b != 0) {
            long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
    
    // Расширенный алгоритм Евклида для нахождения обратного элемента
    // Элементы а и b называются мультипликативно взаимно обратимыми,
    // если аb = 1 mod n.
    // Например, в группе по модулю 10 элементы 3 и 7 взаимно обратимы, так как 3 × 7 = 1
    long modInverse(long a, long m) {
        long m0 = m, t, q;
        long x0 = 0, x1 = 1;
        
        if (m == 1) return 0;
        
        while (a > 1) {
            q = a / m;
            t = m;
            m = a % m;
            a = t;
            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }
        
        if (x1 < 0) x1 += m0;
        return x1;
    }
    
    // Быстрое возведение в степень по модулю
    long modPow(long base, long exponent, long modulus) {
        if (modulus == 1) return 0;
        long result = 1;
        base = base % modulus;
        
        while (exponent > 0) {
            if (exponent % 2 == 1)
                result = (result * base) % modulus;
            exponent = exponent >> 1;
            base = (base * base) % modulus;
        }
        return result;
    }

public:
    SimpleRSA() {
        // Используем небольшие простые числа для демонстрации
        p = 61;
        q = 53;
        generateKeys();
    }
    
    void generateKeys() {
        // Вычисляем модуль
        n = p * q;
        
        // Вычисляем функцию Эйлера
        phi = (p - 1) * (q - 1);
        
        // Выбираем открытую экспоненту (обычно 65537, но для маленьких чисел берем меньше)
        e = 17;
        while (gcd(e, phi) != 1) {
            e++;
        }
        
        // Вычисляем закрытую экспоненту
        d = modInverse(e, phi);
        
        std::cout << "Сгенерированы ключи RSA:\n";
        std::cout << "p = " << p << ", q = " << q << "\n";
        std::cout << "n = " << n << "\n";
        std::cout << "phi(n) = " << phi << "\n";
        std::cout << "Открытый ключ (e, n): (" << e << ", " << n << ")\n";
        std::cout << "Закрытый ключ (d, n): (" << d << ", " << n << ")\n";
    }
    
    // Шифрование одного числа
    long encrypt(long message) {
        if (message >= n) {
            throw std::invalid_argument("Сообщение должно быть меньше n");
        }
        return modPow(message, e, n);
    }
    
    // Расшифрование одного числа
    long decrypt(long ciphertext) {
        return modPow(ciphertext, d, n);
    }
    
    // Шифрование строки (поблочно)
    std::vector<long> encryptString(const std::string& message) {
        std::vector<long> result;
        
        for (char c : message) {
            // Преобразуем символ в число (ASCII код)
            long charValue = static_cast<long>(c);
            result.push_back(encrypt(charValue));
        }
        
        return result;
    }
    
    // Расшифрование строки (поблочно)
    std::string decryptString(const std::vector<long>& ciphertext) {
        std::string result;
        
        for (long encryptedChar : ciphertext) {
            long decryptedValue = decrypt(encryptedChar);
            result += static_cast<char>(decryptedValue);
        }
        
        return result;
    }
    
    // Получить максимальное число для шифрования
    long getMaxMessage() const {
        return n - 1;
    }
    
    // Получить открытый ключ
    std::pair<long, long> getPublicKey() const {
        return {e, n};
    }
    
    // Получить закрытый ключ
    std::pair<long, long> getPrivateKey() const {
        return {d, n};
    }
};

// Демонстрация работы алгоритма
int main() {
    try {
        SimpleRSA rsa;
        
        std::cout << "\n=== Демонстрация шифрования чисел ===\n";
        long originalNumber = 42;
        std::cout << "Исходное число: " << originalNumber << "\n";
        
        long encrypted = rsa.encrypt(originalNumber);
        std::cout << "Зашифрованное число: " << encrypted << "\n";
        
        long decrypted = rsa.decrypt(encrypted);
        std::cout << "Расшифрованное число: " << decrypted << "\n";
        
        std::cout << "\n=== Демонстрация шифрования строки ===\n";
        std::string message = "HELLO";
        std::cout << "Исходное сообщение: " << message << "\n";
        
        auto encryptedMessage = rsa.encryptString(message);
        std::cout << "Зашифрованное сообщение: ";
        for (long val : encryptedMessage) {
            std::cout << val << " ";
        }
        std::cout << "\n";
        
        std::string decryptedMessage = rsa.decryptString(encryptedMessage);
        std::cout << "Расшифрованное сообщение: " << decryptedMessage << "\n";
        
        std::cout << "\n=== Проверка корректности ===\n";
        std::cout << "Максимальное число для шифрования: " << rsa.getMaxMessage() << "\n";
        
        // Проверка RSA свойства: (m^e)^d ≡ m mod n
        long testValue = 123;
        long testEncrypted = rsa.encrypt(testValue);
        long testDecrypted = rsa.decrypt(testEncrypted);
        std::cout << "Проверка RSA свойства: " << testValue << " -> " 
                  << testEncrypted << " -> " << testDecrypted 
                  << " (результат: " << (testValue == testDecrypted ? "OK" : "ERROR") << ")\n";
                  
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
    
    return 0;
}