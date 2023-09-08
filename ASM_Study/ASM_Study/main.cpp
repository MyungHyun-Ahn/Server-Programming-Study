#include <stdio.h>
#include <Windows.h>
#include <conio.h>

enum CMP_RESULT
{
	A_EQUAL_B,
	A_GREATOR_B,
	A_LESS_B,
	A_ABOVE_B, // 부호 고려 X
	A_BELOW_B,
	CMP_RESULT_COUNT
};

// 데이터 전송 예제
void CopyTest()
{
	__declspec(align(16)) char szSrc[64] = "ABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEF.";
	__declspec(align(16)) char szDest[64] = {  };

	__asm
	{
		lea esi, dword ptr[szSrc]
		lea edi, dword ptr[szDest]

		; copy 1byte, szDest[0] = szSrc[0]
		mov al, byte ptr[esi]
		mov byte ptr[edi], al

		; copy 2byte, szDest[0 - 1] = szSrc[0 - 1]
		mov ax, word ptr[esi]
		mov word ptr[edi], ax

		; copy 4byte, szDest[0 - 3] = szSrc[0 - 3]
		mov eax, dword ptr[esi]
		mov dword ptr[edi], eax

		; copy 4byte * 16, szDest[0 - 63] = szSrc[0 - 63]
		mov ecx, 64
		shr ecx, 2
		rep movsd
		nop
	}
}

CMP_RESULT CmpTestSigned(int a, int b)
{
	CMP_RESULT r = A_EQUAL_B;

	// signed - (a > b) -> jg (ZF = 0 and SF = OF)
	// signed - (a < b) -> jl (SF <> OF)
	// signed - (a == b) -> je (ZF = 1) 

	__asm
	{
		mov eax, dword ptr[a]
		mov edx, dword ptr[b]
		cmp eax, edx
		je lb_a_equal_b
		jg lb_a_greator_b
		jl lb_a_less_b

	lb_a_equal_b:
		mov DWORD ptr[r], A_EQUAL_B
		jmp lb_exit

	lb_a_greator_b:
		mov DWORD ptr[r], A_GREATOR_B
		jmp lb_exit

	lb_a_less_b:
		mov DWORD ptr[r], A_LESS_B
		jmp lb_exit

	lb_exit:
		nop
	}
	return r;
}

CMP_RESULT CmpTestUnsigned(int a, int b)
{
	CMP_RESULT r = A_EQUAL_B;

	// unsigned ja (CF = 0 and ZF = 0)  above
	// unsigned jb (CF = 1)				below
	// signed - (a == b) -> je (ZF = 1) 

	__asm
	{
		mov eax, dword ptr[a]
		mov edx, dword ptr[b];
		cmp eax, edx
			je lb_a_equal_b
			ja lb_a_above_b
			jb lb_a_below_b
			int 3

	lb_a_equal_b:
		mov dword ptr[r], A_EQUAL_B
		jmp lb_exit

	lb_a_above_b:
		mov dword ptr[r], A_ABOVE_B
		jmp lb_exit

	lb_a_below_b:
		mov dword ptr[r], A_BELOW_B
		jmp lb_exit

	lb_exit:
		nop

	}
	return r;
}

void TestCmpTest()
{

	const WCHAR* wchInequality[] = { L"==", L">", L"<", L">", L"<" };
	CMP_RESULT r = A_EQUAL_B;

	int a = -1;
	int b = 0;
	int c = 0;

	wprintf_s(L"Signed Test\n");
	r = CmpTestSigned(a, b);
	wprintf_s(L"CMP A, B -> %d %s %d\n", a, wchInequality[r], b);

	wprintf_s(L"\n");

	wprintf_s(L"Unsigned Test\n");
	r = CmpTestUnsigned(a, b);
	wprintf_s(L"CMP A, B -> %u %s %u\n", a, wchInequality[r], b);

	wprintf_s(L"Press any key.\n");
	_getch();
}

