#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <map>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

// Базовый класс File для представления файлов
class File
{
protected:
    string name;               // Название файла
    string path;               // Путь к файлу
    string creationDate;       // Дата создания файла
    string modificationDate;   // Дата последнего изменения файла
    size_t size;               // Размер файла в килобайтах
    vector<string> tags;       // Список тегов, привязанных к файлу

public:
    // Конструктор для инициализации всех параметров файла
    File(const string& name, const string& path, const string& creationDate, size_t size, const vector<string>& tags) : name(name), path(path), creationDate(creationDate), size(size), tags(tags) {}

    // Деструктор
    virtual ~File() = default;

    // Геттеры для доступа к данным о файле
    string getName() const { return name; }
    string getPath() const { return path; }
    string getCreationDate() const { return creationDate; }
    string getModificationDate() const { return modificationDate; }
    size_t getSize() const { return size; }
    vector<string> getTags() const { return tags; }

    // Сеттер для модификации даты последнего изменения
    void setModificationDate(const string& date) { modificationDate = date; }

    // Добавление нового тега к файлу
    void addTag(const string& tag) { tags.push_back(tag); }

    // Открытие файла (базовый метод для всех файлов)
    virtual void open() const{}

    // Виртуальный метод для получения типа файла (переопределяется в производных классах)
    virtual string getType() const = 0;

    // Проверка наличия тега у файла
    bool hasTag(const string& tag) const
    {
        return find(tags.begin(), tags.end(), tag) != tags.end();
    }

    // Оператор сравнения для сортировки файлов по дате создания
    bool operator<(const File& other) const
    {
        return creationDate < other.creationDate;
    }
};

// Класс TextDocument для текстовых документов
class TextDocument : public File
{
public:
    TextDocument(const string& name, const string& path, const string& creationDate, size_t size, const vector<string>& tags)
        : File(name, path, creationDate, size, tags) {}

    // Переопределенный метод для получения типа файла
    string getType() const override { return "TextDocument"; }

    void open() const override
    {
        cout << "Открыт текстовый документ: " << name << endl;
    }
};

// Класс PDFDocument для PDF-документов
class PDFDocument : public File
{
public:
    PDFDocument(const string& name, const string& path, const string& creationDate, size_t size, const vector<string>& tags)
        : File(name, path, creationDate, size, tags) {}

    // Переопределенный метод для получения типа файла
    string getType() const override { return "PDFDocument"; }

    void open() const override
    {
        cout << "Открыт PDF-документ: " << name << endl;
    }
};

// Класс ImageFile для изображений
class ImageFile : public File
{
public:
    ImageFile(const string& name, const string& path, const string& creationDate, size_t size, const vector<string>& tags)
        : File(name, path, creationDate, size, tags) {}

    // Переопределенный метод для получения типа файла
    string getType() const override { return "ImageFile"; }

    void open() const override
    {
        cout << "Открыто изображения: " << name << endl;
    }


};

// Класс VideoFile для видеофайлов
class VideoFile : public File
{
public:
    VideoFile(const string& name, const string& path, const string& creationDate, size_t size, const vector<string>& tags)
        : File(name, path, creationDate, size, tags) {}

    // Переопределенный метод для получения типа файла
    string getType() const override { return "VideoFile"; }

    void open() const override
    {
        cout << "Открыт видеофайл: " << name << endl;
    }


};

// Класс AudioFile для аудиофайлов
class AudioFile : public File
{
public:
    AudioFile(const string& name, const string& path, const string& creationDate, size_t size, const vector<string>& tags)
        : File(name, path, creationDate, size, tags) {}

    // Переопределенный метод для получения типа файла
    string getType() const override { return "AudioFile"; }

    void open() const override
    {
        cout << "Открыт аудиофайл: " << name << endl;
    }
};

