/*
 * MIT License
 *
 * Copyright (c) 2020
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ATP_ANY_HPP__
#define __ATP_ANY_HPP__

#include <string>
#include <typeinfo>
#include <typeindex>
#include <exception>
#include <type_traits>

namespace atp {

namespace detail {

template <bool B, class T = void>
struct disable_if_c {
    typedef T type;
};

template <class T>
struct disable_if_c<true, T> {};

template <class Cond, class T = void>
struct disable_if : public disable_if_c<Cond::value, T> {};

template<bool, class _Ty1, class _Ty2>
struct _If {
    typedef _Ty2 type;
};

template<class _Ty1, class _Ty2>
struct _If<true, _Ty1, _Ty2> {
    typedef _Ty1 type;
};

template<class _Ty>
struct add_reference {
    typedef typename std::remove_reference<_Ty>::type& type;
};

template<>
struct add_reference<void> {
    typedef void type;
};

template<>
struct add_reference<const void> {
    typedef const void type;
};

template<>
struct add_reference<volatile void> {
    typedef volatile void type;
};

template<>
struct add_reference<const volatile void> {
    typedef const volatile void type;
};

class bad_cast {
public:
    explicit bad_cast( const std::string& msg) {
        _what = msg;
    }

    const char* what() const {
        return(_what.data());
    }

private:
    std::string _what;
};

} /* end namespace detail */

class any {
public:
    any()
        : content(0) {

    }

    template<typename _Ty>
    any(const _Ty& value)
        : content(new holder<typename std::remove_cv<typename std::decay<const _Ty>::type>::type>(value)) {

    }

    any(const any& other)
        : content(other.content ? other.content->clone() : 0) {

    }

    any(any&& other)
        : content( other.content ) {
        other.content = 0;
    }

    template<typename _Ty>
    any(_Ty&& value
        , typename detail::disable_if< typename std::is_same<any&, _Ty> >::type* = 0

        , typename detail::disable_if< typename std::is_const<_Ty> >::type* = 0 )

        : content(new holder<typename std::decay<_Ty>::type>(static_cast<_Ty&&>(value))) {

    }

    ~any() {
        if (content) {
            delete content;
        }

        content = nullptr;
    }

public:
    any& swap(any& rhs) {
        std::swap(content, rhs.content);

        return(*this);
    }

    any& operator=(const any& rhs) {
        any(rhs).swap(*this);

        return(*this);
    }

    any& operator=(any&& rhs) {
        rhs.swap(*this);

        any().swap(rhs);

        return(*this);
    }

    template<class valueType>
    any& operator=(valueType&& rhs) {
        any(static_cast<valueType&&>(rhs)).swap(*this);

        return(*this);
    }

public:
    bool empty() {
        return(!content);
    }

    void clear() {
        any().swap(*this);
    }

    const std::type_info& type() const {
        return(content ? content->type() : typeid(void));
    }

private:
    class placeholder {
    public:
        virtual ~placeholder() {

        }

        virtual const std::type_info& type() const = 0;

        virtual placeholder* clone() const = 0;
    };

    template<typename _Ty>
    class holder : public placeholder {
    public:
        typedef _Ty value_type;

        holder(const _Ty& value) : held(value) {

        }

        holder(_Ty&& value)
            : held(static_cast<_Ty&&>(value)) {

        }

        virtual const std::type_info& type() const {
            return(typeid(value_type));
        }

        virtual placeholder* clone() const {
            return(new holder(held));
        }

    public:
        _Ty held;

    private:
        holder& operator=( const holder&);
    };

private:
    template<typename _Ty>
    friend _Ty* any_cast(any*);

    template<typename _Ty>
    friend _Ty* unsafe_any_cast(any*);

private:
    placeholder* content;
};


inline void swap(any& lhs, any& rhs) {
    lhs.swap(rhs);
}

template<typename _Ty>
_Ty* any_cast(any* operand) {
    return(operand&& operand->type() == typeid(_Ty) ?

        &static_cast<any::holder< typename std::remove_cv<_Ty>::type>*>(operand->content)->held : 0);
}

template<typename _Ty>
inline const _Ty* any_cast(const any* operand) {
    return(any_cast<_Ty>(const_cast<any*>(operand)));
}

template<typename _Ty>
_Ty any_cast(any& operand) {
    typedef typename std::remove_reference<_Ty>::type nonref;

    nonref* result = any_cast<nonref>(&operand);

    if (!result) {
        std::string reason = "bad any_cast : can't convert ";

        reason += operand.type().name();

        reason += " to ";

        reason += typeid(_Ty).name();

        throw detail::bad_cast(reason);
    }

    typedef typename detail::_If<

        std::is_reference<_Ty>::value,

        _Ty,

        typename detail::add_reference<_Ty>::type

        >::type ref_type;

    return(static_cast<ref_type>(*result));
}

template<typename _Ty>
inline _Ty any_cast(const any& operand) {
    typedef typename std::remove_reference<_Ty>::type nonref;

    return(any_cast<const nonref&>(const_cast<any&>(operand)));
}

template<typename _Ty>
inline _Ty any_cast(any&& operand) {
    static_assert(
        std::is_rvalue_reference<_Ty&&>::value

        || std::is_const< typename std::remove_reference<_Ty>::type >::value,

        "any_cast shall not be used for getting nonconst references to temporary objects"

        );

    return(any_cast<_Ty>(operand));
}

} /* end namespace atp */

#endif /* __ATP_ANY_HPP__ */
