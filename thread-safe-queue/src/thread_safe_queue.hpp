#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue
{
    std::queue<T> q_;
    mutable std::mutex q_mtx_;
    std::condition_variable cv_not_empty_;

public:
    bool empty() const
    {
        std::lock_guard<std::mutex> lk{q_mtx_};
        return q_.empty();
    }

    void push(const T& item)
    {
        {
            std::lock_guard<std::mutex> lk{q_mtx_};
            q_.push(item);
        }

        cv_not_empty_.notify_one();
    }

    void push(T&& item)
    {
        {
            std::lock_guard<std::mutex> lk{q_mtx_};
            q_.push(std::move(item));
        }

        cv_not_empty_.notify_one();
    }

    void push(std::initializer_list<T> il)
    {
        {
            std::lock_guard<std::mutex> lk{q_mtx_};
            for (const auto& item : il)
                q_.push(item);
        }

        cv_not_empty_.notify_all();
    }

    bool try_push(const T& item)
    {
        {
            std::unique_lock<std::mutex> lk(q_mtx_, std::try_to_lock);
            if (!lk)
                return false;

            q_.push(item);
        }

        cv_not_empty_.notify_one();

        return true;
    }

    bool try_pop(T& item)
    {
        std::unique_lock<std::mutex> lk{q_mtx_, std::try_to_lock};
        if (!lk || q_.empty())
            return false;

        item = std::move(q_.front());
        q_.pop();
        return true;
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> lk{q_mtx_};

        cv_not_empty_.wait(lk, [&] { return !q_.empty(); });

        item = std::move(q_.front());
        q_.pop();
    }
};

#endif // THREAD_SAFE_QUEUE_HPP
