/** @file
  Copyright (C) 2020, vit9696. All rights reserved.

  All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef OC_APFS_INTERNAL_H
#define OC_APFS_INTERNAL_H

#include <IndustryStandard/Apfs.h>
#include <Protocol/BlockIo.h>
#include <Protocol/ApfsEfiBootRecordInfo.h>

#define APFS_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('A', 'F', 'J', 'S')

typedef struct APFS_PRIVATE_DATA_ APFS_PRIVATE_DATA;

/**
  Private data storing fusion pairs and location protocol data.
**/
typedef struct APFS_PRIVATE_DATA_ {
  //
  // Set to APFS_PRIVATE_DATA_SIGNATURE.
  //
  UINT32                              Signature;
  //
  // Linked to next instance of APFS_PRIVATE_DATA.
  //
  LIST_ENTRY                          Link;
  //
  // Location information describing controller and container.
  //
  APFS_EFIBOOTRECORD_LOCATION_INFO    LocationInfo;
  //
  // Block I/O protocol.
  //
  EFI_BLOCK_IO_PROTOCOL               *BlockIo;
  //
  // APFS block size, a multiple of Block I/O block size.
  //
  UINT32                              ApfsBlockSize;
  //
  // Number of Block I/O blocks in APFS block.
  //
  UINT32                              LbaMultiplier;
  //
  // JumpStart driver LBA.
  //
  UINT64                              EfiJumpStart;
  //
  // Fusion UUID.
  //
  GUID                                FusionUuid;
  //
  // Mask used to determine where block belongs.
  //
  UINT64                              FusionMask;
  //
  // Fusion sibling private data.
  //
  APFS_PRIVATE_DATA                   *FusionSibling;
  //
  // Can try loading bundled driver.
  //
  BOOLEAN                             CanLoadDriver;
  //
  // Partition is part of Fusion drive.
  //
  BOOLEAN                             IsFusion;
  //
  // Partition is master Fusion partition.
  //
  BOOLEAN                             IsFusionMaster;
} APFS_PRIVATE_DATA;

/**
  List of discovered partitions.
**/
extern LIST_ENTRY  mApfsPrivateDataList;

EFI_STATUS
InternalApfsReadSuperBlock (
  IN  EFI_BLOCK_IO_PROTOCOL  *BlockIo,
  OUT APFS_NX_SUPERBLOCK     **SuperBlockPtr
  );

EFI_STATUS
InternalApfsReadJumpStartDriver (
  IN  APFS_PRIVATE_DATA    *PrivateData,
  OUT UINTN                *DriverSize,
  OUT VOID                 **DriverBuffer
  );

VOID
InternalApfsInitFusionData (
  IN  APFS_NX_SUPERBLOCK   *SuperBlock,
  OUT APFS_PRIVATE_DATA    *PrivateData
  );

EFI_BLOCK_IO_PROTOCOL *
InternalApfsTranslateBlock (
  IN  APFS_PRIVATE_DATA    *PrivateData,
  IN  UINT64               Block,
  OUT EFI_LBA              *Lba
  );

#endif // OC_APFS_INTERNAL_H
