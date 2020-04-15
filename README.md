# RPC Daemon и RPC клиент.
Реализуйте многопоточный RPC-демон и библиотеку RPC-клиента поверх системных вызовов.

## Реализация.
RPC Демон принимает подключения клиентов и начинает работать с ними в отдельном потоке. Для передачи запросов будет использоваться самописный протокол передачи данных, который будет подразумевать вызов либо программы, либо системного вызова по его номеру, количеству аргументов и самих аргументов.
Только главный поток будет обрабатывать сигналы, завершая все потоки при их получении.

Пока что попытаемся реализовать минимальный функционал: RPC Клиент будет на стандартном потоке принимать строчку вида `(SYSCALL | EXECUTE | EXIT) <args>`, где `<args>`, в случае обычной программы выглядит как `./path/to/file <args...>`, а в случае системного вызова как `<syscall num> <args...>`, причем все аргументы в этом случае - просто числа типа `int64_t`.

Возможные дополнения:
* Реализация более простого интерфейса для системных вызовов - получать название системного вызова и затем парсить таблицу символов `glibc`, чтобы найти нужный системный вызов.
* Сделать передачу информации по настоящему протоколу RPC, установленному [RFC 1050](https://tools.ietf.org/html/rfc1050)
