#ifdef __BUILDING_MPTR_H__
//#include "MPtr.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

static int mptr_total_created = 0;
static int mptr_accumulated = 0;

template <class T>
MPtr<T>::MPtr () {
    mObj = (T *) 0;
    mptr_total_created++;
    mptr_accumulated++;
}

template <class T>
MPtr<T>::MPtr (T *arg) {
    mObj = arg;
    if (mObj)
	mObj->reference ();
    mptr_total_created++;
    mptr_accumulated++;
}

template <class T>
MPtr<T>::MPtr (const MPtr<T> &arg) {
    mObj = arg.mObj;
    if (mObj)
	mObj->reference ();
    mptr_total_created++;
    mptr_accumulated++;
}

template <class T>
MPtr<T>::~MPtr () {
    if (mObj)
	mObj->unreference ();
    mptr_accumulated--;
}

template <class T>
T *
MPtr<T>::operator -> () {
    if (mObj)
	return mObj;
//    cerr << "MPtr -> operator: object is NULL\n";
    abort ();
    // Not reached.
    return (T *) 0;
}

template <class T>
const T *
MPtr<T>::operator -> () const {
    if (mObj)
	return mObj;
    cerr << "MPtr -> operator: object is NULL\n";
    exit (1);
    // Not reached.
    return (T *) 0;
}

template <class T>
T &
MPtr<T>::operator * () {
    if (mObj)
	return *mObj;
    cerr << "MPtr -> operator: object is NULL\n";
    exit (1);
    // Not reached.
    return *mObj;
}

template <class T>
const T &
MPtr<T>::operator * () const {
    if (mObj)
	return *mObj;
    cerr << "MPtr -> operator: object is NULL\n";
    exit (1);
    // Not reached.
    return *mObj;
}

template <class T>
MPtr<T> &
MPtr<T>::operator = (const MPtr<T> &pt) {
    //
    //  Note: don't change the ref/unref order.
    //  It works for the case: x = x;
    //
    if (pt.mObj)
		pt.mObj->reference ();
    if (mObj)
		mObj->unreference ();
    mObj = pt.mObj;
    return *this;
}

template <class T>
MPtr<T> *
MPtr<T>::operator & () {
    cerr << "MPtr & operator: Illegal operation\n";
    exit (1);
    // Not reached.
    return this;
}

template <class T>
void
MPtr<T>::debugit() {
    if (mptr_total_created && mptr_accumulated) {
        return;
    }
}

template <class T>
int
MPtr<T>::operator == (const MPtr<T> &t) const {
    if (mObj == t.mObj)
	return 1;
    if ((mObj && !t.mObj) || (!mObj && t.mObj))
	return 0;
    return *(mObj) == *(t.mObj);
}

template <class T>
MPtrDLLNode<T>::MPtrDLLNode<T>(const T & node): _next(0), _prev(0), _item(node) {
}

template <class T>
MPtrDLL<T>::~MPtrDLL<T>() {
	reset();
}

template <class T>
int
MPtrDLL<T>::reset() {
	while (remove(head()));
    return 0;
}

template <class T>
int
MPtrDLL<T>::remove(MPtrDLLNode<T> *p) {
	if (p == 0) return 0;

	if (_head == p && p == _tail) {
		_head = 0;
		_tail = 0;
	}
	if (_head == p) {
		_head = p->_next;
	}
	if (_tail == p) {
		_tail = p->_prev;
	}
	if (p->_prev != 0)
		p->_prev->_next = p->_next;
	if (p->_next != 0)
		p->_next->_prev = p->_prev;

//	logger.log("deleting:");
//	logger.log(p->_item->getId3("FILE").fast_rep());
	delete p;
	_size--;
	return 1;

}

template <class T> void
MPtrDLL<T>::remove(int element) {
	if (element >= _size) {
//		cerr << "MPtrDLL::remove [] error: element >= size!";
        abort();
	}
    
    MPtrDLLNode<T> * p = _head;
    int i = 0;
    while (i < element) {
        if (!p) {
//            cerr << "out of bounds";
            abort();
            return ;
        }
        p = this->next(p);
        i++;
    }
    remove (p);
    return ;
}
template <class T> void
MPtrDLL<T>::replace(MPtrDLLNode<T> ** p, T song) {
	PlaylistNode * newp = new PlaylistNode(song);
	newp->_prev = (*p)->_prev;
	newp->_next = (*p)->_next;
	if ((*p)->_prev)
		(*p)->_prev->_next = newp;
	if ((*p)->_next)
		(*p)->_next->_prev = newp;
	delete *p;
	*p = newp;
}

template <class T>
MPtrDLL<T>::MPtrDLL<T>(): _head(0), _tail(0), _size(0) {
}

// get the nth element
template <class T> T
MPtrDLL<T>::operator [] (int element) {
	if (element >= _size) {
//		cerr << "MPtrDLL::operator [] error: element >= size!";
        abort();
	}
    
    MPtrDLLNode<T> * p = _head;
    int i = 0;
    while (i < element) {
        if (!p) {
//            cerr << "out of bounds";
            abort();
            T x;
            return x;
        }
        p = this->next(p);
        i++;
    }
    return p->_item;
}

template <class T>
void
MPtrDLL<T>::append(T arg) {
	MPtrDLLNode<T> *p = new MPtrDLLNode<T>(arg);
	if (_head == 0) {
		_head = p;
		_tail = _head;
	} else {
		_tail->_next = p;
		p->_prev = _tail;
		_tail = p;
	}
	_size++;
}

template <class T>
void
MPtrDLL<T>::moveUp(int element) {
	if (element >= _size)
        return;
    if (element < 1) 
        return; // can't move up any higher
    MPtrDLLNode<T> * p = _head;
    int i = 0;
    while (i < element) {
        if (!p)
            return ;
        p = this->next(p);
        i++;
    }
    if (!p || p == _head)
        return;

    // swap p & prev
    MPtrDLLNode<T> * oldnext = p->_next;
    MPtrDLLNode<T> * oldprev = p->_prev;
    p->_next = oldprev;
    if (oldprev)
        p->_prev = oldprev->_prev;
    if (oldprev) {
        if (oldprev->_prev)
            oldprev->_prev->_next = p;
        oldprev->_next = oldnext;
        oldprev->_prev = p;
    }
    if (oldnext)
        oldnext->_prev = oldprev;
    if (_head == oldprev)
        _head = p;
    if (_tail == p)
        _tail = oldprev;
}
// p is 3
// orig      1,2,3,4
// should be 1,3,2,4
// got       1,2,4,err

template <class T>
void
MPtrDLL<T>::moveDown(int element) {
    if (element >= _size)
        return;
    MPtrDLLNode<T> * p = _head;
    int i = 0;
    while (i < element) {
        if (!p)
            return ;
        p = this->next(p);
        i++;
    }
    if (!p || p == _tail)
        return;

    // swap p & next
    MPtrDLLNode<T> * oldnext = p->_next;
    MPtrDLLNode<T> * oldprev = p->_prev;
    if (oldnext)
        p->_next = oldnext->_next;
    p->_prev = oldnext;
    if (oldprev)
        oldprev->_next = oldnext;
    if (oldnext) {
        if (oldnext->_next)
            oldnext->_next->_prev = p;
        oldnext->_prev = oldprev;
        oldnext->_next = p;
    }
    if (_head == p)
        _head = oldnext;
    if (_tail == oldnext)
        _tail = p;
}
#endif