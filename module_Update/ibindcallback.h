#include <urlmon.h>

class CBindTransferStatus : public IBindStatusCallback
{
public:
        CBindTransferStatus();
        virtual ~CBindTransferStatus();
		
		void RegistCallBack(BOOL (*CallBack)(int,int,int));
		void SetIndex(int i);
public:
     

//////////////////////////////////////////////////////////////////////////////////////////
//      IUnknown Method ����
//////////////////////////////////////////////////////////////////////////////////////////
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();
        STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj);


//////////////////////////////////////////////////////////////////////////////////////////
//      IBindStatusCallback Method ����
//////////////////////////////////////////////////////////////////////////////////////////
        STDMETHOD(GetBindInfo)(DWORD *pgrfBINDF, BINDINFO *pbindinfo);
        STDMETHOD(GetPriority)(LONG *pnPriotity);
        STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC *pfmtetc, STGMEDIUM *pstgmed);
        STDMETHOD(OnLowResource)(DWORD dwReserved);
        STDMETHOD(OnObjectAvailable)(REFIID riid, IUnknown* punk);
        STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusCode);
        STDMETHOD(OnStartBinding)(DWORD dwReserved, IBinding *pbinding);
        STDMETHOD(OnStopBinding)(HRESULT hrStatus, LPCWSTR szStatusText);

private:
	BOOL (*pCallBack)(int, int,int);
	int idx;
};
