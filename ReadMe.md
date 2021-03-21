Чтобы запустить main.cpp на исполнение и увидеть результат необходимо ввести cледующую команду из папки с файлами.
```shell
g++ main.cpp -lvector -L. -o RUN; mydir=$(pwd); cp libvector.so ~/bin; cd ~; cd ~bin; export LD_LIBRARY_PATH=.; cd $mydir; ./RUN
```
