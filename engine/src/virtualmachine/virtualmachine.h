
#include "../common/singleton.h"

#include "lua.hpp"
#include "config.h"

namespace prev {

	class VirtualMachine : public Singleton<VirtualMachine> {
		friend class Singleton<VirtualMachine>;
	private:
		VirtualMachine();
		~VirtualMachine();
	public:
		void DoString(const pvstring & script);
		Config ReadConfigFile(const pvstring & path);
	private:
		lua_State * L;
	};

}
