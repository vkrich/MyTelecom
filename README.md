# MyTelecom

# Постановка задачи

   Задание: разработать приложение–сервер системы торгов. На торги выставляются лоты, имеющие начальную стоимость. Участники торгов могут повышать стоимость лота. Распорядитель может прекратить торги. При окончании торгов всем участникам рассылаются результаты торгов.

Основные возможности. Серверное приложение должно реализовывать следующие функции:

>1) Прослушивание определенного порта
>2) Обработка запросов на подключение по этому порту от клиентов системы торгов (как распорядитель или участник торгов)
>3) Поддержка одновременной работы нескольких клиентов системы торгов через механизм нитей
>4) От участников торгов: прием запросов на передачу списка лотов
>5) От участников торгов: прием запросов на повышение стоимости лота
>6) От распорядителя: прием запроса на добавление нового лота с первоначальной стоимостью
>7) От распорядителя: прием запроса об окончании торгов
>8) Осуществление добавления лота, учет повышения стоимости лота участниками, завершение торгов и рассылка результатов торгов всем участникам
>9) Обработка запроса на отключение клиента
>10) Принудительное отключение клиента


# Алгоритм работы

