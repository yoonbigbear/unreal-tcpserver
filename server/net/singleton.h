#ifndef _SINGLETON_H_

#include <mutex>

namespace net {

    template<typename T>
    class Singleton
    {
    public:
        static T& instance()
        {
            std::call_once(once_flag_, []() {
                instance_ = std::make_shared<T>();
                });

            return *(instance_.get());
        }

    private:
        static std::once_flag once_flag_;
        static std::shared_ptr<T> instance_;
    };

    template<typename T>
    std::shared_ptr<T> Singleton<T>::instance_ = nullptr;
    template<typename T>
    std::once_flag Singleton<T>::once_flag_;

} // namespace net

#endif // !_SINGLETON_H_