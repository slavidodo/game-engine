
#ifndef ENGINE_MATH_MATHUTILITY_H
#define ENGINE_MATH_MATHUTILITY_H

class Math {
public:
	template<typename T>
	static inline bool IsPowerOfTwo(T Value) {
		return ((Value & (Value - 1)) == (T)0);
	}

	template<typename T>
	static inline T Max(const T A, const T B) {
		return A > B ? A : B;
	}

	template<typename T>
	static inline T Min(const T A, const T B) {
		return A < B ? A : B;
	}
};



#endif