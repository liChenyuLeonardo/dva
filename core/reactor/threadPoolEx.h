#ifndef _THREAD_POOL_EX_INCLUDED_
#define _THREAD_POOL_EX_INCLUDED_
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

//超简陋线程池
class threadPool {
 public:
  explicit threadPool(size_t thread_count)
      : data_(std::make_shared<data>()) {
    for (size_t i = 0; i < thread_count; ++i) {
      std::thread([data = data_] {
        std::unique_lock<std::mutex> lk(data->mtx_);
        for (;;) {
          if (!data->tasks_.empty()) {
            auto current = std::move(data->tasks_.front());
            data->tasks_.pop();
            lk.unlock();
            current();
            lk.lock();
          } else if (data->is_shutdown_) {
            break;
          } else {
            data->cond_.wait(lk);
          }
        }
      }).detach();
    }
  }

  threadPool() = default;
  threadPool(threadPool&&) = default;

  ~threadPool() {
    if ((bool) data_) {
      {
        std::lock_guard<std::mutex> lk(data_->mtx_);
        data_->is_shutdown_ = true;
      }
      data_->cond_.notify_all();
    }
  }

  template <typename F, typename... Args>
  void execute(F&& task, Args&&... args) {
    {
      std::lock_guard<std::mutex> lk(data_->mtx_);
      data_->tasks_.emplace(std::bind(std::forward<F>(task), std::forward<Args>(args)...));
    }
    data_->cond_.notify_one();
  }

 private:
  struct data {
    std::mutex mtx_;
    std::condition_variable cond_;
    bool is_shutdown_ = false;
    std::queue<std::function<void()>> tasks_;
  };
  std::shared_ptr<data> data_;
};

#endif