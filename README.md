# OS-LABS

Можно запускать сразу object файл main.

Если нужно скомпилировать заново - используйте следующий скрипт:
```{r, engine='bash', count_lines}
g++ m.cpp -o m && g++ a.cpp -o a && g++ p.cpp -o p && g++ s.cpp -o s && g++ main.cpp -o main && ./main
```
