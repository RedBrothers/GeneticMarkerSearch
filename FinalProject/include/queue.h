#ifndef GENES_QUEUE_H
#define GENES_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

/*
 * Thread-Safe Deque implementation
 */
template<typename T>
class Deque {
public:
    void push_front(const T&);
    void push_front(T&&);
    void push_back(const T&);
    void push_back(T&&);
    T pop_front();
    T pop_back();
private:
    std::deque<T> queue_;
    std::mutex m_;
    std::condition_variable cond_;
};


template<typename T>
void Deque<T>::push_front(const T& el) {
    std::unique_lock lock{ m_ };
    queue_.push_front(el);
    lock.unlock();
    cond_.notify_one();
}

template<typename T>
void Deque<T>::push_front(T&& el) {
    std::unique_lock lock{ m_ };
    queue_.push_front(std::move(el));
    lock.unlock();
    cond_.notify_one();
}

template<typename T>
void Deque<T>::push_back(const T& el) {
    std::unique_lock lock{ m_ };
    queue_.push_back(el);
    lock.unlock();
    cond_.notify_one();
}

template<typename T>
void Deque<T>::push_back(T&& el) {
    std::unique_lock lock{ m_ };
    queue_.push_back(std::move(el));
    lock.unlock();
    cond_.notify_one();
}

template<typename T>
T Deque<T>::pop_front() {
    std::unique_lock lock{ m_ };
    cond_.wait(lock, [this](){ return queue_.size() > 0; });
    auto el = queue_.front();
    queue_.pop_front();
    lock.unlock();
    return el;
}

template<typename T>
T Deque<T>::pop_back() {
    std::unique_lock lock{ m_ };
    cond_.wait(lock, [this](){ return queue_.size() > 0; });
    auto el = queue_.back();
    queue_.pop_back();
    lock.unlock();
    return el;
}


#endif //GENES_QUEUE_H
