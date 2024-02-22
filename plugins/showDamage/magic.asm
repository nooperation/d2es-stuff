.model flat, C

EXTERN OriginalFunc:DWORD
EXTERN RenderHook:PROC

.CODE
	intercept_RenderHook PROC
		pushad
		call RenderHook
		popad
		jmp OriginalFunc
	intercept_RenderHook ENDP
END
