#pragma once

#ifdef WIN32
#include <windows.h>
#endif


class Hook{

private:
	void * m_hkAddy;
	void * m_hkFnctAddy;
	DWORD m_len;
	bool m_isHooked;
public:
	Hook(void * hkAddy, void * hkFnctAddy, DWORD len) {
		m_isHooked = false;
		m_hkAddy = hkAddy;
		m_len = len;
		m_hkFnctAddy = hkFnctAddy;
	}

	bool Hooked() { return m_isHooked; }

	void * CreateDetour(){
		void * pTrp;
		DWORD oldProtect, Bkup, relativeAddy;

		if (m_hkAddy == NULL || m_len < 5)
			return nullptr;

		if (!VirtualProtect(m_hkAddy, m_len, PAGE_EXECUTE_READWRITE, &oldProtect))
			return nullptr;

		//Allocate a spot of memory for pTrp
		pTrp = VirtualAlloc(0, m_len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		//Copy the bytes into the beginning of pTrp for when we're returning.
		memcpy(pTrp, m_hkAddy, m_len);
		//NOP out the start of the function
		memset(m_hkAddy, 0x90, m_len);

		/*PLACING OUR JUMP AT THE ORIGINAL FUNCTION*/

		//Get relative address, place jmp, put relative address on next 4 bytes
		relativeAddy = ((DWORD)m_hkFnctAddy - (DWORD)m_hkAddy) - 5;
		*(BYTE*)m_hkAddy = 0xE9;
		*(DWORD*)((DWORD)m_hkAddy + 0x1) = relativeAddy;

		/*PLACING OUR JUMP AT THE TRAMPOLINE*/

		//Get relative address, place jmp, put relative address on next 4 bytes
		DWORD relAddy = ((DWORD)m_hkAddy - (DWORD)pTrp) - 5;						//This really make sense? 
		*(BYTE*)((DWORD)pTrp + m_len) = 0xE9;
		*(DWORD*)((DWORD)pTrp + m_len + 0x1) = relAddy;

		//Restore whatever protection there was
		if (!VirtualProtect(m_hkAddy, m_len, oldProtect, &Bkup))
			return nullptr;

		return pTrp;
	}
};