# Инструкция пользователя.

В папке Debug лежат уже собраные exe файлы, так что можно просто их открыть, если операционная система  / антивирус разрешит (вряд ли), иначе можно открыть проект в вижле и пересобрать.
Если нужно пересобрать решение / поменять переменную окружения, нужно открыть проект в VS и собрать сначала Killer, а потом User, обе под х86. Затем запустить User на исполнение, назначив его автзапускаемым проектом через интерфейс вижлы.
Именя процессов вводить без .exe, просто имя без расширения.

Вызова командной строки здесь не играют особой роли, они тут чисто для проверки присутсвия процесса до удаления и его отсутсвия после удаления.
