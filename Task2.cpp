#include <iostream> // консоль - cout
#include <fstream>  // работа с файлами

class Adr
{
private:
    std::string city;
    std::string street;
    unsigned short house;
    unsigned short apartment;
public:
    Adr()
    {
        house = 0;
        apartment = 0;
    }
    bool setCity(std::string str)
    {
        if (str == "") return false;
        city = str;
        return true;
    }
    bool setStreet(std::string str)
    {
        if (str == "") return false;
        street = str;
        return true;
    }
    bool setHouse(short num)
    {
        if (num <= 0) return false;
        house = static_cast<unsigned short>(num);
        return true;
    }
    bool setApartment(short num)
    {
        if (num <= 0) return false;
        apartment = static_cast<unsigned short>(num);
        return true;
    }
    std::string getCity() { return city; }
    std::string getStreet() { return street; }
    unsigned short getHouse() { return house; }
    unsigned short getApartment() { return apartment; }
};

enum Err { NO_ERR, CANT_OP, FILE_ERR };

void memFree(Adr*&);

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");   // задаём русский текст
    system("chcp 1251");            // настраиваем кодировку консоли
    std::system("cls");

    std::cout << "Задача 2. Сортируем адреса\n";
    std::cout << "--------------------------\n" << std::endl;

    const std::string IN_FILE = "in.txt";
    const std::string OUT_FILE = "out.txt";

    Adr* adress = nullptr;          // указатель на структуру адресов
    unsigned int numAdr = 0;        // адресов в базе
    Err err = NO_ERR;               // ошибок чтения - нет
    std::ifstream file(IN_FILE);

    // читаю из файла
    if (file.is_open())
    {
        int temp; // временная переменная, т.к. проверка на отриц. значение
        if ((file >> temp) && temp > 0)
        {
            numAdr = temp;

            adress = new Adr[numAdr];    // создаю базу адресов в локальной памяти
            for (unsigned int i = 0; i < numAdr; ++i)
            {
                std::string str;
                short temp;
                if (!(file >> str && adress[i].setCity(str) &&
                    file >> str && adress[i].setStreet(str) &&
                    file >> temp && adress[i].setHouse(temp) &&
                    file >> temp && adress[i].setApartment(temp)))
                {
                    memFree(adress);
                    err = FILE_ERR;
                    break;
                }
            }
        }
        else err = FILE_ERR;

        file.close();
    }
    else err = CANT_OP;


    // сортировка
    if (adress)
    {
        bool cont;
        do
        {
            cont = false;
            Adr temp = adress[0];
            for (int i = 1; i < numAdr; ++i)
            {
                if (adress[i].getCity() < temp.getCity())
                {
                    adress[i - 1] = adress[i];
                    adress[i] = temp;
                    cont = true;
                }
                else temp = adress[i];
            }
        } while (cont);
    }


    // пишу в файл
    std::ofstream file_wr(OUT_FILE);
    if (file_wr.is_open())
    {
        if (adress)
        {
            file_wr << numAdr << std::endl;
            for (int i = 0; i < numAdr; ++i)
            {
                file_wr << adress[i].getCity() << ", "
                    << adress[i].getStreet() << ", "
                    << adress[i].getHouse() << ", "
                    << adress[i].getApartment() << std::endl;
            }
            std::cout << "Смотри файл: " << OUT_FILE << std::endl;
        }
        else // если входной файл не считан, выходной создается пустым/очищается
        {
            switch (err)
            {
            case CANT_OP:
                std::cout << "Не удалось открыть файл: " << IN_FILE;
                break;
            case FILE_ERR:
                std::cout << "Ошибка! Файл повреждён!";
                break;
            default:
                std::cout << "Ошибка!";
            }
            std::cout << std::endl;
        }

        file_wr.close();
    }
    else std::cout << "Не удалось соранить файл: " << OUT_FILE << std::endl;

    memFree(adress);
    return 0;
}

void memFree(Adr*& memPtr)
{
    if (memPtr)
    {
        //std::cout << "Освобождаю память..." << std::endl;
        delete[] memPtr;
        memPtr = nullptr;
    }
    //else std::cout << "Нечего освобождать!" << std::endl;
}



/*
Задача 2. Сортируем адреса
В этом задании вы будете читать из файла адреса и выводить их в другой файл в другом формате, отсортированными по названию города.

Вам дан файл in.txt, в котором находится N адресов. Число N идёт первым в файле. После числа N располагаются N блоков по 4 строки
в каждом. Каждый блок состоит из следующих строк: название города, название улицы, номер дома, номер квартиры.

Задача программы заключается в том, чтобы считать адреса из файла in.txt, отсортировать их по названию города и вывести
отсортированный список адресов в файл out.txt в другом формате. Не забудьте вывести количество адресов в начале.

Города должны быть отсортированы в алфавитном порядке.

Формат вывода адреса: <название города>, <название улицы>, <номер дома>, <номер квартиры>.

Пример работы программы
in.txt
5
Москва
Строителей
34
12
Новокузнецк
Науки
5
16
Омск
Пушкина
2
13
Москва
Арбат
4
1
Новосибирск
Мира
85
64
out.txt
5
Москва, Строителей, 34, 12
Москва, Арбат, 4, 1
Новокузнецк, Науки, 5, 16
Новосибирск, Мира, 85, 64
Омск, Пушкина, 2, 13
*/