#ifndef _PACKET_QUEUE_H_
#define _PACKET_QUEUE_H_

#include <deque>
#include <mutex>

namespace net
{
    template<typename T>
    class PacketQueue
    {
    public:
        PacketQueue() = default;
        PacketQueue(const PacketQueue<T>&) = delete;
        virtual ~PacketQueue() { clear(); }

        const T& front()
        {
            std::scoped_lock lock(mutex_queue_);
            return deque_.front();
        }

        const T& back()
        {
            std::scoped_lock lock(mutex_queue_);
            return deque_.back();
        }

        T pop_front()
        {
            std::scoped_lock lock(mutex_queue_);
            auto t = std::move(deque_.front());
            deque_.pop_front();
            return t;
        }

        T pop_back()
        {
            std::scoped_lock lock(mutex_queue_);
            auto t = std::move(deque_.back());
            deque_.pop_back();
            return t;
        }

        void push_back(const T& item)
        {
            std::scoped_lock lock(mutex_queue_);
            deque_.emplace_back(std::move(item));

            std::unique_lock<std::mutex> lock_(mutex_block_);
            block_.notify_one();
        }

        void push_front(const T& item)
        {
            std::scoped_lock lock(mutex_queue_);
            deque_.emplace_front(std::move(item));

            std::unique_lock<std::mutex> lock_(mutex_block_);
            block_.notify_one();
        }

        bool empty()
        {
            std::scoped_lock lock(mutex_queue_);
            return deque_.empty();
        }

        size_t count()
        {
            std::scoped_lock lock(mutex_queue_);
            return deque_.size();
        }

        void clear()
        {
            std::scoped_lock lock(mutex_queue_);
            deque_.clear();
        }

        void wait()
        {
            while (empty())
            {
                std::unique_lock<std::mutex> lock(mutex_block_);
                block_.wait(lock);
            }
        }

    protected:
        std::mutex mutex_queue_;
        std::deque<T> deque_;
        std::condition_variable block_;
        std::mutex mutex_block_;
    };

} // namespace net

#endif // ! _PACKET_QUEUE_H_
