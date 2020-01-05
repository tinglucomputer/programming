#define mB 2300000 
#define mb 18350000
#define len  30 

unsigned int RSHash(const char* str,unsigned int length);
unsigned int JSHash(const char* str,unsigned int length);
unsigned int PJWHash(const char* str,unsigned int length);
unsigned int ELFHash(const char* str,unsigned int length);
unsigned int BKDRHash(const char* str,unsigned int length);
unsigned int SDBMHash(const char* str,unsigned int length);
unsigned int DJBHash(const char* str,unsigned int length);
unsigned int DEKHash(const char* str,unsigned int length);
unsigned int APHash(const char* str,unsigned int length);
unsigned int THash(const char* str,unsigned int length);
