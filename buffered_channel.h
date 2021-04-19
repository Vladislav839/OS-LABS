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
        // ���� ����� �� ������ ����� ���������� ��������� ������
        if (!is_closed) {
            std::unique_lock<std::mutex> lock(mt);
            /* ���� ����� �������� ���� ���� ��� ������� (� ����� � ���� �� ����� ��������� ������),
            ���� � ��� �������� ��������� ����� */
            if (buffer_queue.size() != size) {
                wait_for_send.wait(lock, [this] { return buffer_queue.size() != size || is_closed; });
                // ���� ����� �������, ������ ���������� ��������� �������
                if (is_closed) {
                    throw(std::runtime_error("Can't send data to closed channel"));
                }
            }
            // ��� ��� ����� �� �����, �� � ���� ����� ��������� ������
            buffer_queue.push(value);
            /* ������� ������� � ��������� ������,
            ������� ���� ���������� ��������� �� ������ ������.
            ��� ��� ������� � ��� ����,
            ���������� ���������������� �������� ������ � ������ � �����*/
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
            // ���� ����� ������ � ������� ����� ���������� false � 
            // ��������� �������� T
            if (is_closed)
                return std::make_pair(T(), false);

            /* ��� ��� ������� ����� ���� ���� � ��� ���-������ �������,
            ���� �������, � ����� ����� ����� � ����� �������� ��������� �������� T � false*/
            wai_for_receive.wait(lock, [this]() {return !buffer_queue.empty() || is_closed; });
            /* ���� �� ������� ����� � ���-�� ��������� ������� ���� ������ ���
            ��������� � �������, �� ���������� ��������� �������� � false, ���� ������� �����*/
            if (buffer_queue.empty() && is_closed)
                return std::make_pair(T(), false);
        }
        // �. �. ������� �� �����, �� ������� �� ��� ��������
        T value = buffer_queue.front();
        buffer_queue.pop();
        /* ������� ������� � ��������� ������,
           ������� ���� ���������� �������� ������ � �����.
           ��� ��� ������� � ��� ����,
           ���������� ���������������� �������� ������ � ������ � �����*/
        lock.unlock();
        wait_for_send.notify_one();
        // ���������� ���������
        return std::make_pair(value, true);
    }

    void Close() {
        // �������� ������ �������� ���������� is_closed �� ���������������
        std::unique_lock<std::mutex> lock(mt);
        is_closed = true;
        lock.unlock();
        /* ��������� ������ ������ � ���, ��� �������� ����������
         is_closed ���������� � ����� ������������� ������� � ������ wait*/
        wait_for_send.notify_all();
        wai_for_receive.notify_all();
    }

    int Size() {
        std::lock_guard<std::mutex> lock(mt);
        return buffer_queue.size();
    }
};

#endif