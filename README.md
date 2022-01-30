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
- Coockie:;

## HOW TO: 
### Для запуска сервера:
ubuntu\linux\MAC => make -> ./webserver в агрументы (argv[1])можно передать свои настройки, анотация выше в разделе Файл конфигурации. По дефолту работает конфигурация src/Config/file/default.conf.

### Автоиндексация:
Google Chrome/Microsoft Edge/Mozila 4.0 и старше/Opera -> http://localhost:8080/dow/ с дефолтной конфигурацией прослушивается папка src/www/downloads/.

### Загрузка на сервер:  
Google Chrome/Microsoft Edge/Mozila 4.0 и старше/Opera -> http://localhost:8080/ -> нажать кнопку **"Выберите файл"** в верхней части сайта. Загружается в srcs/www/downloads/, сайт http://localhost:8080/dow/ .

### Скачивание с сервера:
Google Chrome/Microsoft Edge/Mozila 4.0 и старше/Opera -> http://localhost:8080/dow/. Файлы не относящиеся к типу **mp4 | jpg | png | jpeg | gif | html** начинают загрузку, остальные воспроизводятся в браузере с возможностью сохранения.

## localhost:8080/
![Image alt](https://github.com/Katczinski/webserv/blob/master/srcs/www/downloads/auto/location_root.png)

## localhost:8080/dow/
![Image alt](https://github.com/Katczinski/webserv/blob/master/srcs/www/downloads/auto/location_dow.png)
____
