/* Copyright (c) 2018 Griefer@Work                                            *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement in the product documentation would be  *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
#ifndef GUARD_KERNEL_SRC_CORE_FAT_H
#define GUARD_KERNEL_SRC_CORE_FAT_H 1

#include <hybrid/compiler.h>
#include <kos/types.h>
#include <hybrid/host.h>
#include <hybrid/align.h>
#include <sched/rwlock.h>
#include <fs/node.h>
#include <fs/iomode.h>
#include <endian.h>

DECL_BEGIN

/* NOTE: This implementation uses the absolute on-disk location of a
 *       FAT directory entry (The `FatFile' structure) as `ino_t'
 *       Here, the `FatFile' that is actually describing the file is
 *       meant, meaning that LFN-entries are _NOT_ used as index, but
 *       the actual file entry following thereafter! */


#define FAT12 0
#define FAT16 1
#define FAT32 2
typedef u16 FatType; /* FAT filesystem type (One of `FAT12', `FAT16' or `FAT32'). */

#define FAT12_MAXCLUSTERS 0xff4      /* 4084 */
#define FAT16_MAXCLUSTERS 0xfff4     /* 65524 */
#define FAT32_MAXCLUSTERS 0xfffffff4 /* 4294967284 */


/* File attribute flags for `FatFile::f_attr' */
#define ATTR_READONLY      0x01
#define ATTR_HIDDEN        0x02
#define ATTR_SYSTEM        0x04
#define ATTR_VOLUMEID      0x08
#define ATTR_DIRECTORY     0x10
#define ATTR_ARCHIVE       0x20
#define ATTR_DEVICE        0x40
/*                         0x80 */
#define ATTR_LONGFILENAME (ATTR_READONLY|ATTR_HIDDEN|ATTR_SYSTEM|ATTR_VOLUMEID)


typedef struct PACKED {
    union PACKED {
        struct PACKED {
            unsigned int ft_sec  : 5;
            unsigned int ft_min  : 6;
            unsigned int ft_hour : 5;
        };
        u16              ft_sum;
    };
} FileTime;

typedef struct PACKED {
    union PACKED {
        struct PACKED {
            unsigned int fd_day   : 5;
            unsigned int fd_month : 4;
            unsigned int fd_year  : 7;
        };
        u16              fd_sum;
    };
} FileDate;

typedef struct PACKED {
    u8       fc_sectenth; /* Creation time in 10ms resolution (0-199). */
    FileTime fc_time;     /* Creation time. */
    FileDate fc_date;     /* Creation date. */
} FileCreationTime;

typedef struct PACKED {
    FileTime fc_time;     /* Modification time. */
    FileDate fc_date;     /* Modification date. */
} FileModificationTime;

typedef FileDate FileAccessedTime;



