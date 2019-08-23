#include "zcustompch.h"

#include "Application.h"

int main( void )
{
	{
		Application::GetInstance()->Init();
		Application::GetInstance()->Run();
		Application::GetInstance()->Exit();
	}
	return 0;
}