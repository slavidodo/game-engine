
#ifndef ENGINE_CORE_MATH_RECT_H
#define ENGINE_CORE_MATH_RECT_H

template<typename T>
class TRect
{
public:
	using TPoint = glm::vec<2, T, glm::packed_lowp>;

	TRect() : mMinimum(0), mMaximum(0) {}

	TRect(TPoint min, TPoint max) {
		assert((min.x <= max.x && min.y <= max.y));

		mMinimum = min;
		mMaximum = max;
	}

	TRect(T xMin, T yMin, T xMax, T yMax)
		: TRect(TPoint(xMin, yMin), TPoint(xMax, yMax)) {}

	TPoint GetMinimum() const {
		return mMinimum;
	}

	TPoint GetMaximum() const {
		return mMaximum;
	}

	TPoint GetSize() const {
		return mMaximum - mMinimum;
	}

	inline TRect<T>& operator*=(T scale) {
		mMinimum *= scale;
		mMaximum *= scale;
		return *this;
	}


	inline TRect<T>& operator+=(const TRect<T>& point) {
		mMinimum += point;
		mMaximum += point;
		return *this;
	}


	inline TRect<T>& operator-=(const TRect<T>& point) {
		mMinimum -= point;
		mMaximum -= point;
		return *this;
	}


	inline TRect<T> operator*(T scale) const {
		return TRect<T>(mMinimum * scale, mMaximum * scale);
	}

	inline bool operator==(const TRect<T>& other) const {
		return mMinimum == other.mMinimum && mMaximum == other.mMaximum;
	}

	inline bool operator!=(const TRect<T>& other) const {
		return mMinimum != other.mMinimum || mMaximum != other.mMaximum;
	}

private:
	TPoint mMinimum;
	TPoint mMaximum;
};

template<typename T>
inline std::ostream& operator <<(std::ostream& os, const TRect<T>& Rect)
{
	return os << "Rect(" << Rect.GetMinimum().x
		<< ", " << Rect.GetMinimum().x
		<< ", " << Rect.GetMinimum().y
		<< ", " << Rect.GetMaximum().x
		<< ", " << Rect.GetMaximum().y
		<< ")";
}

typedef TRect<float> Rect;
typedef TRect<int32_t> IntRect;

#endif //ENGINE_CORE_MATH_RECT_H