typedef struct PACKED {
    /* FAT directory file entry */
    union PACKED {
        struct PACKED {
            union PACKED {
                struct PACKED {
                     union PACKED {
#define MARKER_DIREND             0x00           /* End of directory. */
#define MARKER_IS0XE5             0x05           /* Character: First character is actually 0xe5. */
#define MARKER_UNUSED             0xe5           /* Unused entry (ignore). */
                         u8       f_marker;      /* Special directory entry marker (One of `MARKER_*'). */
                         char     f_name[8];     /* Short (8-character) filename. */
                     };
                     char         f_ext[3];      /* File extension. */
                };
                char              f_nameext[11]; /* Name+extension. */
            };
            u8                    f_attr;        /* File attr. */
/* https://en.wikipedia.org/wiki/8.3_filename
 * NOTE: After testing, the flags specified by that link are wrong.
 *       >> The following lowercase flags are correct though! */
#define NTFLAG_NONE               0x00           /* Normal flags. */
#define NTFLAG_LOWBASE            0x08           /* Lowercase basename. */
#define NTFLAG_LOWEXT             0x10           /* Lowercase extension. */
            u8                    f_ntflags;     /* NT Flags (Set of `NTFLAG_*'). */
            FileCreationTime      f_ctime;       /* Creation time. */
            FileAccessedTime      f_atime;       /* Last access time (or rather date...). */
            le16                  f_clusterhi;   /* High 2 bytes of the file's cluster. */
            FileModificationTime  f_mtime;       /* Last modification time. */
            le16                  f_clusterlo;   /* Lower 2 bytes of the file's cluster. */
            le32                  f_size;        /* File size. */
        };
/* Sizes of the three name portions. */
#define LFN_NAME1      5
#define LFN_NAME2      6
#define LFN_NAME3      2
#define LFN_NAME      (LFN_NAME1+LFN_NAME2+LFN_NAME3)
        struct PACKED {
            /* Long filename entry. */
#define LFN_SEQNUM_MIN        0x01
#define LFN_SEQNUM_MAX        0x14
#define LFN_SEQNUM_MAXCOUNT ((LFN_SEQNUM_MAX-LFN_SEQNUM_MIN)+1)
            u8                    lfn_seqnum;            /* Sequence number (One of `LFN_SEQNUM_*'). */
            le16                  lfn_name_1[LFN_NAME1]; /* First LFN part */
            u8                    lfn_attr;              /* Attributes (always `ATTR_LONGFILENAME') */
            u8                    lfn_type;              /* Long directory entry type (set to ZERO(0)) */
            u8                    lfn_csum;              /* Checksum of DOS filename (s.a.: `fat_LFNchksum'). */
            le16                  lfn_name_2[LFN_NAME2]; /* Second LFN part */
            le16                  lfn_clus;              /* First cluster (Always 0x0000). */
            le16                  lfn_name_3[LFN_NAME3]; /* Third LFN part */
        };
    };
} FatFile;


typedef struct PACKED {
    /* FAT BIOS Parameter Block (common header) */
    u8             bpb_jmp[3];              /* Jump instructions (executable). */
    char           bpb_oem[8];              /* OEM identifier */
    le16           bpb_bytes_per_sector;    /* The number of Bytes per sector. */
    u8             bpb_sectors_per_cluster; /* Number of sectors per cluster. */
    le16           bpb_reserved_sectors;    /* Number of reserved sectors. */
    u8             bpb_fatc;                /* Number of File Allocation Tables (FAT's) on the storage media (1..4). */
    le16           bpb_maxrootsize;         /* [!FAT32] Max number of entries in the root directory. */
    le16           bpb_shortsectorc;        /* The total sectors in the logical volume (If 0, use `bpb_numheads' instead). */
    u8             bpb_mediatype;           /* Indicates the media descriptor type. */
    le16           bpb_sectors_per_fat;     /* [!FAT32] Number of sectors per FAT. */
    le16           bpb_sectors_per_track;   /* Number of sectors per track. */
    le16           bpb_numheads;            /* Number of heads or sides on the storage media. */
    le32           bpb_hiddensectors;       /* Absolute sector address of the fat header (lba of the fat partition). */
    le32           bpb_longsectorc;         /* Large amount of sector on media (Used for more than `65535' sectors) */
} BiosParameterBlock;

typedef struct PACKED {
    BiosParameterBlock f16_bpb;           /* BIOS Parameter Block. */
    u8                 f16_driveno;       /* Drive number. The value here should be identical to the value returned by BIOS interrupt 0x13,
                                           * or passed in the DL register; i.e. 0x00 for a floppy disk and 0x80 for hard disks.
                                           * This number is useless because the media is likely to be moved to another
                                           * machine and inserted in a drive with a different drive number. */
    u8                 f16_ntflags;       /* Windows NT Flags. (Set to 0) */
    u8                 f16_signature;     /* Signature (Must be 0x28 or 0x29). */
    le32               f16_volid;         /* VolumeID ~Serial~ number. Used for tracking volumes between computers. */
    char               f16_label[11];     /* Volume label string. This field is padded with spaces. */
    char               f16_sysname[8];    /* System identifier string. This field is a string representation of the FAT file system type.
                                           * It is padded with spaces. The spec says never to trust the contents of this string for any use. */
    u8                 f16_bootcode[448]; /* Boot code. */
    u8                 f16_bootsig[2];    /* Bootable partition signature (0x55, 0xAA). */
} Fat16DiskHeader;

