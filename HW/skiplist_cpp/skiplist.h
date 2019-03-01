#ifndef SKIPLIST_H
#define SKIPLIST_H


/* You may only include these two header files. */
#include <utility>      /* For std::pair. */
#include <functional>   /* For std::less. */


#define RAND_HALF ((RAND_MAX) / 2)


/* Key type, value type, comparator class. */
template<class Key, class Val, class Compare = std::less<Key> >
class skiplist
{
private:    /* Internal classes. */
    /* Private node class. */
    class _listnode
    {
    private:
        /* ==================== Begin students' code ==================== */
        /* One possible field has been filled for you, you may change its name. */
        std::pair<const Key, Val> _data;
        _listnode** forward;
        int node_height_limit;
        /* Fill in other needed _listnode member variables. */
        /* ==================== End students' code ==================== */

        /* Friend classes to access private value. */
        friend class skiplist;
        friend class iterator;
        friend class const_iterator;
    public:
        /* ==================== Begin students' code ==================== */
        /* Fill in node constructor and destructors. */
        _listnode(const Key& key, const Val& val, int height_limit);
        ~_listnode();
        /* ==================== End students' code ==================== */
    };

private:    /* Skiplist contents. */
    /* ==================== Begin students' code ==================== */
    /* Fill in skiplist member variables. */
    int _level;
    int Height_limit;
    int _size;
    Compare Cmp;
    _listnode* header;
    /* ==================== End students' code ==================== */

public:     /* RAII */
    /* Constructor declaration. */
    skiplist(int height_limit, Compare cmp = Compare());
    /* disable copy constructor (This would cause double free) */
    skiplist(const skiplist&) = delete;
    /* Destructor declaration. */
    ~skiplist();

private:    /* Custom helper functions. */
    /* ==================== Begin students' code ==================== */
    /* Fill in zero or more helper functions as you wish. */
    /* ==================== End students' code ==================== */

public:     /* Basic iterator definitions. */
    class iterator
    {
    private:    /* Iterator data. */
        /* ==================== Begin students' code ==================== */
        /* Fill in needed iterator member variables. */
        _listnode* nodeptr;
        /* ==================== End students' code ==================== */
    public:
        /* Constructor & destructor. */
        iterator(_listnode *ptr = nullptr);
        ~iterator();
        /* 2x operator++. */
        inline iterator &operator++();      /* ++it */
        inline iterator operator++(int);    /* it++ */
        /* Equality operator. */
        inline bool operator==(const iterator &other) const;
        inline bool operator!=(const iterator &other) const;
        /* Dereference operator. */
        /* Return type is a reference to std::pair<const Key, Val>. */
        inline std::pair<const Key, Val> &operator*() const;
        /* Return type is a pointer to std::pair<const Key, Val>. */
        inline std::pair<const Key, Val> *operator->() const;
    };

    class const_iterator
    {
        /* ==================== Begin students' code ==================== */
        /* Fill in needed const_iterator defination */
    private:
        _listnode* nodeptr;
    public:
      /* Constructor & destructor. */
      const_iterator(_listnode *ptr = nullptr);
      ~const_iterator();
      /* 2x operator++. */
      inline const_iterator &operator++();      /* ++it */
      inline const_iterator operator++(int);    /* it++ */
      /* Equality operator. */
      inline bool operator==(const const_iterator &other) const;
      inline bool operator!=(const const_iterator &other) const;
      /* Dereference operator. */
      /* Return type is a reference to std::pair<const Key, Val>. */
      inline std::pair<const Key, Val> &operator*() const;
      /* Return type is a pointer to std::pair<const Key, Val>. */
      inline std::pair<const Key, Val> *operator->() const;
        /* ==================== End students' code ==================== */
    };

    /* STL-like begin end iterator interface. */
    inline iterator begin();
    inline iterator end();
    /* ==================== Begin students' code ==================== */
    /* Fill in needed begin() and end() for const_iterator */
    inline const_iterator begin() const;
    inline const_iterator end() const;
    /* ==================== End students' code ==================== */

public:     /* Functionalities: find, insert, erase. */
    /* Find a key in skiplist. Return a iterator to the found node, end() if not found. */
    iterator find(const Key &key);
    const_iterator find(const Key &key) const;

    /* Insert node with key and value.
        Return type is std::pair<iterator, bool>.
            - The first argument is the iterator to the inserted node. Replace
                old value if key already exists.
            - The second argument is the a boolean indicating whether the KV pair
                is newly inserted (true) or the old value is replaced (false).
     */
    std::pair<iterator, bool> insert(const Key &key, const Val &val);

    /* Erase node with key. Return true on success, or false if key is not found. */
    bool erase(const Key &key);

public:     /* Utility part. */
    /* Examine if the container is empty. */
    inline bool empty() const;
    /* Get the size of container. */
    inline int size() const;
    /* Get the current level of the skiplist. */
    inline int level() const;

    void print_list();
};


#include "skiplist.hpp"

#endif  /* SKIPLIST_H */
