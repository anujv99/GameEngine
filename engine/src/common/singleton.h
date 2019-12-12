#pragma once

#include "assert.h"

namespace prev {

	template<class T>
	class Singleton {
	public:
		static void CreateInst();
		static void DestroyInst();

		inline static bool InstExists();
		inline static T * Get();
		inline static T & Ref();
	private:
		static T * s_Instance;
	};

	template<class T>
	inline void Singleton<T>::CreateInst() {
		if (!s_Instance) s_Instance = new T();
	}

	template<class T>
	inline void Singleton<T>::DestroyInst() {
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	template<class T>
	inline bool Singleton<T>::InstExists() {
		return s_Instance != nullptr;
	}

	template<class T>
	inline T * Singleton<T>::Get() {
		return s_Instance;
	}

	template<class T>
	inline T & Singleton<T>::Ref() {
		ASSERT(s_Instance != nullptr);
		return *s_Instance;
	}

	template<class T>
	T * Singleton<T>::s_Instance = nullptr;

}