typedef struct PACKED {
    /* FAT32 Extended boot record. */
    BiosParameterBlock f32_bpb;             /* BIOS Parameter Block. */
    le32               f32_sectors_per_fat; /* Number of sectors per FAT. */
    le16               f32_flags;           /* Flags. */
    le16               f32_version;         /* FAT version number. The high byte is the major version and the low byte is the minor version. FAT drivers should respect this field. */
    le32               f32_root_cluster;    /* The cluster number of the root directory. Often this field is set to 2. */
    le16               f32_fsinfo_cluster;  /* The sector number of the FSInfo structure. */
    le16               f32_backup_cluster;  /* The sector number of the backup boot sector. */
    u8                 f32_set2zero[12];    /* Reserved. When the volume is formated these bytes should be zero. */
    u8                 f32_driveno;         /* Drive number. The value here should be identical to the value returned by BIOS interrupt 0x13,
                                             * or passed in the DL register; i.e. 0x00 for a floppy disk and 0x80 for hard disks.
                                             * This number is useless because the media is likely to be moved to another
                                             * machine and inserted in a drive with a different drive number. */
    u8                 f32_ntflags;         /* Windows NT Flags. (Set to 0) */
    u8                 f32_signature;       /* Signature (Must be 0x28 or 0x29). */
    le32               f32_volid;           /* VolumeID ~Serial~ number. Used for tracking volumes between computers. */
    char               f32_label[11];       /* Volume label string. This field is padded with spaces. */
    char               f32_sysname[8];      /* System identifier string. This field is a string representation of the FAT file system type.
                                             * It is padded with spaces. The spec says never to trust the contents of this string for any use. */
    u8                 f32_bootcode[420];   /* Boot code. */
    u8                 f32_bootsig[2];      /* Bootable partition signature (0x55, 0xAA). */
} Fat32DiskHeader;


#ifdef __INTELLISENSE__
typedef struct PACKED
#else
typedef union PACKED
#endif
{
    BiosParameterBlock bpb;
    Fat16DiskHeader    fat16;
    Fat32DiskHeader    fat32;
} FatDiskHeader;

typedef u32 FatSectorIndex;  /* Sector number (sector_t). */
typedef u32 FatClusterIndex; /* Cluster/Fat index number. */
#define FAT_CLUSTER_UNUSED     0 /* Cluster number found in the FAT table, marking an unused cluster. */
#define FAT_CLUSTER_FAT16_ROOT 0 /* Cluster ID found in parent-directory entries referring to the ROOT directory. */

typedef struct inode_data FatNode;

typedef struct {
    /* Tracking descriptor for free FAT directory file ranges. */
    u32   dfr_start; /* Starting FatFile index of the free range (directory position / sizeof(FatFile)). */
    u32   dfr_size;  /* [!0] Amount of consecutive, free directory entries. */
} FatDirectoryFreeRange;


