#pragma once

#include <cstring>
#include <string>
#include <cassert>

#include <memory>

enum ErrorExpectedEnum {
    UNEXPECTED,
};

enum SuccessExpectedEnum {
    SUCCESS,
};

template <class _Result, class _Err>
class Expected {
 private:
    enum class State : std::uint8_t {
        OK = 0,
        ERR = 1,
    };

    union Share {
        _Result r;
        _Err    e;

        inline Share() {}
        inline ~Share() {}
    
        explicit inline Share(const _Result& res) {
            new (this) _Result(res);
        }

        explicit inline Share(_Result&& res) {
            new (this) _Result(std::move(res));
        }

        explicit inline Share(int, const _Err& err) {
            new (this) _Err(err);
        }

        explicit inline Share(int, _Err&& err) {
            new (this) _Err(std::move(err));
        }
    };

    State   __state;
    Share   __shared;

 public:
    Expected() = delete;

    inline Expected(const _Result& res)
        : __state(State::OK)
        , __shared(res) {
    }

    inline Expected(_Result&& res)
        : __state(State::OK)
        , __shared(res) {
    }

    inline Expected(ErrorExpectedEnum, const _Err& err)
        : __state(State::ERR)
        , __shared(0, err) {
    }

    inline Expected(ErrorExpectedEnum, _Err&& err)
        : __state(State::ERR)
        , __shared(0, std::move(err)) {
    }

    inline Expected(SuccessExpectedEnum, const _Result& res)
        : __state(State::OK)
        , __shared(res) {
    }

    inline Expected(SuccessExpectedEnum, _Result&& res)
        : __state(State::OK)
        , __shared(std::move(res)) {
    }

    inline Expected(const Expected& exp)
        : __state(exp.__state) {
        if (exp.isOk()) {
            new (&this->__shared) _Result(exp.__shared.r);
        } else {
            new (&this->__shared) _Result(0, exp.__shared.e);
        }
    }

    inline Expected(Expected&& exp)
        : __state(exp.__state) {
        if (exp.isOk()) {
            new (&this->__shared) _Result(std::move(exp.__shared.r));
        } else {
            new (&this->__shared) _Result(0, std::move(exp.__shared.e));
        }
    }

    inline Expected& operator=(const Expected& exp) {
        this->~Expected();
        new (this) Expected(exp);
        return *this;
    }

    inline Expected& operator=(Expected&& exp) {
        this->~Expected();
        new (this) Expected(std::move(exp));
        return *this;
    }

    inline ~Expected() {
        if (this->isError()) {
            this->__shared.e.~_Err();
        } else {
            this->__shared.r.~_Result();
        }
    }
    inline _Result& operator*() {
        return this->value();
    }

    inline const _Result& operator*() const {
        return this->value();
    }

    inline _Result& value() {
        assert(this->isOk());
        return this->__shared.r;
    }

    inline const _Result& value() const {
        assert(this->isOk());
        return this->__shared.r;
    }

    inline const _Err& error() const {
        assert(this->isError());
        return this->__shared.e;
    }

    inline operator bool() const {
        return this->isOk();
    }

    inline bool isError() const {
        return this->__state == State::ERR;
    }

    inline bool isOk() const {
        return this->__state == State::OK;
    }
};
