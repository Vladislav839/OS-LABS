# OS-LABS

Можно запускать сразу object файл main.

Если нужно скомпилировать заново то называйте -о файлы как здесь.
Для этого можно использовать следующий скрипт:
```{r, engine='bash', count_lines}
g++ m.cpp -o m && g++ a.cpp -o a && g++ p.cpp -o p && g++ s.cpp -o s && g++ main.cpp -o main && ./main
```