struct inode_data {
    union PACKED {
        struct PACKED {
            pos_t              f16_rootpos;  /* [const] On-disk starting address of the root directory segment. */
            u32                f16_rootsiz;  /* [const] Max size of the root-directory segment (in bytes) */
        }                      i16_root;     /* [valid_if(:f_type != FAT32 && :self == :s_root)] */
        struct PACKED {
            size_t             i_clusterc;   /* [lock(:i_lock)]
                                              * [if(:f_type != FAT32 && :self == :s_root,[== 0])]
                                              * [if(:f_type == FAT32 || :self != :s_root,[!0])]
                                              *  Amount of loaded cluster indices. */
            size_t             i_clustera;   /* [lock(:i_lock)]
                                              * [if(:f_type != FAT32 && :self == :s_root,[== 0])]
                                              * [if(:f_type == FAT32 || :self != :s_root,[!0])]
                                              *  Allocated amount of cluster indices. */
            FatClusterIndex   *i_clusterv;   /* [lock(:i_lock)]
                                              * [if(:f_type != FAT32 && :self == :s_root,[== NULL])]
                                              * [if(:f_type == FAT32 || :self != :s_root,[1..i_clusterc|alloc(i_clustera)][owned])]
                                              *  Vector of file cluster starting indices.
                                              *  Indices are passed to `FAT_CLUSTERADDR()' to
                                              *  convert them into absolute on-disk locations.
                                              *  Each cluster has a length of `:f_clustersize' bytes.
                                              *  If `i_clusterv[i_clusterc-1] >= :f_cluster_eof', then
                                              *  all clusters have been loaded. */
        };
    };
    struct PACKED {
        size_t                 i_freea;      /* Allocated amount of directory-free ranges. */
        size_t                 i_freec;      /* Amount of directory-free ranges. */
        FatDirectoryFreeRange *i_freev;      /* [0..i_freec|ALLOC(i_freea)][sort(ASCENDING(->dfr_start))]
                                              * [owned] Vector of directory-free ranges. */
    }                          i_directory;
    struct PACKED { /* File header data. */
        u8                     f_attr;       /* File attr. */
        u8                     f_ntflags;    /* NT Flags (Set of `NTFLAG_*'). */
    }                          i_file;
};

/* Returns the cluster index of the `nth_cluster' cluster that is allocated for `node'.
 * NOTE: The caller must be holding at least a read-lock on `node'
 * @param: mode: Set of `FAT_GETCLUSTER_MODE_F*'
 * @throw: E_WOULDBLOCK: `IO_NONBLOCK' was specified and the operation would have blocked. */
INTDEF FatClusterIndex KCALL
Fat_GetFileCluster(struct inode *__restrict node,
                   size_t nth_cluster,
                   unsigned int mode, iomode_t flags);
#define FAT_GETCLUSTER_MODE_FNORMAL 0x0000 /* If that cluster hasn't been loaded yet, load it now.
                                            * If the cluster doesn't exist, return `f_cluster_eof_marker'. */
#define FAT_GETCLUSTER_MODE_FCREATE 0x0001 /* Allocate missing clusters. */
#define FAT_GETCLUSTER_MODE_FNOZERO 0x0002 /* Do not ZERO per-initialize newly allocated clusters. */


typedef struct superblock_data Fat;

typedef FatClusterIndex (KCALL *PFatGetFatIndirection)(Fat *__restrict self,
                                                       FatClusterIndex index);
typedef void (KCALL *PFatSetFatIndirection)(Fat *__restrict self,
                                            FatClusterIndex index,
                                            FatClusterIndex indirection_target);

/* Returns a sector number offset from `f_fat_start', within
 * which the data associated with the given `id' is stored. */
typedef FatSectorIndex (KCALL *PFatGetTableSector)(Fat *__restrict self,
                                                   FatClusterIndex id);

#define FAT_METALOAD  0x1 /* When set, the associated sector has been loaded. */
#define FAT_METACHNG  0x2 /* When set, the associated sector has been changed (Write data when syncing the filesystem). */
#define FAT_METABITS    2 /* The number of FAT metadata bits per FAT table cluster. */

