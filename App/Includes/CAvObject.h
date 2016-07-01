#ifndef _CAVOBJECT_H_
#define _CAVOBJECT_H_

class CAvObject {
public:
	CAvObject(){};
	virtual ~CAvObject(){};
};

/////////////////////////////////////////////////
///////////////// 单件模式
#define SINGLETON_DECLARE(CNAME)	\
static CNAME * instance();

#define SINGLETON_IMPLEMENT(CNAME)	\
CNAME * CNAME::instance()		\
{												\
	static CNAME * _instance = NULL;		\
	if( NULL == _instance)						\
	{											\
		_instance = new CNAME;				\
	}											\
	return _instance;							\
}
/////////////////////////////////////////////////

// Join two variables
#define MACRO_JOIN( X, Y ) MACRO_DO_JOIN( X, Y )
#define MACRO_DO_JOIN( X, Y ) MACRO_DO_JOIN2(X,Y)
#define MACRO_DO_JOIN2( X, Y ) X##Y


#endif


