#ifndef __ERRNO_H_
#define __ERRNO_H_

int *__errno_location(void);
#define errno *__errno_location()

/* errno codes */
#define E2BIG		1
#define EACCES		2
#define EADDRINUSE	3
#define EAFNOSUPPORT	4
#define EAGAIN		5
#define EALREADY	6
#define EBADF		7
#define EBADMSG		8
#define EBUSY		9
#define ECANCELED	10
#define ECHILD		11
#define ECONNABORTED	12
#define ECONNREFUSED	13
#define ECONNRESET	14
#define EDEADLK		15
#define EDESTADDRREQ	16
#define EDOM		17
#define EDQUOT		18
#define EEXIST		19
#define EFAULT		20
#define EFBIG		21
#define EHOSTUNREACH	22
#define EIDRM		23
#define EILSEQ		24
#define EINPROGRESS	25
#define EINTR		26
#define EINVAL		27
#define EIO		28
#define EISCONN		29
#define EISDIR		30
#define ELOOP		31
#define EMFILE		32
#define EMLINK		33
#define EMSGSIZE	34
#define EMULTIHOP	35
#define ENAMETOOLONG	36
#define ENETDOWN	37
#define ENETRESET	38
#define ENETUNREACH	39
#define ENFILE		40
#define ENOBUFS		41
#define ENODATA		42
#define ENODEV		43
#define ENOENT		44
#define ENOEXEC		45
#define ENOLCK		46
#define ENOLINK		47
#define ENOMEM		48
#define ENOMSG		49
#define ENOPROTOOPT	50
#define ENOSPC		51
#define ENOSR		52
#define ENOSTR		53
#define ENOSYS		54
#define ENOTCONN	55
#define ENOTDIR		56
#define ENOTEMPTY	57
#define ENOTRECOVERABLE 58
#define ENOTSOCK	59
#define ENOTSUP		60
#define ENOTTY		61
#define ENXIO		62
#define EOPNOTSUPP	63
#define EOVERFLOW	64
#define EOWNERDEAD	65
#define EPERM		66
#define EPIPE		67
#define EPROTO		68
#define EPROTOTYPE	69
#define ERANGE		70
#define EROFS		71
#define ESPIPE		72
#define ESRCH		73
#define ESTALE		74
#define ETIME		75
#define ETIMEDOUT	76
#define ETXTBSY		77
#define EWOULDBLOCK	78
#define EXDEV		79

#endif
