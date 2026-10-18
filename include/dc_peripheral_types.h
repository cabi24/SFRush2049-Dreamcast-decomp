/* Target SDK view: vtable slots14/18/1C/20/28, recordstride2C. */
typedef struct tDCPeripheralObject tDCPeripheralObject;
typedef struct tDCPeripheralOps {
    char pad0[0x14];
    void (*release)(tDCPeripheralObject *);
    void (*start)(tDCPeripheralObject *,void *);
    void (*stop)(tDCPeripheralObject *);
    int (*state)(tDCPeripheralObject *);
    void *unknown24;
    void (*pause)(tDCPeripheralObject *,int);
} tDCPeripheralOps;
struct tDCPeripheralObject { tDCPeripheralOps *ops; };
typedef struct tDCPeripheralConfig {
    int mode;
    unsigned int fieldC;
    int width;
    int height;
    int format;
    void *buffer;
    unsigned int bufferSize;
    int field24;
    int field28;
} tDCPeripheralConfig;
typedef struct tDCPeripheral {
    int state;
    tDCPeripheralObject *object;
    tDCPeripheralConfig config;
} tDCPeripheral;
typedef char dc_peripheral_stride[(sizeof(tDCPeripheral)==44)?1:-1];
typedef struct tDCControllerPortPrefix {char field0;unsigned char port;char unknown2[0x50-2];}tDCControllerPortPrefix;
