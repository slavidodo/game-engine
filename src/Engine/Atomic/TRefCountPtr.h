
#ifndef ENGINE_ATMOIC_TREFCOUNTPTR_H
#define ENGINE_ATMOIC_TREFCOUNTPTR_H

// NOT THREAD SAFE
template<typename TRefType>
class TRefCountPtr
{
public:
	TRefCountPtr()
		: mReference(nullptr)
	{
	}

	TRefCountPtr(TRefType* InReference, bool IncRef = true)
		: mReference(InReference)
	{
		if (mReference && IncRef) {
			mReference->AddRef();
		}
	}

	TRefCountPtr(const TRefCountPtr& Rhs)
	{
		mReference = Rhs.mReference;
		if (mReference) {
			mReference->AddRef();
		}
	}

	inline TRefCountPtr(TRefCountPtr&& Rhs)
	{
		mReference = Rhs.mReference;
		Rhs.mReference = nullptr;
	}

	TRefCountPtr& operator=(TRefType* InReference)
	{
		// Call AddRef before Release, in case the new reference is the same as the old reference.
		TRefType* OldReference = mReference;
		mReference = InReference;
		if (mReference) {
			mReference->AddRef();
		}
		if (OldReference) {
			OldReference->Release();
		}
		return *this;
	}

	TRefCountPtr& operator=(const TRefCountPtr& Rhs) {
		*this = Rhs.mReference;
	}

	TRefCountPtr& operator=(TRefCountPtr&& Rhs)
	{
		if (this != &Rhs) {
			TRefType* OldReference = mReference;
			mReference = Rhs.mReference;
			Rhs.mReference = nullptr;
			if (OldReference) {
				OldReference->Release();
			}
		}
		return *this;
	}

	~TRefCountPtr()
	{
		if (mReference) {
			mReference->Release();
		}
	}
	
	inline TRefType* GetReference() const {
		return mReference;
	}

	inline TRefType* operator->() const {
		return mReference;
	}

	inline operator TRefType*() const {
		return mReference;
	}

	inline operator bool() const {
		return IsValid();
	}

	bool IsValid() const {
		return mReference != nullptr;
	}

	void Release() {
		*this = nullptr;
	}

	void SafeRelease() {
		*this = nullptr;
	}

	uint32_t GetRefCount() {
		uint32_t Result = 0;
		if (mReference) {
			Result = mReference->GetRefCount();
		}
		return Result;
	}

	void Swap(TRefCountPtr& InPtr) {
		// swapping doesn't change ref count
		// notice the use of atomic counters, so this is faster approach
		TRefType* OldReference = mReference;
		mReference = InPtr.Reference;
		InPtr.mReference = OldReference;
	}

private:
	TRefType* mReference;
};

#endif // ENGINE_ATMOIC_TREFCOUNTPTR_H