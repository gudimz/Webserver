____

# HTTP запросы
 
Версия HTTP 1.0, 1.1.

### Поддерживаемые коды ответа:
200, 206, 301, 400, 403, 404, 405, 408, 413, 500.
 
### Методы поддерживаемые сервером:
- GET стандарта RFC 2616; 
- POST стандарта RFC 2616;
- DELETE стандарта RFC 2616;

### Поддерживаемые сервером хэдеры (RFC 72**):
- Accept-range: bytes;
- Connection: close/Keep-Alive;
- Location: ; для переадресации по 30* запросу перемещения
- Transfer-Encoding: chunked;
- Content-type: multipart/form-data;
- Cookie:;

## HOW TO:
### Файл конфигурации
Файл с дефолтной конфигурацией вебсервера находится `src/Config/file/default.conf`.

**Блок server:**

В случае отсутствия блока `server`, файл конфигурации считается невалидным. В одном конфигурационном файле можно указать несколько блоков `server`. В результате будут параллельно запущены несколько серверов, к которым можно обратиться, введя в строке браузера `host`:`listen`.
- `listen` - перичисляем порты для подключения к серверу.
- `host` - указываем ip адрес сервера, либо localhost.
- `server_name` - указываем доменное имя сервера.
- `error_page` - по умолчанию все страницы находятся `/srcs/www/default_pages/`. Если необходимо добавить страницы, отличные от дефолтных, указываем номер ошибки и путь к новой странице с ошибкой.
- `root` - указываем корневой путь сервера.
- `index` - указываем название стартовой страницы сервера.
Внутри блока `server` находится блок `location`. Внутри блока `server` можно добавить несколько блоков `location`.

**Блок location:**

В location указывается префикс, который сравнивается с URI из запроса. Для подходящего запроса к префикус добавляется `root` (если он есть), находящийся внутри блока location.
- `root` - указываем корневой путь для location.
- `index` - указываем название страницы или исполняемого файла.
- `cgi_extension` - указываем расширение файла для cgi.
- `cgi_path` - указваем путь к интерпретатору cgi.
- `allow_method` - указываем разрешенные методы. Метод GET всегда разрешен.
- `max_body_size` - указываем максимальный размер запроса от клиента.
- `autoindex` - включить `on`, выключить `off`. Если этого поля нет, то автоиндекс выключен.

**Для cgi-скриптов нужен отдельный location, поля 'cgi_extension' и 'cgi_path' - обязательны.**

### Для запуска сервера:
ubuntu\linux\MAC => make -> ./webserver в агрументы (argv[1])можно передать свои настройки, анотация выше в разделе Файл конфигурации. По дефолту работает конфигурация src/Config/file/default.conf.

### Автоиндексация:
Google Chrome/Microsoft Edge/Mozila 4.0 и старше/Opera -> http://localhost:8080/dow/ с дефолтной конфигурацией прослушивается папка `src/www/downloads/`.

### Загрузка на сервер:  
Google Chrome/Microsoft Edge/Mozila 4.0 и старше/Opera -> http://localhost:8080/ -> нажать кнопку **"Выберите файл"** в верхней части сайта. Загружается в `srcs/www/downloads/`, сайт http://localhost:8080/dow/ .

### Скачивание с сервера:
Google Chrome/Microsoft Edge/Mozila 4.0 и старше/Opera -> http://localhost:8080/dow/. Файлы не относящиеся к типу **mp4 | jpg | png | jpeg | gif | html** начинают загрузку, остальные воспроизводятся в браузере с возможностью сохранения.

## localhost:8080/
![Image alt](https://github.com/Katczinski/webserv/blob/master/srcs/www/downloads/auto/location_root.png)

## localhost:8080/dow/
![Image alt](https://github.com/Katczinski/webserv/blob/master/srcs/www/downloads/auto/location_dow.png)
____
