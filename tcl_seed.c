/*
    seed encrypt,decrypt tcl package
    2008.01.21 created by dancal
    license : LGPL 

    SEED�� �ΰ��ι������� ������ ���� �����̹��� ���� ��ȣ�ϱ� ���Ͽ�
    �ѱ�������ȣ�����(��,�ѱ�������ȣ����)�� ETRI�ֵ��Ͽ� ���ߵ� ��ĪŰ ����� 128��Ʈ 
    ����ȣ �˰������� 1999�� 9�� ���� ��üǥ��ȭ (TTA.KO-12.0004)�� �Ϸ��Ͽ����ϴ�.
    ����, �ѱ�������ȣ����������� SEED�� ���� ǥ��ȭ�� �����߿� �ֽ��ϴ�.
    http://www.securitytechnet.com/std-algorithm/block.html
*/

#include <tcl.h>
#include <math.h>
#include <malloc.h>
#include <errno.h>

#include "base64.h"
#include "seedsrc/SEED_KISA.h"

#define SEED_BLK_LEN    16

static int TCL_Seed_encrypt( ClientData data, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	if ( objc < 2 ) {
	    Tcl_WrongNumArgs(interp, 1, objv, "Wrong args\nex)pkg $string");
    	return TCL_OK;
    }

    BYTE pUserKey[SEED_BLK_LEN];
    BYTE pUserStr[SEED_BLK_LEN];
    DWORD pdwRoundKey[32];      // Round keys for encryption or decryption

    int nStrKeyLen;
    char *pStrKey = Tcl_GetStringFromObj( objv[1], &nStrKeyLen );
    if ( nStrKeyLen != SEED_BLK_LEN) {
    Tcl_WrongNumArgs(interp, 1, objv, "Wrong KeyLength 16Byte");
    	return TCL_ERROR;
    }

	int nStringLen;
    char *pString = Tcl_GetStringFromObj( objv[2], &nStringLen );
    if ( nStringLen == 0 ) {
    	return TCL_ERROR;
    }

    // 16�� ����� �´� ������ ���ϱ�
    int dwSize;
    if ((nStringLen % SEED_BLK_LEN) == 0)
        dwSize = nStringLen;
    else
        dwSize = nStringLen + (SEED_BLK_LEN - (nStringLen % SEED_BLK_LEN));

	memset(pUserKey,0x00,SEED_BLK_LEN);
    memcpy(pUserKey,pStrKey,SEED_BLK_LEN);
    SeedRoundKey(pdwRoundKey,pUserKey);

    char *pResult= (char*)malloc(sizeof(char)*(dwSize)+1);
    memset(pResult,0x00,dwSize);
    memcpy(pResult,pString,nStringLen);

	int i;
    for(i=0;i<dwSize;i+=SEED_BLK_LEN) {
        SeedEncrypt(pResult+i,pdwRoundKey);
	}
	pResult[dwSize] = '\0';

	int len;
    char *pData = php_base64_encode( pResult, dwSize, &len );
    Tcl_SetObjResult(interp, Tcl_NewStringObj( pData, len ) );

	free(pData);
	free(pResult);

	return TCL_OK;
}

static int TCL_Seed_decrypt( ClientData data, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {

	if ( objc < 2 ) {
	    Tcl_WrongNumArgs(interp, 1, objv, "Wrong args\nex)pkg $string");
    	return TCL_OK;
    }

    BYTE pUserKey[SEED_BLK_LEN];
    BYTE pUserStr[SEED_BLK_LEN];
    DWORD pdwRoundKey[32];      // Round keys for encryption or decryption

	int nStrKeyLen;
    char *pStrKey = Tcl_GetStringFromObj( objv[1], &nStrKeyLen );
    if ( nStrKeyLen != SEED_BLK_LEN) {
    Tcl_WrongNumArgs(interp, 1, objv, "Wrong KeyLength 16Byte");
    	return TCL_ERROR;
    }

	int nStringLenT;
    char *pStrTmp = Tcl_GetStringFromObj( objv[2], &nStringLenT );
	int nStringLen;
    char *pString = php_base64_decode( pStrTmp, nStringLenT, &nStringLen );
	if ( nStringLen == 0 ) {
    	return TCL_ERROR;
    }

	memset(pUserKey,0x00,SEED_BLK_LEN);
    memcpy(pUserKey,pStrKey,SEED_BLK_LEN);
    SeedRoundKey(pdwRoundKey,pUserKey);

    char *pResult= (char*)malloc(sizeof(char)*(nStringLen)+1);
    memset(pResult,0x00,nStringLen);
	memcpy(pResult,pString,nStringLen);

	int i;
    for(i=0;i<nStringLen;i+=SEED_BLK_LEN){
        SeedDecrypt(pResult+i,pdwRoundKey);
	}
	
	Tcl_SetObjResult(interp, Tcl_NewStringObj(pResult, strlen(pResult)) );

    free(pResult);
    free(pString);

    return TCL_OK;
}

//TCL PACKAGE INIT
int Seed_Init(Tcl_Interp* interp) {

	Tcl_CreateObjCommand(interp, "seed::encrypt", TCL_Seed_encrypt, NULL, NULL);
    Tcl_CreateObjCommand(interp, "seed::decrypt", TCL_Seed_decrypt, NULL, NULL);

    Tcl_PkgProvide(interp, "seed", "1.0");

    return TCL_OK;
}
