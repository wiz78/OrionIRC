#include <windows.h>
#include "../FileProps.h"

int main()
{
	FP_WriteProperties( L"d:\\tmp\\VCM_Config.h", "product", "comment" );

	return( 0 );
}