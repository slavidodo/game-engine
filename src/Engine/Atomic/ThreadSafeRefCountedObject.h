
#ifndef ENGINE_ATMOIC_THREADSAFEREFCOUNTEROBJECT_H
#define ENGINE_ATMOIC_THREADSAFEREFCOUNTEROBJECT_H

#include "RefCountedObject.h"

class ThreadSafeRefCountedObject : IRefCountedObject
{
public:
	ThreadSafeRefCountedObject()
		: mNumRefs(0) {}

	virtual uint32_t AddRef() const override {
		int32_t NewRefCount = ++mNumRefs;
		return uint32_t(NewRefCount);
	}

	virtual uint32_t Release() const override {
		int32_t NewRefCount = --mNumRefs;
		if (NewRefCount == 0) {
			delete this;
		}
		return uint32_t(NewRefCount);
	}

	virtual uint32_t GetRefCount() const override {
		int32_t RefCount = mNumRefs.load();
		return uint32_t(RefCount);
	}

private:
	mutable std::atomic<int32_t> mNumRefs;
};

#endif // ENGINE_ATMOIC_THREADSAFEREFCOUNTEROBJECT_H