#ifndef __EFI_PROTOCOL_STIP_H
#define __EFI_PROTOCOL_STIP_H	1

#include "../efi-types.h"

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID \
	{ 0x387477c1, 0x69c7, 0x11d2, \
	 { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b }}

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct {
	UINT16 ScanCode;
	CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI *EFI_INPUT_RESET) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
	IN BOOLEAN ExtendedVerification);
	
typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
	OUT EFI_INPUT_KEY *Key);

/*typedef*/ struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	EFI_INPUT_RESET		Reset;
	EFI_INPUT_READ_KEY	ReadKeyStroke;
	EFI_EVENT			WaitForKey;
};// EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

#endif /* __EFI_PROTOCOL_STIP_H */