#ifndef _MPtr_hh_
#define _MPtr_hh_

template <class T> class MPtr {
    public:
	MPtr ();
	MPtr (T *);
	MPtr (const MPtr<T> &);
	~MPtr();

    public:
	T *       operator -> ();
	const T * operator -> () const;
	MPtr<T>&  operator = (const MPtr<T> &);
	T &		  operator * ();
	const T & operator * () const;
	int		  operator == (const MPtr<T> &) const;
	int 	  operator != (const MPtr<T> &) const;
//            operator int () const { return mObj != 0; }
			  operator T * () { return mObj; }

    void debugit();

//    public: // this was private, made public to get it to compile
	//  This is verboten (address of).
	private:

	MPtr<T> *	operator & ();

    private:
	//  What we're pointing to...
	T *		mObj;
    
};

template <class T> class MPtrDLLNode {
	public:
		MPtrDLLNode(const T &);
		~MPtrDLLNode(){};

		MPtrDLLNode *_next;
		MPtrDLLNode *_prev;
		T _item;
};

template <class T> class MPtrDLL {
    public:
		MPtrDLL ();
		~MPtrDLL();

    public:
		T operator [] (int);
		void append(T);
		int remove(MPtrDLLNode<T> *);
        void remove(int );
        int reset();
        int size() { return _size; }
        void moveUp(int);
        void moveDown(int);
		MPtrDLLNode<T> * next(MPtrDLLNode<T> *p) { return p->_next; }
		MPtrDLLNode<T> * prev(MPtrDLLNode<T> *p) { return p->_prev; }
		MPtrDLLNode<T> * head() { return _head; }

    private:
		MPtrDLLNode<T> *_head;
		MPtrDLLNode<T> *_tail;
		int _size;

};

#define __BUILDING_MPTR_H__
#include "MPtr.cpp"
#endif

