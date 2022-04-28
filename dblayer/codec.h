#include "tbl.h"

int
EncodeInt(int i, Byte *bytes);
int
DecodeInt(Byte *bytes);

int
EncodeShort(short s, Byte *bytes) ;
short
DecodeShort(Byte *bytes);

int
EncodeDouble(double s, Byte *bytes) ;
double
DecodeDouble(Byte *bytes);

int
EncodeLong(long long l, Byte *bytes);

long long
DecodeLong(Byte *bytes);

int
EncodeCString(char *str, Byte *bytes, int max_len);

int
DecodeCString(Byte *bytes, char *str, int max_len);