int Add_C(int a, int b)
{
	int c = a + b;
	return c;
}

int Add_ASM_CDECL(int a, int b)
{
	int c;
	__asm
	{
		mov eax, dword ptr[a]
		mov edx, dword ptr[b]
		add eax, edx
		mov dword ptr[c], eax
	}
	return c;
}

int __declspec(naked) __cdecl Mul_ASM(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp, esp

		mov eax, dword ptr[a]
		mov ecx, dword ptr[b]
		imul ecx

		mov esp, ebp
		pop ebp

		ret
	}
}


int __declspec(naked) __cdecl Div_ASM(int a, int b)
{
	__asm
	{
		push ebp
		mov ebp, esp

		mov eax, dword ptr[a]
		mov ecx, dword ptr[b]
		idiv ecx

		mov esp, ebp
		pop ebp

		ret
	}
}

void TestCallMemcpy()
{
	WCHAR	src[32] = L"TestCallMemcpy()";
	WCHAR	dest[32] = {};

	//memcpy(dest, src, 64);

	__asm
	{
		; src ptr
		lea eax, dword ptr[src]

		; dest ptr
		lea edx, dword ptr[dest]

		; size(sizeof(WCHAR) * 32 = 64)
		push 64;

		; src
		push eax

		; dest
		push edx

		call memcpy
		add esp, 12
	}
	wprintf_s(L"src = %s\n", src);
	wprintf_s(L"dest = %s\n", dest);

	wprintf_s(L"Press any key.\n");
	_getch();
}

void TestCallIndirect()
{
	const WCHAR* wchCaption = L"Caption";
	const WCHAR* wchText = L"Text";

	// MessageBox(nullptr, wchText, wchCaption, MB_OK);

	// stdcall - 스택 16바이트 사용 -> 함수를 호출한 쪽이 아니라 함수에서 복구하게 되어있음
	// MessageBox 이름 자체가 함수 포인터
	// Win32 함수 같은 것은 .dll 프로세스 내부에 함수 테이블이 있음 -> 이름 자체가 함수 포인터
	__asm
	{
		mov eax, dword ptr[wchCaption]
		mov edx, dword ptr[wchText]
		push MB_OK
		push eax
		push edx
		push 0
		call dword ptr[MessageBox]
	}
	int a = 0;
}

int __declspec(naked) __cdecl Add_NakedCall_CDECL_LOCAL_VAR(int a, int b)
{
	// ebx, esi, edi must be preserved.
#define ARRAY_COUNT 4
	int temp[ARRAY_COUNT];


	// __LOCAL_SIZE builtin 변수 16 바이트를 선언 했지만 Memory Align 에 따라 16 바이트가 아닐 수도 있음. 미리 sp 를 확보
	// 빌트인 변수를 잘 써야한다.
	__asm
	{
		push ebp
		mov ebp, esp

		; local area
		sub esp, __LOCAL_SIZE

		; bakcup registers
		push edi

		; clear local variable
		; &temp[0] -> (ebp - 20), & temp[1] -> (ebp - 16), & temp[2] -> (ebp - 12), & temp[3] -> (ebp - 8)
		mov ecx, ARRAY_COUNT
		xor eax, eax

		lea edi, dword ptr[temp]	// 
		rep stosd

		; a + b
		mov eax, dword ptr[a]
		mov edx, dword ptr[b]
		add eax, edx

		pop edi

		mov esp, ebp
		pop ebp
		ret
	}
}


int main()
{
	CopyTest();

	int a = -1;
	int b = 0;
	int c = 0;

	c = Add_C(a, b);

	c = Add_ASM_CDECL(a, b);

	a = 3;
	b = 4;

	c = Mul_ASM(a, b);

	a = 13;
	b = 4;
	c = Div_ASM(a, b);

	TestCmpTest();

	// TestCallMemcpy();

	TestCallIndirect();

	return 0;
}