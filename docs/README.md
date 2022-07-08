В этом разделе вы можете посмотреть задание на проект и чек лист для его проверки

<details>
	<summary> subject от 7.7.2022 </summary>

# Webserv #

Это когда вы, наконец, понимаете, почему URL-адрес начинается с HTTP.

	Резюме:
	Этот проект посвящен написанию собственного HTTP-сервера. Вы сможете проверить это с помощью реального браузера. HTTP — один из самых используемых протоколов в Интернете. Знание его секретов будет полезно, даже если вы не будете работать над веб-сайтом.

	Версия: 19

# Содержание #

1. [Введение](#Введение)
2. [Общие инструкции](#Общие-инструкции)
3. [Обязательная часть](#Обязательная-часть)
4. [Бонусная часть](#Бонусная-часть)
5. [Представление и защита](#Представление-и-защита)

# Введение #

`Hypertext Transfer Protocol` (HTTP) — это прикладной протокол для распределенных, совместных, информационных гипермедиа систем.

`HTTP` является основой передачи данных для `World Wide Web (WWW)`, где гипертекстовые документы содержат гиперссылки на другие ресурсы, к которым пользователь может легко получить доступ. Например, щелкнув мышью или коснувшись экрана в веб-браузере.

`HTTP` был разработан для облегчения работы с гипертекстом и Всемирной паутиной.

Основная функция веб-сервера — хранить, обрабатывать и доставлять веб-страницы клиентам. Связь между клиентом и сервером происходит с использованием протокола передачи гипертекста `(HTTP)`.

Доставляемые страницы чаще всего представляют собой HTML-документы, которые могут включать в себя изображения, таблицы стилей и скрипты в дополнение к текстовому содержимому.

Для веб-сайта с высокой посещаемостью можно использовать несколько веб-серверов.

Пользовательский агент, обычно веб-браузер или веб-сканер, инициирует связь, запрашивая определенный ресурс с использованием `HTTP`, и сервер отвечает содержимым этого ресурса или сообщением об ошибке, если не может этого сделать. Ресурс обычно является реальным файлом во вторичном хранилище сервера, но это не обязательно так и зависит от того, как реализован веб-сервер.

Хотя основной функцией является предоставление контента, полная реализация HTTP также включает способы получения контента от клиентов. Эта функция используется для отправки веб-форм, включая загрузку файлов.

# Общие инструкции #

* Ваша программа не должна давать сбоев ни при каких обстоятельствах (даже если ей не хватает памяти) и не должна неожиданно завершать работу. Если это произойдет, ваш проект будет считаться нефункциональным, а ваша оценка будет равна 0.
* Вы должны включить Makefile, который скомпилирует ваши исходные файлы. Он не должен повторно связываться.
* Ваш Makefile должен как минимум содержать правила:
	> `$(NAME), all, clean, fclean and re`
* Compile your code with c++ and the flags -Wall -Wextra -Werror
* Your code must comply with the C++ 98 standard. Then, it should still compile
if you add the flag -std=c++98.
* Старайтесь всегда разрабатывать с использованием как можно большего количества функций C++ (например, выбирайте <cstring> вместо <string.h>). Вам разрешено использовать функции C, но всегда предпочитайте их версии C++, если это возможно.
* Любая внешняя библиотека и библиотеки Boost запрещены

# Обязательная часть #


Название программы  | webserv
:------------------:|:---------------
Файлы для сдачи     | Makefile, *.{h, hpp}, *.cpp, *.tpp, *.ipp, configuration files
Makefile            | NAME, all, clean, fclean, re
Параметры			| [A configuration file]
Разрешенные внешние функции     |Everything in C++ 98. htons, htonl, ntohs, ntohl, select, poll, epoll (epoll_create, epoll_ctl, epoll_wait), kqueue (kqueue, kevent), socket, accept, listen, send, recv, bind, connect, inet_addr, setsockopt, getsockname, fcntl
Использование libft | n/a
Описание            | A HTTP server in C++ 98

You must write a HTTP server in C++ 98.

Your executable will be run as follows:
	 ./webserv [configuration file]

> Даже если в теме и шкале оценки упоминается poll(), вы можете использовать любой эквивалент, такой как select(), kqueue(), or epoll().

> Пожалуйста, прочтите RFC и выполните несколько тестов с telnet и NGINX, прежде чем начинать этот проект. Даже если вам не нужно реализовывать все RFC, его чтение поможет вам разработать необходимые функции.

## Требования ##

* Ваша программа должна принимать файл конфигурации в качестве аргумента или использовать путь по умолчанию.
* Вы не можете `execve` другой веб-сервер
* Ваш сервер никогда не должен блокироваться, а клиент может быть правильно возвращен, если это необходимо.
* Он должен быть неблокирующим и использовать только `1 poll()` (или эквивалент) для всех операций ввода-вывода между клиентом и сервером (включая прослушивание).
* `poll()` (или аналогичный) должен проверять чтение и запись одновременно.
* Вы никогда не должны выполнять операцию чтения или записи, не пройдя через `poll()` (или аналогичный).
* Проверка значения `errno` строго запрещена после операции чтения или записи.
* Вам не нужно использовать `poll()` (или эквивалент) перед чтением файла конфигурации.
> Поскольку вам приходится использовать неблокирующие файловые дескрипторы, можно использовать функции чтения/получения или записи/отправки без `poll()` (или эквивалента), и ваш сервер не будет блокироваться. Но это будет потреблять больше системных ресурсов. Таким образом, если вы попытаетесь прочитать/получить или записать/отправить любой файловый дескриптор без использования poll() (или аналогичного), ваша оценка будет равна 0.
* Вы можете использовать любой макрос и определять как `FD_SET, FD_CLR, FD_ISSET, FD_ZERO` (понимание того, что и как они делают, очень полезно).
* Запрос к вашему серверу никогда не должен зависать навсегда.
* Ваш сервер должен быть совместим с `веб-браузером` по вашему выбору.
* Мы будем считать, что `NGINX` совместим с `HTTP 1.1` и может использоваться для сравнения заголовков и поведения ответов.
* Ваши коды состояния HTTP-ответа должны быть точными.
* На вашем сервере должны быть `default error pages`, если они не предоставлены.
* Вы не можете использовать fork для чего-то другого, кроме CGI (например, PHP или Python и т. д.).
* Вы должны быть в состоянии обслуживать полностью статичный веб-сайт.
* У клиентов должна быть возможность загружать файлы.
* Вам нужны как минимум методы `GET, POST и DELETE`.
* Стресс-тесты вашего сервера. Он должен быть доступен любой ценой.
* Ваш сервер должен иметь возможность прослушивать несколько портов (см. Файл конфигурации).

## For MacOS only ##

> Поскольку MacOS не реализует write() так же, как другие ОС Unix, вам разрешено использовать fcntl().
Вы должны использовать файловые дескрипторы в неблокирующем режиме, чтобы получить поведение, аналогичное другим ОС Unix.

> Однако вам разрешено использовать fcntl() только следующим образом:
fcntl(fd, F_SETFL, O_NONBLOCK);
Любые другие флаги запрещены.

## Configuration file ##

> Вы можете получить некоторое вдохновение из «серверной» части файла конфигурации NGINX.

В файле конфигурации вы должны иметь возможность:
* Выберите порт и хост каждого `server`.
* Настраивать `server_names` или нет.
* Первый сервер для `host:port` будет использоваться по умолчанию для этого `host:port` (это означает, что он будет отвечать на все запросы, которые не принадлежат другому серверу).
* Настройте страницы ошибок по умолчанию.
* Ограничьте `body size` клиента.
* Настройте маршруты с одним или несколькими из следующих правил/конфигураций (маршруты не будут использовать regex):
	* Определите список допустимых HTTP-методов для маршрута.
	* Определить перенаправление `HTTP`
	* Определите каталог или файл, в котором следует искать файл (например, если URL-адрес `/kapouet` связан с `/tmp/www`, URL-адрес `/kapouet/pouic/toto/pouet` имеет вид `/tmp/www/pouic/toto/pouet`).
	* Включите или выключите список каталогов.
	* Выполнять CGI на основе определенного расширения файла (например, .php).
	* Сделайте так, чтобы маршрут мог принимать загруженные файлы, и настройте место их сохранения.
		* Вы задаетесь вопросом, что такое [CGI](https://en.wikipedia.org/wiki/Common_Gateway_Interface)?
		* Поскольку вы не будете вызывать `CGI` напрямую, используйте полный путь в качестве `PATH_INFO`.
		* Просто помните, что для фрагментированного запроса ваш сервер должен разблокировать его, и `CGI` будет ожидать `EOF` как конец `body`.
		* То же самое для вывода `CGI`. Если из CGI не возвращается `content_length`, `EOF` отмечает конец возвращенных данных.
		* Ваша программа должна вызывать `CGI` с запрошенным файлом в качестве первого аргумента.
		* `CGI` должен запускаться в правильном каталоге для доступа к файлам относительного пути.
		* Ваш сервер должен работать с одним `CGI` (php-CGI, Python и т. д.).

Вы должны предоставить некоторые файлы конфигурации и базовые файлы по умолчанию для тестирования и демонстрации работы каждой функции во время оценки.

> Если у вас есть вопрос об одном поведении, вам следует сравнить поведение вашей программы с поведением NGINX.
Например, проверьте, как работает server_name.
Мы поделились с вами небольшим тестером. Его не обязательно проходить, если с вашим браузером и тестами все работает нормально, но это может помочь вам найти некоторые ошибки.

> Главное — это устойчивость. Ваш сервер никогда не должен умереть.

> Не проводите тестирование только с одной программой. Пишите тесты на более удобном языке, таком как Python или Golang и т. д. Даже на C или C++, если хотите.

# Бонусная часть #

Вот дополнительные функции, которые вы можете добавить:
* Поддержка файлов `cookie` и `session_management` (подготовьте краткие примеры).
* Обработка нескольких `CGI`.

> Бонусная часть будет оцениваться только в том случае, если обязательная часть будет «ОТЛИЧНО». Идеальный означает, что обязательная часть была полностью выполнена и работает без сбоев. Если вы не выполнили ВСЕ обязательные требования, ваша бонусная часть вообще не будет оцениваться.

# Представление и защита #

Сдайте задание в своем репозитории Git, как обычно. Во время защиты будет оцениваться только работа внутри вашего репозитория. Не стесняйтесь перепроверять имена ваших файлов, чтобы убедиться, что они верны.

</details>

<details>
	<summary> checklist от ??.??.???? </summary>

Нужно найти чек-лист

</details>

<details>
	<summary> Тесты из intra42 </summary>

test for MacOS:
* [tester](https://projects.intra.42.fr/uploads/document/document/9242/tester)
* [cgi_tester](https://projects.intra.42.fr/uploads/document/document/9244/cgi_tester)

test for Ubuntu:
* [ubuntu_tester](https://projects.intra.42.fr/uploads/document/document/9245/ubuntu_tester)
* [ubuntu_cgi_tester](https://projects.intra.42.fr/uploads/document/document/9243/ubuntu_cgi_tester)

Usage tester:

	usage: ./ubuntu_tester scheme://host:port
	scheme://host:port: example http://localhost:80

</details>

<details>
	<summary> Интересные ссылки по теме </summary>


</details>