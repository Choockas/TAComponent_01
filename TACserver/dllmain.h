// dllmain.h : Declaration of module class.

class CTACserverModule : public ATL::CAtlDllModuleT< CTACserverModule >
{
public :
	DECLARE_LIBID(LIBID_TACserverLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TACSERVER, "{d5dfc4aa-99dd-4911-8938-8cd82bbcdbde}")
};

extern class CTACserverModule _AtlModule;
