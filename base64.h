#define zend_bool   int

#define emalloc(size)       malloc(size)
#define efree(ptr)          free(ptr)
#define _emalloc(size)      malloc(size)
#define _efree(ptr)         free(ptr)

#define ecalloc(nmemb, size)                       calloc((nmemb), (size))
#define safe_emalloc(nmemb, size, offset)          malloc((nmemb) * (size) + (offset))

unsigned char *php_base64_encode(const unsigned char *, int, int *);
unsigned char *php_base64_decode_ex(const unsigned char *, int, int *, zend_bool);
unsigned char *php_base64_decode(const unsigned char *, int, int *);