// Класс для управления файлами: добавление, удаление, архивация и поиск файлов
class FileManager
{
private:
    vector<File*> files;  // Список файлов
    vector<File*> archive; // Список архивированных файлов

public:
    // Деструктор для освобождения памяти
    ~FileManager()
    {
        for (auto file : files) delete file;
        for (auto file : archive) delete file;
    }

    // Метод для добавления нового файла
    void addFile(File* file)
    {
        files.push_back(file);
    }

    // Метод для перемещения файла в архив
    void moveToArchive(const string& name)
    {
        auto it = find_if(files.begin(), files.end(), [&](File* f) { return f->getName() == name; });
        if (it != files.end())
        {
            archive.push_back(*it);  // Добавляем в архив
            files.erase(it);  // Удаляем из основной коллекции
            cout << "Файл " << name << " перемещен в архив." << endl;
        }
        else
        {
            cout << "Файл " << name << " не найден." << endl;
        }
    }

    // Метод для удаления файла
    void deleteFile(const string& name) {
        auto it = find_if(files.begin(), files.end(), [&](File* f) { return f->getName() == name; });
        if (it != files.end())
        {
            cout << "Вы уверены, что хотите удалить файл " << name << "? (y/n): ";
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y')
            {
                delete* it;  // Освобождаем память
                files.erase(it);  // Удаляем файл из списка
                cout << "Файл " << name << " удален." << endl;
            }
        }
        else
        {
            cout << "Файл " << name << " не найден." << endl;
        }
    }

    // Метод для поиска файлов с возможностью фильтрации
    vector<File*> search(const string& type = "", const string& tag = "", size_t minSize = 0, size_t maxSize = SIZE_MAX, const string& creationDate = "", const string& modificationDate = "") {
        vector<File*> result;
        for (auto file : files)
        {
            // Фильтрация файлов по заданным критериям
            if ((type.empty() || file->getType() == type) &&
                (tag.empty() || file->hasTag(tag)) &&
                (file->getSize() >= minSize && file->getSize() <= maxSize) &&
                (creationDate.empty() || file->getCreationDate() == creationDate) &&
                (modificationDate.empty() || file->getModificationDate() == modificationDate))
            {
                result.push_back(file);
            }
        }
        return result;
    }

    // Метод сортировки файлов по различным критериям
    void sortFiles(vector<File*>& files, const string& sortBy = "name")
    {
        if (sortBy == "name")
        {
            sort(files.begin(), files.end(), [](File* a, File* b) { return a->getName() < b->getName(); });
        }
        else if (sortBy == "creationDate")
        {
            sort(files.begin(), files.end(), [](File* a, File* b) { return a->getCreationDate() < b->getCreationDate(); });
        }
        else if (sortBy == "size")
        {
            sort(files.begin(), files.end(), [](File* a, File* b) { return a->getSize() < b->getSize(); });
        }
    }

    File* getFileByName(const string& name)
    {
        auto it = find_if(files.begin(), files.end(), [&](File* f) { return f->getName() == name; });
        return (it != files.end()) ? *it : nullptr;
    }


    // Метод для генерации отчета по найденным файлам
    void generateReport(const vector<File*>& files)
    {
        size_t totalFiles = files.size();  // Общее количество файлов
        size_t totalSize = 0;  // Общий размер файлов
        string lastModificationDate = "";  // Дата последнего изменения файлов

        // Проход по всем найденным файлам для вычисления статистики
        for (auto file : files) 
        {
            totalSize += file->getSize();
            if (file->getModificationDate() > lastModificationDate)
            {
                lastModificationDate = file->getModificationDate();
            }
        }

        // Вывод статистики
        cout << "Статистика по найденным файлам:" << endl;
        cout << "Количество файлов: " << totalFiles << endl;
        cout << "Общий размер: " << totalSize << " KB" << endl;
        cout << "Средний размер: " << (totalFiles > 0 ? totalSize / totalFiles : 0) << " KB" << endl;
        cout << "Последнее изменение: " << lastModificationDate << endl;
    }

