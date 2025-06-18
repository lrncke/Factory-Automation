/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III                                        *
 *   module name: enet.h                                             *
 *        author: Christian Sommerfeld                               *
 *          date: 2017-04-04                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *           1.1 flags for fcntl added                               *
 *                                                                   *
 *                                                                   *                                                                *
 *-------------------------------------------------------------------*/

#ifndef _ENET_H_
#define _ENET_H_

#define ip_data 32

typedef long long time_t;
typedef long suseconds_t;


struct timeval {
       time_t      tv_sec;      /* seconds */
       suseconds_t tv_usec;     /* and microseconds */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET		0xfff		/* options for socket level */
#define INADDR_ANY		((unsigned int) 0x00000000)

/*
 * address families
 */
#define	AF_UNSPEC			0		/* unspecified */
#define	AF_LOCAL			1		/* local to host (pipes, portals) */
#define	AF_UNIX				AF_LOCAL/* backward compatibility */
#define	AF_INET				2		/* internetwork: UDP, TCP, etc. */
#define	AF_IMPLINK			3		/* arpanet imp addresses */
#define	AF_PUP				4		/* pup protocols: e.g. BSP */
#define	AF_CHAOS			5		/* mit CHAOS protocols */
#define	AF_NS				6		/* XEROX NS protocols */
#define	AF_ISO				7		/* ISO protocols */
#define	AF_ECMA				8		/* European computer manufacturers */
#define	AF_DATAKIT			9		/* datakit protocols */
#define	AF_CCITT			10		/* CCITT protocols, X.25 etc */
#define	AF_SNA				11		/* IBM SNA */
#define AF_DECnet			12		/* DECnet */
#define AF_DLI				13		/* DEC Direct data link interface */
#define AF_LAT				14		/* LAT */
#define	AF_HYLINK			15		/* NSC Hyperchannel */
#define	AF_APPLETALK		16		/* Apple Talk */
#define	AF_ROUTE			17		/* Internal Routing Protocol */
#define	AF_LINK				18		/* Link layer interface */
#define	pseudo_AF_XTP		19		/* eXpress Transfer Protocol (no AF) */
#define	AF_COIP				20		/* connection-oriented IP, aka ST II */
#define	AF_CNT				21		/* Computer Network Technology */
#define pseudo_AF_RTIP		22		/* Help Identify RTIP packets */
#define	AF_IPX				23		/* Novell Internet Protocol */
#define	AF_SIP				24		/* Simple Internet Protocol */
#define	pseudo_AF_PIP		25		/* Help Identify PIP packets */
#define	AF_ISDN				26		/* Integrated Services Digital Network*/
#define	AF_E164				AF_ISDN	/* CCITT E.164 recommendation */
#define	pseudo_AF_KEY		27		/* Internal key-management function */
#define	AF_INET6			28		/* IPv6 */
#define	AF_NATM				29		/* native ATM access */
#define	AF_ATM				30		/* ATM */
#define pseudo_AF_HDRCMPLT 	31	/* Used by BPF to not rewrite headers
								 * in interface output routine */
#define	AF_NETGRAPH			32			/* Netgraph sockets */
#define	AF_MAX				33

/*
 * Types
 */
#define	SOCK_STREAM			1		/* stream socket */
#define	SOCK_DGRAM			2		/* datagram socket */
#define	SOCK_RAW			3		/* raw-protocol interface */
#define	SOCK_RDM			4		/* reliably-delivered message */
#define	SOCK_SEQPACKET		5		/* sequenced packet stream */

/*
 * Option flags per-socket.
 */
#define	SO_DEBUG	    	0x0001		/* turn on debugging info recording */
#define	SO_ACCEPTCONN		0x0002		/* socket has had listen() */
#define	SO_REUSEADDR		0x0004		/* allow local address reuse */
#define	SO_KEEPALIVE		0x0008		/* keep connections alive */
#define	SO_DONTROUTE		0x0010		/* just use interface addresses */
#define	SO_BROADCAST		0x0020		/* permit sending of broadcast msgs */
#define	SO_USELOOPBACK		0x0040		/* bypass hardware when possible */
#define	SO_LINGER	    	0x0080		/* linger on close if data present */
#define	SO_OOBINLINE		0x0100		/* leave received OOB data in line */
#define	SO_REUSEPORT		0x0200		/* allow local address & port reuse */
#define	SO_TIMESTAMP		0x0400		/* timestamp received dgram traffic */
#define	SO_ACCEPTFILTER		0x1000		/* there is an accept filter */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF			0x1001		/* send buffer size */
#define SO_RCVBUF			0x1002		/* receive buffer size */
#define SO_SNDLOWAT			0x1003		/* send low-water mark */
#define SO_RCVLOWAT			0x1004		/* receive low-water mark */
#define SO_SNDTIMEO			0x1005		/* send timeout */
#define SO_RCVTIMEO			0x1006		/* receive timeout */
#define	SO_ERROR			0x1007		/* get error status and clear */
#define	SO_TYPE				0x1008		/* get socket type */

/*
 * flags for Enet_recv
 */
#define	MSG_OOB				0x1			/* process out-of-band data */
#define	MSG_PEEK			0x2			/* peek at incoming message */
#define	MSG_DONTROUTE		0x4		/* send without using routing tables */
#define	MSG_EOR				0x8			/* data completes record */
#define	MSG_TRUNC			0x10		/* data discarded before delivery */
#define	MSG_CTRUNC			0x20		/* control data lost before delivery */
#define	MSG_WAITALL			0x40		/* wait for full request or error */
#define	MSG_DONTWAIT		0x80	/* this message should be nonblocking */
#define	MSG_EOF				0x100		/* data completes connection */
#define MSG_COMPAT      	0x8000	/* used in sendit() */

/*
 * User-settable options (used with setsockopt).
 */
#define	TCP_NODELAY			0x01	/* don't delay send to coalesce packets */
#define	TCP_MAXSEG			0x02	/* set maximum segment size */
#define TCP_NOPUSH			0x04	/* don't push last block of write */
#define TCP_NOOPT			0x08	/* don't use TCP options */

/*
 * Protocols (RFC 1700)
 */
#define	IPPROTO_IP			0		/* dummy for IP */
#define	IPPROTO_HOPOPTS		0		/* IP6 hop-by-hop options */
#define	IPPROTO_ICMP		1		/* control message protocol */
#define	IPPROTO_IGMP		2		/* group mgmt protocol */
#define	IPPROTO_GGP			3		/* gateway^2 (deprecated) */
#define IPPROTO_IPV4		4 		/* IPv4 encapsulation */
#define IPPROTO_IPIP		IPPROTO_IPV4	/* for compatibility */
#define	IPPROTO_TCP			6		/* tcp */
#define	IPPROTO_ST			7		/* Stream protocol II */
#define	IPPROTO_EGP			8		/* exterior gateway protocol */
#define	IPPROTO_PIGP		9		/* private interior gateway */
#define	IPPROTO_RCCMON		10		/* BBN RCC Monitoring */
#define	IPPROTO_NVPII		11		/* network voice protocol*/
#define	IPPROTO_PUP			12		/* pup */
#define	IPPROTO_ARGUS		13		/* Argus */
#define	IPPROTO_EMCON		14		/* EMCON */
#define	IPPROTO_XNET		15		/* Cross Net Debugger */
#define	IPPROTO_CHAOS		16		/* Chaos*/
#define	IPPROTO_UDP			17		/* user datagram protocol */
#define	IPPROTO_MUX			18		/* Multiplexing */
#define	IPPROTO_MEAS		19		/* DCN Measurement Subsystems */
#define	IPPROTO_HMP			20		/* Host Monitoring */
#define	IPPROTO_PRM			21		/* Packet Radio Measurement */
#define	IPPROTO_IDP			22		/* xns idp */
#define	IPPROTO_TRUNK1		23		/* Trunk-1 */
#define	IPPROTO_TRUNK2		24		/* Trunk-2 */
#define	IPPROTO_LEAF1		25		/* Leaf-1 */
#define	IPPROTO_LEAF2		26		/* Leaf-2 */
#define	IPPROTO_RDP			27		/* Reliable Data */
#define	IPPROTO_IRTP		28		/* Reliable Transaction */
#define	IPPROTO_TP			29 		/* tp-4 w/ class negotiation */
#define	IPPROTO_BLT			30		/* Bulk Data Transfer */
#define	IPPROTO_NSP			31		/* Network Services */
#define	IPPROTO_INP			32		/* Merit Internodal */
#define	IPPROTO_SEP			33		/* Sequential Exchange */
#define	IPPROTO_3PC			34		/* Third Party Connect */
#define	IPPROTO_IDPR		35		/* InterDomain Policy Routing */
#define	IPPROTO_XTP			36		/* XTP */
#define	IPPROTO_DDP			37		/* Datagram Delivery */
#define	IPPROTO_CMTP		38		/* Control Message Transport */
#define	IPPROTO_TPXX		39		/* TP++ Transport */
#define	IPPROTO_IL			40		/* IL transport protocol */
#define	IPPROTO_IPV6		41		/* IP6 header */
#define	IPPROTO_SDRP		42		/* Source Demand Routing */
#define	IPPROTO_ROUTING		43		/* IP6 routing header */
#define	IPPROTO_FRAGMENT	44		/* IP6 fragmentation header */
#define	IPPROTO_IDRP		45		/* InterDomain Routing*/
#define	IPPROTO_RSVP		46 		/* resource reservation */
#define	IPPROTO_GRE			47		/* General Routing Encap. */
#define	IPPROTO_MHRP		48		/* Mobile Host Routing */
#define	IPPROTO_BHA			49		/* BHA */
#define	IPPROTO_ESP			50		/* IP6 Encap Sec. Payload */
#define	IPPROTO_AH			51		/* IP6 Auth Header */
#define	IPPROTO_INLSP		52		/* Integ. Net Layer Security */
#define	IPPROTO_SWIPE		53		/* IP with encryption */
#define	IPPROTO_NHRP		54		/* Next Hop Resolution */
/* 55-57: Unassigned */
#define	IPPROTO_ICMPV6		58		/* ICMP6 */
#define	IPPROTO_NONE		59		/* IP6 no next header */
#define	IPPROTO_DSTOPTS		60		/* IP6 destination option */
#define	IPPROTO_AHIP		61		/* any host internal protocol */
#define	IPPROTO_CFTP		62		/* CFTP */
#define	IPPROTO_HELLO		63		/* "hello" routing protocol */
#define	IPPROTO_SATEXPAK	64		/* SATNET/Backroom EXPAK */
#define	IPPROTO_KRYPTOLAN	65		/* Kryptolan */
#define	IPPROTO_RVD			66		/* Remote Virtual Disk */
#define	IPPROTO_IPPC		67		/* Pluribus Packet Core */
#define	IPPROTO_ADFS		68		/* Any distributed FS */
#define	IPPROTO_SATMON		69		/* Satnet Monitoring */
#define	IPPROTO_VISA		70		/* VISA Protocol */
#define	IPPROTO_IPCV		71		/* Packet Core Utility */
#define	IPPROTO_CPNX		72		/* Comp. Prot. Net. Executive */
#define	IPPROTO_CPHB		73		/* Comp. Prot. HeartBeat */
#define	IPPROTO_WSN			74		/* Wang Span Network */
#define	IPPROTO_PVP			75		/* Packet Video Protocol */
#define	IPPROTO_BRSATMON	76		/* BackRoom SATNET Monitoring */
#define	IPPROTO_ND			77		/* Sun net disk proto (temp.) */
#define	IPPROTO_WBMON		78		/* WIDEBAND Monitoring */
#define	IPPROTO_WBEXPAK		79		/* WIDEBAND EXPAK */
#define	IPPROTO_EON			80		/* ISO cnlp */
#define	IPPROTO_VMTP		81		/* VMTP */
#define	IPPROTO_SVMTP		82		/* Secure VMTP */
#define	IPPROTO_VINES		83		/* Banyon VINES */
#define	IPPROTO_TTP			84		/* TTP */
#define	IPPROTO_IGP			85		/* NSFNET-IGP */
#define	IPPROTO_DGP			86		/* dissimilar gateway prot. */
#define	IPPROTO_TCF			87		/* TCF */
#define	IPPROTO_IGRP		88		/* Cisco/GXS IGRP */
#define	IPPROTO_OSPFIGP		89		/* OSPFIGP */
#define	IPPROTO_SRPC		90		/* Strite RPC protocol */
#define	IPPROTO_LARP		91		/* Locus Address Resoloution */
#define	IPPROTO_MTP			92		/* Multicast Transport */
#define	IPPROTO_AX25		93		/* AX.25 Frames */
#define	IPPROTO_IPEIP		94		/* IP encapsulated in IP */
#define	IPPROTO_MICP		95		/* Mobile Int.ing control */
#define	IPPROTO_SCCSP		96		/* Semaphore Comm. security */
#define	IPPROTO_ETHERIP		97		/* Ethernet IP encapsulation */
#define	IPPROTO_ENCAP		98		/* encapsulation header */
#define	IPPROTO_APES		99		/* any private encr. scheme */
#define	IPPROTO_GMTP		100		/* GMTP*/
#define	IPPROTO_IPCOMP		108		/* payload compression (IPComp) */
#define	IPPROTO_PIM			103		/* Protocol Independent Mcast */
#define	IPPROTO_PGM			113		/* PGM */
#define IPPROTO_SCTP		132		/* SCTP (RFC2960) */

/*
 * flags for fcntl
 */
#define O_RDONLY     (1<<0)   /* Open for reading only */
#define O_WRONLY     (1<<1)   /* Open for writing only */
#define O_RDWR       (O_RDONLY|O_WRONLY) /* Open for reading and writing */

/* File access mode mask */
#define O_ACCMODE    (O_RDONLY|O_RDWR|O_WRONLY)

/* open() mode flags */
#define O_CREAT      (1<<3)    /* Create file it it does not exist */
#define O_EXCL       (1<<4)    /* Exclusive use */
#define O_NOCTTY     (1<<5)    /* Do not assign a controlling terminal */
#define O_TRUNC      (1<<6)    /* Truncate */

/* File status flags used for open() and fcntl() */
#define O_APPEND     (1<<7)    /* Set append mode */
#define O_DSYNC      (1<<8)    /* Synchronized I/O data integrity writes */
#define O_NONBLOCK   (1<<9)    /* No delay */
#define O_RSYNC      (1<<10)   /* Synchronized read I/O */
#define O_SYNC       (1<<11)   /* Synchronized I/O file integrity writes */

/* fcntl() command values */
#define F_DUPFD      (1<<0)    /* Duplicate file descriptor */
#define F_GETFD      (1<<1)    /* Get file descriptor flags */
#define F_SETFD      (1<<2)    /* Set file descriptor flags */
#define F_GETFL      (1<<3)    /* Get file status flags */
#define F_SETFL      (1<<4)    /* Set file status flags */
#define F_GETLK      (1<<5)    /* Get record locking information */
#define F_SETLK      (1<<6)    /* Set record locking information */
#define F_SETLKW     (1<<7)    /* Set record locking info; wait if blocked */

#endif
