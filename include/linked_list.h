#pragma once

#include <initializer_list>
#include <stdexcept>

template<typename T>
class linked_list
{
private:
    struct node
    {
        T data{};
        node* next;
    };

    node* mFirst;
    node* mLast;
    size_t mSize;

public:
    struct iterator
    {
    private:
        node* mCurrent;
        node* mNext;

        iterator() noexcept : mCurrent(nullptr), mNext(nullptr) {}
        iterator(node* cur, node* next) noexcept : mCurrent(cur), mNext(next) {}
        iterator(node* cur) noexcept : mCurrent(cur), mNext(cur != nullptr ? cur->next : nullptr) {}
    public:
        iterator& operator++()
        {
            if (mCurrent == nullptr && mNext == nullptr)
            {
                throw std::out_of_range(__FUNCTION__ ": can't increment end() iterator.");
            }

            mCurrent = mNext != nullptr ? mNext : mCurrent->next;
            mNext = mCurrent != nullptr ? mCurrent->next : nullptr;
            return *this;
        }

        iterator operator++(int)
        {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const noexcept
        {
            return mCurrent == other.mCurrent;
        }

        bool operator!=(const iterator& other) const noexcept
        {
            return !(*this == other);
        }

        const T& operator*() const
        {
            if (mCurrent == nullptr)
            {
                throw std::out_of_range(__FUNCTION__ ": can't dereference end() or before_begin() iterator.");
            }

            return mCurrent->data;
        }

        friend class linked_list;
    };

    linked_list() : mSize(0), mFirst(nullptr), mLast(nullptr) {}
    linked_list(const std::initializer_list<T>& elems) : mSize(0), mFirst(nullptr), mLast(nullptr)
    {
        for (const auto& elem : elems)
        {
            push_back(elem);
        }
    }

    linked_list(const linked_list& other) : mSize(0), mFirst(nullptr), mLast(nullptr)
    {
        for (const auto& elem : other)
        {
            push_back(elem);
        }
    }

    linked_list(linked_list&& other) noexcept : mSize(other.mSize), mFirst(other.mFirst), mLast(other.mLast)
    {
        other.mSize = 0;
        other.mFirst = nullptr;
        other.mLast = nullptr;
    }

    ~linked_list()
    {
        clear();
    }

    linked_list& operator=(const linked_list& other)
    {
        if (mFirst == other.mFirst)
        {
            return *this;
        }

        clear();
        for (const auto& elem : other)
        {
            push_back(elem);
        }
    }

    linked_list& operator=(linked_list&& other)
    {
        if (mFirst == other.mFirst)
        {
            return *this;
        }

        clear();
        mFirst = other.mFirst;
        mLast = other.mLast;
        mSize = other.mSize;
        other.mFirst = nullptr;
        other.mLast = nullptr;
        other.mSize = 0;
    }

    size_t size() const noexcept { return mSize; }
    bool is_empty() const noexcept { return size() == 0; }

    void push_front(const T& elem)
    {
        node* nw = new node{ elem, mFirst };
        mFirst = nw;
        if (mLast == nullptr)
        {
            mLast = nw;
        }

        ++mSize;
    }

    void push_back(const T& elem)
    {
        node* nw = new node{ elem, nullptr };
        if (mLast != nullptr)
        {
            mLast->next = nw;
        }
        mLast = nw;
        if (mFirst == nullptr)
        {
            mFirst = nw;
        }

        ++mSize;
    }

    void pop_front()
    {
        if (is_empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't pop front element of empty list.");
        }

        if (mFirst == mLast)
        {
            delete mFirst;
            mFirst = nullptr;
            mLast = nullptr;
        } else
        {
            node* tmp = mFirst;
            mFirst = mFirst->next;
            delete tmp;
        }

        --mSize;
    }

    void pop_back()
    {
        if (is_empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't pop back element from empty list.");
        }

        if (mLast == mFirst)
        {
            delete mLast;
            mFirst = nullptr;
            mLast = nullptr;
        } else
        {
            node* cur = mFirst;
            for (; cur->next != mLast; cur = cur->next) {}
            cur->next = nullptr;
            node* tmp = mLast;
            mLast = cur;
            delete tmp;
        }

        --mSize;
    }

    void clear()
    {
        while (!is_empty())
        {
            pop_front();
        }
    }

    const T& back()
    {
        if (is_empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't get back from empty list.");
        }

        return mLast->data;
    }

    const T& front()
    {
        if (is_empty())
        {
            throw std::out_of_range(__FUNCTION__ ": can't get front from empty list.");
        }

        return mFirst->data;
    }

    iterator insert_after(const iterator& pos, const T& data)
    {
        if (pos.mNext == nullptr && pos.mCurrent == nullptr)
        {
            throw std::out_of_range(__FUNCTION__": can't insert elements after end() iterator.");
        }

        if (pos.mNext == nullptr)
        {
            push_back(data);
            return iterator(mLast);
        }

        if (pos.mCurrent == nullptr)
        {
            push_front(data);
            return iterator(mFirst);
        }

        node* nw = new node{ data, pos.mNext };
        pos.mCurrent->next = nw;

        ++mSize;

        return iterator(nw);
    }

    void erase_after(const iterator& pos)
    {
        if (pos.mNext == nullptr)
        {
            throw std::out_of_range(__FUNCTION__ ": can't erase after end of list.");
        }

        if (pos.mCurrent == nullptr)
        {
            pop_front();
            return;
        }

        if (pos.mNext->next == nullptr)
        {
            pop_back();
            return;
        }

        pos.mCurrent->next = pos.mNext->next;
        delete pos.mNext;
        --mSize;
    }

    iterator begin() const noexcept { return iterator(mFirst); }
    iterator before_begin() const noexcept { return iterator(nullptr, mFirst); }
    iterator before_end() const noexcept { return iterator(mLast, nullptr); }
    iterator end() const noexcept { return iterator(); }
};