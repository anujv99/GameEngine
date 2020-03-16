#pragma once

namespace prev {

	struct TypeID {
		template<typename T>
		static inline unsigned int value() {
			static unsigned int id = ++counter;
			return id;
		}
	private:
		static unsigned int counter;
	};

}