struct superblock_data {
    mode_t                  f_mode;        /* Default permissions for every file on this filesystem (Defaults to 0777). */
    uid_t                   f_uid;         /* Owner UID for every file on this filesystem (Defaults to 0). */
    gid_t                   f_gid;         /* Owner GID for every file on this filesystem (Defaults to 0). */
    FatType                 f_type;        /* [const] Fat type. */
    char                    f_oem[9];      /* [const] OEM identifier. */
    char                    f_label[12];   /* [const] Volume label string (zero-terminated). */
    char                    f_sysname[9];  /* [const] System identifier string (zero-terminated). */
#define FAT_FNORMAL         0x0000         /* Normal fat flags. */
#define FAT_FCHANGED        0x0001         /* [lock(f_fat_lock)] At least one of the `FAT_METACHNG' bits in `f_fat_meta' is set. */
    u16                     f_flags;       /* Set of `FAT_F*' */
    u8                      f_pad;         /* ... */
    u32                     f_volid;       /* [const] FAT Volume ID. */
    u8                      f_fat_count;   /* [const] Amount of redundant FAT copies. */
    size_t                  f_sectorsize;  /* [const] Size of a sector (in bytes). */
    size_t                  f_clustersize; /* [const][== f_sec4clus*f_sectorsize] Size of a cluster (in bytes). */
    size_t                  f_fat_size;    /* [const][== f_sec4fat*f_sectorsize] Size of a single FAT table (in bytes). */
    FatSectorIndex          f_sec4clus;    /* [const] Amount of sectors per cluster. */
    FatSectorIndex          f_sec4fat;     /* [const] Amount of sectors per FAT table. */
    FatSectorIndex          f_dat_start;   /* [const] First data sector. */
    FatSectorIndex          f_fat_start;   /* [const] Sector number of the first FAT. */
    union PACKED {
        FatClusterIndex     f_fat_length;  /* [const] The max number of the FAT indirection entries. */
        FatClusterIndex     f_cluster_eof; /* [const] Cluster indices greater than or equal to this are considered EOF. */
    };
    FatClusterIndex         f_cluster_eof_marker; /* [const] Marker used to indicate EOF entries in the FAT. */
    PFatGetFatIndirection   f_fat_get;     /* [const][1..1] Read an entry from the FAT table. */
    PFatSetFatIndirection   f_fat_set;     /* [const][1..1] Write an entry to the FAT table. */
    PFatGetTableSector      f_fat_sector;  /* [const][1..1] Return a sector offset from `f_fat_start' of a given FAT table index.
                                            *              (That is the sector in which that part of the the FAT table is stored on-disk).
                                            *  HINT: The number returned by this may be used to interact with the
                                            *       `f_fat_meta' bitset-vector, preferably using the `FAT_META_*' macros. */
    mutex_t                 f_fat_lock;    /* Lock for accessing the FAT table cache. */
    void                   *f_fat_table;   /* [lock(f_fat_lock)][1..f_fat_size|LOGICAL_LENGTH(f_fat_length)][owned][const]
                                            *  Memory-cached version of the FAT table.
                                            *  NOTE: The way that entries within this table are read/written depends
                                            *        on the type of FAT, but `f_fat_(g|s)et' can be used for convenience.
                                            *  NOTE: The amount of entries can be read from `f_fat_length'.
                                            *  NOTE: The amount of bytes can be read from `f_fat_size'. */
    byte_t                 *f_fat_meta;    /* [lock(f_fat_lock)][1..CEILDIV(f_sec4fat,8/FAT_METABITS)|LOCIAL_LENGTH(f_sec4fat)]
                                            * [BITSET(FAT_METABITS)][owned][const]
                                            *  A bitset used to track the load/change status of `f_fat_table'.
                                            *  Stored inside this, one can find information about what FAT
                                            *  sectors have already been loaded, and which have changed.
                                            *  NOTE: This bitset contains one entry of `FAT_METABITS'
                                            *        for each sector within the FAT lookup table.
                                            *  NOTE: `f_fat_changed' must be set to `true' while changed fat entries exist. */
    FatClusterIndex         f_free_pos;    /* [lock(f_fat_lock)] Next cluster index that should be considered when search for free clusters. */
};

