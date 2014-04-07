//camac.h
#define GPD_TYPE 40000



// The IOCTL function codes from 0x800 to 0xFFF are for customer use.


#define IOCTL_GPD_READ_PORT_UCHAR \
    CTL_CODE( GPD_TYPE, 0x900, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_PORT_USHORT \
    CTL_CODE( GPD_TYPE, 0x901, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_READ_PORT_ULONG \
    CTL_CODE( GPD_TYPE, 0x902, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_GPD_WRITE_PORT_UCHAR \
    CTL_CODE(GPD_TYPE,  0x910, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_PORT_USHORT \
    CTL_CODE(GPD_TYPE,  0x911, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_GPD_WRITE_PORT_ULONG \
    CTL_CODE(GPD_TYPE,  0x912, METHOD_BUFFERED, FILE_WRITE_ACCESS)



#define IOCTL_SENDF \
    CTL_CODE(GPD_TYPE,  0x913, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_SENDNAF \
    CTL_CODE(GPD_TYPE,  0x914, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_SENDDNAF \
    CTL_CODE(GPD_TYPE,  0x915, METHOD_BUFFERED, FILE_WRITE_ACCESS)


#define IOCTL_READD \
    CTL_CODE(GPD_TYPE,  0x916, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_READCSR \
    CTL_CODE(GPD_TYPE,  0x917, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_READENCL \
    CTL_CODE(GPD_TYPE,  0x918, METHOD_BUFFERED, FILE_WRITE_ACCESS)



#define IOCTL_LISTPTRRESET \
    CTL_CODE(GPD_TYPE,  0x919, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_LISTDNAF \
    CTL_CODE(GPD_TYPE,  0x91a, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_LISTSTART \
    CTL_CODE(GPD_TYPE,  0x91b, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_LISTREADD \
    CTL_CODE(GPD_TYPE,  0x91c, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_DATAPTRRESET \
    CTL_CODE(GPD_TYPE,  0x91d, METHOD_BUFFERED, FILE_WRITE_ACCESS)


#define IOCTL_SHOWLISTPTR \
    CTL_CODE(GPD_TYPE,  0x91e, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_SHOWDATAPTR \
    CTL_CODE(GPD_TYPE,  0x91f, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_SHOWACTUALLISTPTR \
    CTL_CODE(GPD_TYPE,  0x920, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_SHOWACTUALDATAPTR \
    CTL_CODE(GPD_TYPE,  0x921, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_GPIBREADY \
    CTL_CODE(GPD_TYPE,  0x922, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_LISTREADY \
    CTL_CODE(GPD_TYPE,  0x923, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_SHOWSTRINGPTR \
    CTL_CODE(GPD_TYPE,  0x924, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_SHOWACTUALSTRINGPTR \
    CTL_CODE(GPD_TYPE,  0x925, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_GPIBREADSTRING \
    CTL_CODE(GPD_TYPE,  0x926, METHOD_BUFFERED, FILE_READ_ACCESS)

typedef struct  _GENPORT_WRITE_INPUT_BYTE {
    ULONG   PortNumber;     // Port # to write to
    UCHAR   CharData;
}   GENPORT_WRITE_INPUT_BYTE;

typedef struct  _GENPORT_WRITE_INPUT_SHORT {
    ULONG   PortNumber;     // Port # to write to
    USHORT   ShortData;
}   GENPORT_WRITE_INPUT_SHORT;

typedef struct  _GENPORT_WRITE_INPUT_LONG {
    ULONG   PortNumber;     // Port # to write to
    ULONG   LongData;
}   GENPORT_WRITE_INPUT_LONG;

 