    void addFilesFromTextFile(const string& fileName)
    {
        ifstream file(fileName);
        string line;
        while (getline(file, line))
        {
            string name, path, creationDate, sizeStr, tagsStr;
            size_t size;
            vector<string> tags;

            stringstream ss(line);
            getline(ss, name, ',');
            getline(ss, path, ',');
            getline(ss, creationDate, ',');
            getline(ss, sizeStr, ',');
            getline(ss, tagsStr);

            // Парсим размер файла
            size_t pos = sizeStr.find("KB");
            if (pos != string::npos)
            {
                size = stoi(sizeStr.substr(0, pos));
            }
            else
            {
                pos = sizeStr.find("MB");
                if (pos != string::npos)
                {
                    size = stoi(sizeStr.substr(0, pos)) * 1024;  // Переводим в KB
                }
            }

            // Парсим теги
            size_t start = 0, end;
            while ((end = tagsStr.find(',', start)) != string::npos)
            {
                tags.push_back(tagsStr.substr(start, end - start));
                start = end + 1;
            }
            tags.push_back(tagsStr.substr(start));

            // Создаем объект файла и добавляем его в систему
            File* newFile = nullptr;
            if (name.find(".pdf") != string::npos)
            {
                newFile = new PDFDocument(name, path, creationDate, size, tags);
            }
            else if (name.find(".jpg") != string::npos)
            {
                newFile = new ImageFile(name, path, creationDate, size, tags);
            }
            else if (name.find(".txt") != string::npos)
            {
                newFile = new TextDocument(name, path, creationDate, size, tags);
            }
            else if (name.find(".mp4") != string::npos)
            {
                newFile = new VideoFile(name, path, creationDate, size, tags);
            }
            else if (name.find(".mp3") != string::npos)
            {
                newFile = new AudioFile(name, path, creationDate, size, tags);
            }

            if (newFile)
            {
                addFile(newFile);
            }
        }
        file.close();
    }


};