/* NOTE: `fat_sector_index' should be obtained by calling `f_fat_sector'. */
#define FAT_META_GTLOAD(self,fat_sector_index) ((self)->f_fat_meta[(fat_sector_index)/(8/FAT_METABITS)] &   (FAT_METALOAD << (((fat_sector_index)%(8/FAT_METABITS))*FAT_METABITS)))
#define FAT_META_STLOAD(self,fat_sector_index) ((self)->f_fat_meta[(fat_sector_index)/(8/FAT_METABITS)] |=  (FAT_METALOAD << (((fat_sector_index)%(8/FAT_METABITS))*FAT_METABITS)))
#define FAT_META_UTLOAD(self,fat_sector_index) ((self)->f_fat_meta[(fat_sector_index)/(8/FAT_METABITS)] &= ~(FAT_METALOAD << (((fat_sector_index)%(8/FAT_METABITS))*FAT_METABITS)))
#define FAT_META_GTCHNG(self,fat_sector_index) ((self)->f_fat_meta[(fat_sector_index)/(8/FAT_METABITS)] &   (FAT_METACHNG << (((fat_sector_index)%(8/FAT_METABITS))*FAT_METABITS)))
#define FAT_META_STCHNG(self,fat_sector_index) ((self)->f_fat_meta[(fat_sector_index)/(8/FAT_METABITS)] |=  (FAT_METACHNG << (((fat_sector_index)%(8/FAT_METABITS))*FAT_METABITS)))
#define FAT_META_UTCHNG(self,fat_sector_index) ((self)->f_fat_meta[(fat_sector_index)/(8/FAT_METABITS)] &= ~(FAT_METACHNG << (((fat_sector_index)%(8/FAT_METABITS))*FAT_METABITS)))


/* Get/Set FAT indirection, automatically loading missing parts
 * of the FAT table, as well as mark modified parts as changed.
 * NOTE: When calling `Fat_SetFatIndirection()', the caller must
 *       be holding a lock on `f_fat_lock'
 * @throw: E_WOULDBLOCK: `IO_NONBLOCK' was specified and the operation would have blocked. */
INTDEF FatClusterIndex KCALL
Fat_GetFatIndirection(struct superblock *__restrict self,
                      FatClusterIndex index, iomode_t flags);
INTDEF void KCALL
Fat_SetFatIndirection(struct superblock *__restrict self,
                      FatClusterIndex index,
                      FatClusterIndex indirection_target,
                      iomode_t flags);
/* Find a free cluster index.
 * NOTE: The caller will probably want to be holding a lock on `f_fat_lock' for this.
 * @throw: E_FILESYSTEM_ERROR.ERROR_FS_DISK_FULL: There are no more free clusters.
 * @throw: E_WOULDBLOCK: `IO_NONBLOCK' was specified and the operation would have blocked. */
INTDEF FatClusterIndex KCALL
Fat_FindFreeCluster(struct superblock *__restrict self,
                    iomode_t flags);

/* Delete a chain of free clusters, starting with `first_delete_index'
 * NOTE: The caller must be holding a lock on `f_fat_lock' */
INTDEF void KCALL
Fat_DeleteClusterChain(struct superblock *__restrict self,
                       FatClusterIndex first_delete_index,
                       iomode_t flags);

/* Read/Write data, given its absolute on-disk position and accompanying cluster.
 * @return: * : The actual number of read/written bytes (guarantied
 *              to equal `bufsize' when `IO_NONBLOCK' isn't set) */
INTDEF size_t KCALL
Fat_ReadFromCluster(struct superblock *__restrict self,
                    CHECKED USER byte_t *buf,
                    size_t bufsize, pos_t abs_diskpos,
                    FatClusterIndex cluster_containing_abs_diskpos,
                    iomode_t flags);
