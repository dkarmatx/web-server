#pragma once

#include <utility>

namespace __impl {

template <typename _UnderIterator>
class EnumerateImpl {
 private:
    using EnumeratePairRef = std::pair<size_t, typename _UnderIterator::reference>;

 public:
    class iterator {
     public:
        explicit iterator(const _UnderIterator& iter, size_t index = 0)
            : __it(iter), __index(index) {
        }

        explicit iterator(const iterator& iter)
            : __it(iter.__it), __index(iter.__index) {    
        }
    
        inline auto operator++() -> iterator& {
            ++this->__it;
            ++this->__index;
            return *this;
        }

        inline auto operator++(int) -> iterator {
            iterator old = *this;
            ++this->__it;
            ++this->__index;
            return old;
        }

        auto operator==(const iterator& other) const -> bool {
            return this->__it == other.__it;
        }

        auto operator!=(const iterator& other) const -> bool {
            return this->__it != other.__it;
        }

        auto operator*() const -> EnumeratePairRef {
            return EnumeratePairRef{__index, *(this->__it)};
        }

     private:
        _UnderIterator  __it;
        size_t          __index;
    };

    auto begin() const -> iterator {
        return iterator(__beg, 0);
    }

    auto end() const -> iterator {
        return iterator(__end, -1);
    }

 public:
    explicit EnumerateImpl(const _UnderIterator& b, const _UnderIterator& e)
        : __beg(b), __end(e) {
    }

    explicit EnumerateImpl(_UnderIterator&& b, _UnderIterator&& e)
        : __beg(b), __end(e) {
    }

 private:
    const _UnderIterator __beg;
    const _UnderIterator __end;
};

}  // namespace __impl

template <typename _Iterable>
auto enumerate(_Iterable&& iterable) {
    return __impl::EnumerateImpl( std::move(iterable.begin()) , std::move(iterable.end()) );
}

template <typename _Iterable>
auto enumerate(const _Iterable& iterable) {
    return __impl::EnumerateImpl( std::move(iterable.begin()) , std::move(iterable.end()) );
}
