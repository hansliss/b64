#ifndef B64_H
#define B64_H

int b64_encode(unsigned char *indata, int indatalen, char *result, int reslen);

int b64_decode(unsigned char *indata, int indatalen, char *result, int reslen);

#endif