int main()
{
    // Установка кодировки консоли для корректного вывода текста на русском языке
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    FileManager manager;

    // Добавление примера файлов в систему
    manager.addFile(new PDFDocument("Report.pdf", "C:/Documents", "10.05.2023", 200, { "work", "project" }));
    manager.addFile(new ImageFile("Image.jpg", "C:/Pictures", "15.11.2022", 1500, { "vacation", "family" }));
    manager.addFile(new TextDocument("Notes.txt", "C:/Documents", "01.01.2021", 50, { "work", "personal" }));
    manager.addFile(new VideoFile("Video.mp4", "C:/Videos", "20.02.2020", 500000, { "work" }));
    manager.addFile(new AudioFile("Podcast.mp3", "C:/Music", "05.04.2021", 100000, { "project" }));

    // Основной цикл программы для взаимодействия с пользователем
    while (true)
    {
        cout << "Выберите действие:" << endl;
        cout << "1. Поиск файлов" << endl;
        cout << "2. Открыть файл" << endl;
        cout << "3. Архивировать файл" << endl;
        cout << "4. Удалить файл" << endl;
        cout << "5. Добавить новый файл" << endl;
        cout << "6. добавить из файла" << endl;
        cout << "7. Выход" << endl;

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string type, tag, creationDate, modificationDate;
            size_t minSize, maxSize;

            cout << "Введите тип файла (оставьте пустым для любого): ";
            cin.ignore(); // Игнорируем символ новой строки, оставшийся после предыдущего ввода
            getline(cin, type);

            cout << "Введите тег (оставьте пустым для любого): ";
            getline(cin, tag);

            cout << "Введите минимальный размер (в KB): ";
            cin >> minSize;

            cout << "Введите максимальный размер (в KB): ";
            cin >> maxSize;

            cin.ignore(); // Игнорируем символ новой строки, оставшийся после предыдущего ввода
            cout << "Введите дату создания (оставьте пустым для любой): ";
            getline(cin, creationDate);

            cout << "Введите дату изменения (оставьте пустым для любой): ";
            getline(cin, modificationDate);

            // Выполнение поиска по заданным параметрам
            auto searchResults = manager.search(type, tag, minSize, maxSize, creationDate, modificationDate);
            manager.sortFiles(searchResults, "creationDate");

            // Вывод результатов поиска
            for (auto file : searchResults) 
            {
                cout << file->getName() << " (" << file->getCreationDate() << ", " << file->getSize() << " KB, " << file->getPath() << ", Теги: ";
                for (auto tag : file->getTags()) 
                {
                    cout << tag << " ";
                }
                cout << ")" << endl;
            }

            // Генерация отчета по найденным файлам
            manager.generateReport(searchResults);
            break;
        }

        case 2:
        {
            string fileName;
            cout << "Введите имя файла для открытия: ";
            cin.ignore(); // Игнорируем символ новой строки, оставшийся после предыдущего ввода
            getline(cin, fileName);

            // Используем метод FileManager для поиска файла
            File* file = manager.getFileByName(fileName);

            if (file)
            {
                file->open(); // Вызов переопределенного метода open() для соответствующего типа файла
            }
            else
            {
                cout << "Файл с именем " << fileName << " не найден." << endl;
            }
            break;
        }


        case 3:
        {
            string fileName;
            cout << "Введите имя файла для архивирования: ";
            cin >> fileName;
            manager.moveToArchive(fileName);
            break;
        }

        case 4:
        {
            string fileName;
            cout << "Введите имя файла для удаления: ";
            cin >> fileName;
            manager.deleteFile(fileName);
            break;
        }

        case 5:
        {
            string name, path, creationDate, modificationDate;
            size_t size;
            vector<string> tags;
            string tag;
            string type;

            cout << "Введите тип файла (txt, pdf, jpg, mp4, mp3): ";
            cin.ignore(); // Игнорируем символ новой строки, оставшийся после предыдущего ввода
            getline(cin, type);

            cout << "Введите название файла: ";
            getline(cin, name);

            cout << "Введите путь к файлу: ";
            getline(cin, path);

            cout << "Введите дату создания (например, 01.01.2023): ";
            getline(cin, creationDate);

            cout << "Введите размер файла (в KB): ";
            cin >> size;

            cout << "Введите теги (по одному, для завершения введите пустую строку):" << endl;
            cin.ignore(); // Игнорируем символ новой строки
            while (true)
            {
                getline(cin, tag);
                if (tag.empty()) break;
                tags.push_back(tag);
            }

            File* newFile = nullptr;

            // Создаем объект соответствующего производного класса
            if (type == "txt") 
            {
                newFile = new TextDocument(name, path, creationDate, size, tags);
            }
            else if (type == "pdf") 
            {
                newFile = new PDFDocument(name, path, creationDate, size, tags);
            }
            else if (type == "jpg") 
            {
                newFile = new ImageFile(name, path, creationDate, size, tags);
            }
            else if (type == "mp4") 
            {
                newFile = new VideoFile(name, path, creationDate, size, tags);
            }
            else if (type == "mp3") 
            {
                newFile = new AudioFile(name, path, creationDate, size, tags);
            }
            else
            {
                cout << "Неизвестный тип файла." << endl;
                break;
            }

            if (newFile) 
            {
                manager.addFile(newFile);
                cout << "Файл добавлен!" << endl;
            }
            break;
        }

        case 6:
        {
            string fileName;
            cout << "Введите путь к текстовому файлу для загрузки данных: ";
            cin.ignore(); // Игнорируем символ новой строки
            getline(cin, fileName);

            // Загружаем файлы из указанного текстового файла
            manager.addFilesFromTextFile(fileName);
            cout << "Файлы успешно загружены из " << fileName << endl;
            break;
        }


        case 7:
            return false; // Прерываем выполнение программы

        default:
            cout << "Неверный выбор, попробуйте снова." << endl;
            break;
        }
    }

    return 0;
}




/*


    // Установка кодировки консоли для корректного вывода текста на русском языке
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


*/