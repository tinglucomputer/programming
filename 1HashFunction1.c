#include "defination1.h"


unsigned int RSHash(const char* str, unsigned int length)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   unsigned int i    = 0;
   
   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash = hash * a + (*str);
      a    = a * b;
   }

   return hash % mb ;
}


unsigned int JSHash(const char* str, unsigned int length)
{
   unsigned int hash = 1315423911;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash ^= ((hash << 5) + (*str) + (hash >> 2));
   }

   return hash % mb;
}

unsigned int PJWHash(const char* str, unsigned int length)
{
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   const unsigned int HighBits          =
                      (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash = 0;
   unsigned int test = 0;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash = (hash << OneEighth) + (*str);

      if ((test = hash & HighBits) != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash % mb;
}

unsigned int ELFHash(const char* str, unsigned int length)
{
   unsigned int hash = 0;
   unsigned int x    = 0;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash = (hash << 4) + (*str);

      if ((x = hash & 0xF0000000L) != 0)
      {
         hash ^= (x >> 24);
      }

      hash &= ~x;
   }

   return hash % mb;
}

unsigned int BKDRHash(const char* str, unsigned int length)
{
   unsigned int seed = 131; 
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash = (hash * seed) + (*str);
   }

   return hash % mb;
}

unsigned int SDBMHash(const char* str, unsigned int length)
{
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash = (*str) + (hash << 6) + (hash << 16) - hash;
   }

   return hash % mb;
}

unsigned int DJBHash(const char* str, unsigned int length)
{
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash = ((hash << 5) + hash) + (*str);
   }

   return hash % mb;
}

unsigned int DEKHash(const char* str, unsigned int length)
{
   unsigned int hash = length;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
   }

   return hash % mb;
}


unsigned int APHash(const char* str, unsigned int length)
{
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   }

   return hash % mb;
}


unsigned int THash(const char* str,unsigned int length)
{
	
	unsigned int hash = 1315423911;
   unsigned int i    = 0;

   for (i = 0; i < length && *str; ++str, ++i)
   {
      hash ^= ((hash << 5) + (*str) + (hash >> 2));
   }

   return (2 ^ ( sizeof(unsigned int) * 8) - hash) % mb;
}






