
#include "pch.h"

#include "OpenGLDynamicRHI.h"

#include <boost/functional/hash.hpp>

static inline void SetupGLElement(OpenGLVertexElement& GLElement, GLenum Type, GLuint Size, bool bNormalized, bool bShouldConvertToFloat)
{
	GLElement.Type = Type;
	GLElement.Size = Size;
	GLElement.Normalized = bNormalized;
	GLElement.ShouldConvertToFloat = bShouldConvertToFloat;
}

struct OpenGLVertexDeclarationKey
{
	OpenGLVertexElements VertexElements;
	size_t Hash;

	uint16_t StreamStrides[OpenGLVertexDeclaration::MaxVertexElementCount];

	/** Initialization constructor. */
	explicit OpenGLVertexDeclarationKey(const VertexDeclarationElementList& elements)
	{
		uint16_t UsedStreamsMask = 0;
		memset(StreamStrides, 0, sizeof(StreamStrides));

		VertexElements.reserve(elements.size());
		for (size_t i = 0; i < elements.size(); i++) {
			const VertexElement& Element = elements[i];
			OpenGLVertexElement GLElement;
			GLElement.StreamIndex = Element.StreamIndex;
			GLElement.MemberOffset = Element.MemberOffset;
			GLElement.Divisor = Element.UseInstanceIndex ? 1 : 0;
			GLElement.AttributeIndex = Element.AttributeIndex;
			GLElement.HashStride = Element.Stride;
			GLElement.Padding = 0;
			switch (Element.Type) {
				case VET_Float1: SetupGLElement(GLElement, GL_FLOAT, 1, false, true); break;
				case VET_Float2: SetupGLElement(GLElement, GL_FLOAT, 2, false, true); break;
				case VET_Float3: SetupGLElement(GLElement, GL_FLOAT, 3, false, true); break;
				case VET_Float4: SetupGLElement(GLElement, GL_FLOAT, 4, false, true); break;
				case VET_PackedNormal: SetupGLElement(GLElement, GL_BYTE, 4, true, true); break;
				case VET_UByte4: SetupGLElement(GLElement, GL_UNSIGNED_BYTE, 4, false, false); break;
				case VET_UByte4N: SetupGLElement(GLElement, GL_UNSIGNED_BYTE, 4, true, true); break;
				case VET_Color:
					// todo: gles doesn't support bgra
					SetupGLElement(GLElement, GL_UNSIGNED_BYTE, GL_BGRA, true, true);
					break;
				case VET_Short2: SetupGLElement(GLElement, GL_SHORT, 2, false, false); break;
				case VET_Short4: SetupGLElement(GLElement, GL_SHORT, 4, false, false); break;
				case VET_Short2N: SetupGLElement(GLElement, GL_SHORT, 2, true, true); break;
				case VET_Half2:
					// todo: support half2
					SetupGLElement(GLElement, GL_SHORT, 2, false, true);
					break;
				case VET_Half4:
					// todo: support half4
					SetupGLElement(GLElement, GL_SHORT, 4, false, true);
					break;
				case VET_Short4N: SetupGLElement(GLElement, GL_SHORT, 4, true, true); break;
				case VET_UShort2: SetupGLElement(GLElement, GL_UNSIGNED_SHORT, 2, false, false); break;
				case VET_UShort4: SetupGLElement(GLElement, GL_UNSIGNED_SHORT, 4, false, false); break;
				case VET_UShort2N: SetupGLElement(GLElement, GL_UNSIGNED_SHORT, 2, true, true); break;
				case VET_UShort4N: SetupGLElement(GLElement, GL_UNSIGNED_SHORT, 4, true, true); break;
				case VET_URGB10A2N: SetupGLElement(GLElement, GL_UNSIGNED_INT_2_10_10_10_REV, 4, true, true); break;
				case VET_UInt: SetupGLElement(GLElement, GL_UNSIGNED_INT, 1, false, false); break;
			};

			if ((UsedStreamsMask & 1 << Element.StreamIndex) != 0) {
				// unreal-engine
				//ensure(StreamStrides[Element.StreamIndex] == Element.Stride);
			} else {
				UsedStreamsMask = UsedStreamsMask | (1 << Element.StreamIndex);
				StreamStrides[Element.StreamIndex] = Element.Stride;
			}

			VertexElements.push_back(GLElement);
		}

		struct CompareFOpenGLVertexElement
		{
			inline bool operator()(const OpenGLVertexElement& A, const OpenGLVertexElement& B) const
			{
				if (A.StreamIndex < B.StreamIndex) {
					return true;
				}
				if (A.StreamIndex > B.StreamIndex) {
					return false;
				}
				if (A.MemberOffset < B.MemberOffset) {
					return true;
				}
				if (A.MemberOffset > B.MemberOffset) {
					return false;
				}
				if (A.AttributeIndex < B.AttributeIndex) {
					return true;
				}
				if (A.AttributeIndex > B.AttributeIndex) {
					return false;
				}
				return false;
			}
		};

		// Sort the OpenGLVertexElements by stream then offset.
		std::sort(VertexElements.begin(), VertexElements.end(), CompareFOpenGLVertexElement());

		GenerateHash();
	}

	void GenerateHash();
};