INTDEF size_t KCALL
Fat_WriteToCluster(struct superblock *__restrict self,
                   CHECKED USER byte_t const *buf,
                   size_t bufsize, pos_t abs_diskpos,
                   FatClusterIndex cluster_containing_abs_diskpos,
                   iomode_t flags);


/* Read/Write to/from a given INode at the specified offset.
 * NOTE: These functions do not truncate/update the size value
 *       of the associated INode. However allocating an additional
 *       sector will cause `Fat32_WriteToINode()' to update the
 *       the `a_blocks' attribute of `self'.
 * NOTE: Attempting to read data from beyond the allocated end
 *       of an INode will yield all ZEROes.
 * NOTE: These functions must not be used for I/O operations
 *       on the root directory of a FAT12/FAT16 file system.
 * @throw: E_FILESYSTEM_ERROR.ERROR_FS_DISK_FULL: [...]
 * @return: * : The actual number of read/written bytes (guarantied
 *              to equal `bufsize' when `IO_NONBLOCK' isn't set)
 */
INTDEF size_t KCALL
Fat32_ReadFromINode(struct inode *__restrict self,
                    CHECKED USER byte_t *buf,
                    size_t bufsize, pos_t pos, iomode_t flags);
INTDEF size_t KCALL
Fat32_WriteToINode(struct inode *__restrict self,
                   CHECKED USER byte_t const *buf,
                   size_t bufsize, pos_t pos, iomode_t flags);

/* Same as the functions above, but used for
 * operating with the FAT12/16 root directory.
 * @throw: E_FILESYSTEM_ERROR.ERROR_FS_DISK_FULL:
 *         The given `pos + bufsize' extends beyond
 *         the max size of the root directory.
 * @return: * : The actual number of read/written bytes (guarantied
 *              to equal `bufsize' when `IO_NONBLOCK' isn't set) */
INTDEF size_t KCALL
Fat16_ReadFromRootDirectory(struct inode *__restrict self,
                            CHECKED USER byte_t *buf,
                            size_t bufsize, pos_t pos, iomode_t flags);
INTDEF size_t KCALL
Fat16_WriteToRootDirectory(struct inode *__restrict self,
                           CHECKED USER byte_t const *buf,
                           size_t bufsize, pos_t pos, iomode_t flags);

/* Hybrid of the functions above:
 *  Determine the type, then read from the INode.
 * @return: * : The actual number of read/written bytes (guarantied
 *              to equal `bufsize' when `IO_NONBLOCK' isn't set) */
INTDEF size_t KCALL
Fat_ReadFromINode(struct inode *__restrict self,
                  CHECKED USER byte_t *buf,
                  size_t bufsize, pos_t pos, iomode_t flags);
INTDEF size_t KCALL
Fat_WriteToINode(struct inode *__restrict self,
                 CHECKED USER byte_t const *buf,
                 size_t bufsize, pos_t pos, iomode_t flags);


/* Returns the on-disk address of a given cluster number. */
#define FAT_CLUSTERADDR(self,cluster_id) \
      FAT_SECTORADDR(self,FAT_CLUSTERSTART(self,cluster_id))

/* Returns the on-disk address of a given sector number. */
#define FAT_SECTORADDR(self,sector_num)   \
      ((pos_t)(sector_num)*(self)->f_sectorsize)

/* Returns the sector number of a given cluster, which then spans `self->f_sec4clus' sectors. */
#define FAT_CLUSTERSTART(self,cluster_id) \
      ((FatSectorIndex)((self)->f_dat_start+(((cluster_id)-2)*(self)->f_sec4clus)))



/* In FAT, we store the raw 8.3 filename (without case-fix) immediately after the regular name. */
#define FAT_DIRECTORY_ENTRY83(x)  ((char *)(x)->de_fsdata.de_data)


DECL_END

#endif /* !GUARD_KERNEL_SRC_CORE_FAT_H */
