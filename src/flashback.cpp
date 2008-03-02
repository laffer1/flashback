#include <iostream>
using namespace std;

#ifdef Win32
	#include <windows.h>

int WINAPI WinMain(HINSTANCE  hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShow)
{
#ifdef Debug
	cout << "Running in Debug Mode!" << endl;
	cout << "Windows Version" << endl;
#endif
	//Call Flashback Start
	return 0;
}

#else

int main(int argc, char** args)
{
#ifdef Debug
	cout << "Running in Debug Mode!" << endl;
#endif

	//Call Flashback Start
	return 0;
}	
#endif






