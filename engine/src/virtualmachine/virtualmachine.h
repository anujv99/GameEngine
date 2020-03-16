
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

		void Init(const pvstring & script);
		void Update(float dt);
		void Render();
		void Gui();
	private:
		lua_State * L;
		pvbool m_IsCalled = false;
		pvint m_UpdateFunction;
		pvint m_RenderFunction;
		pvint m_GuiFunction;
	};

}
