#ifndef DC_PASSWORD_H
#define DC_PASSWORD_H
int NewPassword(unsigned int numDataBits, unsigned int numChecksumBits);
int StorePasswordElement(unsigned int data, unsigned int numBits);
int GetPasswordElement(unsigned int *data, unsigned int numBits);
void GetPasswordString(char *password);
int SetPasswordString(char *password);
#endif
