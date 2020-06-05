
#ifndef ENGINE_ATMOIC_REFCOUNTEROBJECT_H
#define ENGINE_ATMOIC_REFCOUNTEROBJECT_H

class IRefCountedObject
{
public:
	virtual ~IRefCountedObject() { }
	virtual uint32_t AddRef() const = 0;
	virtual uint32_t Release() const = 0;
	virtual uint32_t GetRefCount() const = 0;
};

class RefCountedObject : IRefCountedObject
{
public:
	RefCountedObject()
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
		int32_t RefCount = mNumRefs;
		return uint32_t(RefCount);
	}

private:
	mutable int32_t mNumRefs;
};

#endif // ENGINE_ATMOIC_REFCOUNTEROBJECT_H