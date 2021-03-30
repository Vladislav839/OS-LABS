Чтобы запустить main.cpp на исполнение и увидеть результат необходимо ввести cледующую команду из папки с файлами.
```shell
g++ main.cpp -lvector -L. -o RUN; mydir=$(pwd); cp libvector.so ~/bin; cd ~; cd ~bin; export LD_LIBRARY_PATH=.; cd $mydir; ./RUN
```
Если нужно собрать библиотеки из исходников можно удалить все файлы кроме .cpp и .h и затем выполнить cледующую команду.
```shell
g++ -c -fPIC NumberLib.cpp; ar rvs libnumber.a NumberLib.o; g++ -c -fPIC VectorLib.cpp; g++ VectorLib.o -shared -o libvector.so -Wl,--whole-archive libnumber.a -Wl,--no-whole-archive
```
После этого запустить main.cpp на исполнение первой командой.
