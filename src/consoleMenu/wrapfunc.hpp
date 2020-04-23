template <typename T>
class WrapFunc;

template <typename T, typename... Args>
class WrapFunc<T(Args...)>
{
    T (*f)
    (Args...);

public:
    WrapFunc(T (*t)(Args...))
    {
        f = t;
    }

    T operator()(Args &&... args)
    {
        if (f != nullptr)
        {
            return (*f)(std::forward<Args>(args)...);
        }
        else
        {
            return T();
        }
    }
};