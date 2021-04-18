#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <thread>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <exception>

template<class T>
class BufferedChannel {
private:
    int size;
    std::queue<T> buffer_queue;
    std::mutex mt;
    std::condition_variable wait_for_send;
    std::condition_variable wai_for_receive;
    bool is_closed = false;

public:
    explicit BufferedChannel(int size) : size(size) {}

    void Send(T value) {
        // если канал не закрыт можем заниматься отправкой данных
        if (!is_closed) {
            std::unique_lock<std::mutex> lock(mt);
            /* если канал заполнен ждем пока его закроют (и тогда в него не будет поступать данные), 
            либо в нем появится свободное место */
            if (buffer_queue.size() != size) {
                wait_for_send.wait(lock, [this] { return buffer_queue.size() != size || is_closed; });
                // если канал закрыли, бросим исключение ожидающим потокам
                if (is_closed) {
                    throw(std::runtime_error("Can't send data to closed channel"));
                }
            }
            // так как канал не полон, то в него можно добавлять данные
            buffer_queue.push(value);
            /* анлочим мьютекс и оповещаем потоки, 
            которые ждут возможнсть прочитать из канала данные.
            Так как очередь у нас одна, 
            необходимо синхронизировать операции чтения и записи в канал*/
            lock.unlock();
            wai_for_receive.notify_one();
        }
        else {
            throw(std::runtime_error("Can't send data to closed channel"));
        }
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mt);
        if (buffer_queue.empty())
        {
            // Если канал закрыт и очередь пуста возвращаем false и 
            // дефолтное значение T
            if (is_closed)
                return std::make_pair(T(), false);

            /* так как очередь пуста ждем пока в нее что-нибудь добавят,
            либо закроют, и тогда можно будет в ответ полылать дефолтное значение T и false*/
            wai_for_receive.wait(lock, [this]() {return !buffer_queue.empty() || is_closed; });
            /* если мы закрыли поток и кол-во ожидающих потоков было больше чем
            элементов в очереди, то возвращаем дефолтное значение и false, если очередь пуста*/
            if (buffer_queue.empty() && is_closed)
                return std::make_pair(T(), false);
        }
        // т. к. очередь не пуста, то достаем из нее элементы
        T value = buffer_queue.front();
        buffer_queue.pop();
        /* анлочим мьютекс и оповещаем потоки,
           которые ждут возможнсть записать данные в канал.
           Так как очередь у нас одна,
           необходимо синхронизировать операции чтения и записи в канал*/
        lock.unlock();
        wait_for_send.notify_one();
        // возвращаем результат
        return std::make_pair(value, true);
    }

    void Close() {
        // атомарно меняем значение переменной is_closed на противоположное
        std::unique_lock<std::mutex> lock(mt);
        is_closed = true;
        lock.unlock();
        /* оповещаем ждущие потоки о том, что значение переменной
         is_closed изменилось и нужно перепроверить условия в методе wait*/ 
        wait_for_send.notify_all();
        wai_for_receive.notify_all();
    }

    int Size() {
        std::lock_guard<std::mutex> lock(mt);
        return buffer_queue.size(); 
    }
};

#endif
