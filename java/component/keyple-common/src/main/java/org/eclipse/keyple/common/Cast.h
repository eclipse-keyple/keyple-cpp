#pragma once

class Cast {
public:
    template <typename To, typename From>
    static inline std::shared_ptr<To> reinterpret_pointer_cast(std::shared_ptr<From> const & ptr) noexcept
    {
        return std::shared_ptr<To>(ptr, reinterpret_cast<To *>(ptr.get()));
    }
};
