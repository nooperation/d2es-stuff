.model flat, C

EXTERN ReturnPoint_BuildItemPropertyLine:DWORD
EXTERN MyBuildItemPropertyLine:PROC

.CODE
	intercept_BuildItemPropertyLine PROC
		; BuildItemPropertyLine is fastcall, MyBuildItemPropertyLine is cdecl. Put those two args on the stack instead of ecx/edx
		push esi ; esi contains the value of edx
		push ecx
		call MyBuildItemPropertyLine

		add esp, 18h
		jmp ReturnPoint_BuildItemPropertyLine

	intercept_BuildItemPropertyLine ENDP
END
