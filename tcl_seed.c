/*
    seed encrypt,decrypt tcl package
    2008.01.21 created by dancal
    license : LGPL 

    SEED는 민간부문에서의 정보와 개인 프라이버시 등을 보호하기 위하여
    한국정보보호진흥원(구,한국정보보호센터)과 ETRI주도하에 개발된 대칭키 방식의 128비트 
    블럭암호 알고리즘으로 1999년 9월 국내 단체표준화 (TTA.KO-12.0004)를 완료하였습니다.
    현재, 한국정보보호진흥원에서는 SEED의 국제 표준화를 추진중에 있습니다.
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

    // 16의 배수에 맞는 사이즈 구하기
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