namespace boost {

template<>
struct hash<OpenGLVertexElement>
{
	std::size_t operator()(const OpenGLVertexElement& c) {
		std::size_t seed = 0;
		boost::hash_combine(seed, c.Type);
		boost::hash_combine(seed, c.StreamIndex);
		boost::hash_combine(seed, c.MemberOffset);
		boost::hash_combine(seed, c.Size);
		boost::hash_combine(seed, c.Divisor);
		boost::hash_combine(seed, c.HashStride);
		boost::hash_combine(seed, c.Normalized);
		boost::hash_combine(seed, c.AttributeIndex);
		boost::hash_combine(seed, c.ShouldConvertToFloat);
		boost::hash_combine(seed, c.Padding);
		return seed;
	}
};

template<>
struct hash<OpenGLVertexDeclarationKey>
{
	std::size_t operator()(const OpenGLVertexDeclarationKey& key) const
	{
		std::size_t seed = 0;
		hash_combine(seed, hash_range(key.VertexElements.begin(), key.VertexElements.end()));
		hash_combine(seed, boost::hash_range(key.StreamStrides,
													key.StreamStrides + sizeof(key.StreamStrides)));

		return seed;
	}
};

}

namespace std {

template<>
struct hash<OpenGLVertexDeclarationKey>
{
	std::size_t operator()(const OpenGLVertexDeclarationKey& key) const
	{
		return key.Hash;
	}
};

}

void OpenGLVertexDeclarationKey::GenerateHash()
{
	boost::hash<OpenGLVertexDeclarationKey> hasher;
	Hash = hasher(*this);
}

bool operator==(const OpenGLVertexElement& A, const OpenGLVertexElement& B)
{
	return A.Type == B.Type && A.StreamIndex == B.StreamIndex && A.MemberOffset == B.MemberOffset && A.Size == B.Size
		&& A.Divisor == B.Divisor && A.Normalized == B.Normalized && A.AttributeIndex == B.AttributeIndex
		&& A.ShouldConvertToFloat == B.ShouldConvertToFloat && A.HashStride == B.HashStride;
}

bool operator==(const OpenGLVertexDeclarationKey& A, const OpenGLVertexDeclarationKey& B)
{
	return A.VertexElements == B.VertexElements;
}

bool operator<(const OpenGLVertexDeclarationKey& A, const OpenGLVertexDeclarationKey& B)
{
	return false;
}

std::unordered_map<OpenGLVertexDeclarationKey, RHIVertexDeclarationRef> gOpenGLVertexDeclarationCache;

RHIVertexDeclarationRef OpenGLDynamicRHI::RHICreateVertexDeclaration(const VertexDeclarationElementList& elements)
{
	// the key is used to avoid multiple exact declarations
	OpenGLVertexDeclarationKey key(elements);

	auto it = gOpenGLVertexDeclarationCache.find(key);
	if (it != gOpenGLVertexDeclarationCache.end()) {
		return (*it).second;
	}

	auto itt = gOpenGLVertexDeclarationCache.insert({ key, new OpenGLVertexDeclaration(key.VertexElements, key.StreamStrides) });
	return (*itt.first).second;
}

bool OpenGLVertexDeclaration::GetInitializer(VertexDeclarationElementList& list)
{
	for (size_t ElementIndex = 0; ElementIndex < VertexElements.size(); ElementIndex++) {
		OpenGLVertexElement const& GLElement = VertexElements[ElementIndex];
		VertexElement Element;
		Element.StreamIndex = GLElement.StreamIndex;
		Element.MemberOffset = GLElement.MemberOffset;
		Element.UseInstanceIndex = GLElement.Divisor == 1; // a divisor of one hints per instance
		Element.AttributeIndex = GLElement.AttributeIndex;
		Element.Stride = GLElement.HashStride;

		switch (GLElement.Type) {
			case GL_FLOAT:
			{
				switch (GLElement.Size) {
					case 1: Element.Type = VET_Float1; break;
					case 2: Element.Type = VET_Float2; break;
					case 3: Element.Type = VET_Float3; break;
					case 4: Element.Type = VET_Float4; break;
				}
				break;
			}
			case GL_UNSIGNED_BYTE:
			{
				if (GLElement.Size == 4) {
					Element.Type = (GLElement.Normalized) ? VET_UByte4N : VET_UByte4;
				} else if (GLElement.Size == GL_BGRA) {
					Element.Type = VET_Color;
				}
				break;
			}
			case GL_BYTE:
			{
				if (GLElement.Size == 4) {
					Element.Type = VET_PackedNormal;
				}
				break;
			}
			case GL_SHORT:
			{
				switch (GLElement.Size) {
					case 2:
						if (GLElement.Normalized) {
							Element.Type = VET_Short2N;
						} else {
							Element.Type = (!GLElement.ShouldConvertToFloat) ? VET_Short2 : VET_Half2;
						}
						break;
					case 4:
						if (GLElement.Normalized) {
							Element.Type = VET_Short4N;
						} else {
							Element.Type = (!GLElement.ShouldConvertToFloat) ? VET_Short4 : VET_Half4;
						}
						break;
				}
				break;
			}
#if defined(GL_HALF_FLOAT)
			case GL_HALF_FLOAT:
#endif
#if defined(GL_HALF_FLOAT_OES)
			case GL_HALF_FLOAT_OES:
#endif
			{
				switch (GLElement.Size) {
					case 2: Element.Type = VET_Half2; break;
					case 4: Element.Type = VET_Half4; break;
				}
				break;
			}
			case GL_UNSIGNED_SHORT:
			{
				switch (GLElement.Size) {
					case 2:
						if (GLElement.Normalized) {
							Element.Type = VET_UShort2N;
						} else {
							Element.Type = VET_UShort2;
						}
						break;
					case 4:
						if (GLElement.Normalized) {
							Element.Type = VET_UShort4N;
						} else {
							Element.Type = VET_UShort4;
						}
						break;
				}
				break;
			}
			case GL_UNSIGNED_INT_2_10_10_10_REV:
			{
				Element.Type = VET_URGB10A2N;
				break;
			}
			case GL_UNSIGNED_INT:
			{
				Element.Type = VET_UInt;
				break;
			}
		}
		list.push_back(Element);
	}

	return true;
}
