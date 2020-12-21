#include "disk.h"

#include "io.h"
#include "timer.h"
#include "types.h"
#include "util.h"

// https://wiki.osdev.org/IDE#Read.2FWrite_From_ATA_Drive

#define BAR0 0x1f0  // Base address of primary channel
#define BAR1 0x3f6  // Base address of primary channel control port
#define BAR2 0x170  // Base address of secondary channel
#define BAR3 0x376  // Base address of secondary channel control port
#define BAR4 0x000  // !Looks unused and a fake value.

// Status.
#define ATA_SR_BSY 0x80   // Busy
#define ATA_SR_DRDY 0x40  // Drive ready
#define ATA_SR_DF 0x20    // Drive write fault
#define ATA_SR_DSC 0x10   // Drive seek complete
#define ATA_SR_DRQ 0x08   // Data request ready
#define ATA_SR_CORR 0x04  // Corrected data
#define ATA_SR_IDX 0x02   // Index
#define ATA_SR_ERR 0x01   // Error

// Error.
#define ATA_ER_BBK 0x80    // Bad block
#define ATA_ER_UNC 0x40    // Uncorrectable data
#define ATA_ER_MC 0x20     // Media changed
#define ATA_ER_IDNF 0x10   // ID mark not found
#define ATA_ER_MCR 0x08    // Media change request
#define ATA_ER_ABRT 0x04   // Command aborted
#define ATA_ER_TK0NF 0x02  // Track 0 not found
#define ATA_ER_AMNF 0x01   // No address mark

// Command.
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_READ_PIO_EXT 0x24
#define ATA_CMD_READ_DMA 0xC8
#define ATA_CMD_READ_DMA_EXT 0x25
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_CMD_WRITE_PIO_EXT 0x34
#define ATA_CMD_WRITE_DMA 0xCA
#define ATA_CMD_WRITE_DMA_EXT 0x35
#define ATA_CMD_CACHE_FLUSH 0xE7
#define ATA_CMD_CACHE_FLUSH_EXT 0xEA
#define ATA_CMD_PACKET 0xA0
#define ATA_CMD_IDENTIFY_PACKET 0xA1
#define ATA_CMD_IDENTIFY 0xEC

#define ATAPI_CMD_READ 0xA8
#define ATAPI_CMD_EJECT 0x1B

// Helpers for ATA_CMD_IDENTIFY_PACKET and ATA_CMD_IDENTIFY.
#define ATA_IDENT_DEVICETYPE 0
#define ATA_IDENT_CYLINDERS 2
#define ATA_IDENT_HEADS 6
#define ATA_IDENT_SECTORS 12
#define ATA_IDENT_SERIAL 20
#define ATA_IDENT_MODEL 54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID 106
#define ATA_IDENT_MAX_LBA 120
#define ATA_IDENT_COMMANDSETS 164
#define ATA_IDENT_MAX_LBA_EXT 200

// Drive selection.
#define IDE_ATA 0x00
#define IDE_ATAPI 0x01
#define ATA_MASTER 0x00
#define ATA_SLAVE 0x01

// BAR# + _
#define ATA_REG_DATA 0x00
#define ATA_REG_ERROR 0x01
#define ATA_REG_FEATURES 0x01
#define ATA_REG_SECCOUNT0 0x02
#define ATA_REG_LBA0 0x03
#define ATA_REG_LBA1 0x04
#define ATA_REG_LBA2 0x05
#define ATA_REG_HDDEVSEL 0x06
#define ATA_REG_COMMAND 0x07
#define ATA_REG_STATUS 0x07
#define ATA_REG_SECCOUNT1 0x08
#define ATA_REG_LBA3 0x09
#define ATA_REG_LBA4 0x0A
#define ATA_REG_LBA5 0x0B
#define ATA_REG_CONTROL 0x0C
#define ATA_REG_ALTSTATUS 0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels:
#define ATA_PRIMARY 0x00
#define ATA_SECONDARY 0x01

// Directions:
#define ATA_READ 0x00
#define ATA_WRITE 0x01

// IDE poll errors:
#define IDE_POLL_ERR_GENERIC 0x2
#define IDE_POLL_ERR_DEVICE_FAULT 0x1
#define IDE_POLL_ERR_DRQ 0x3

struct ide_channel_registers_t {
  u16 base;
  u16 ctrl;
  u16 bus_master_ide;
  u8 no_interrupt;
} channels[2];

struct ide_device_t {
  u8 reserved;
  u8 channel;
  u8 drive;
  u16 type;
  u16 signature;
  u16 capabilities;
  u32 command_sets;
  u32 size;
  u8 model[41];
} ide_devices[4];

void ide_write(u8 channel, u8 reg, u8 data) {
  if (0x7 < reg && reg < 0xC) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].no_interrupt);
  }

  if (reg < 0x8) {
    io_byte_out(channels[channel].base + reg - 0x0, data);
  } else if (reg < 0xc) {
    io_byte_out(channels[channel].base + reg - 0x6, data);
  } else if (reg < 0xe) {
    io_byte_out(channels[channel].ctrl + reg - 0xa, data);
  } else if (reg < 0xf) {
    // This was 0x16 originall but it seems regs are 0x0..0xd.
    io_byte_out(channels[channel].bus_master_ide + reg - 0xe, data);
  } else {
    panic("Unexpected reg");
  }

  if (0x7 < reg && reg < 0xC) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].no_interrupt);
  }
}

void ide_read_buffer(u8 channel, u8 reg, void* buf, u32 quads) {
  if (0x7 < reg && reg < 0xC) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].no_interrupt);
  }

  asm("pushw %es; movw %ds, %ax; movw %ax, %es");

  if (reg < 0x8) {
    io_dword_seq_in(channels[channel].base + reg - 0x0, buf, quads);
  } else if (reg < 0xc) {
    io_dword_seq_in(channels[channel].base + reg - 0x6, buf, quads);
  } else if (reg < 0xe) {
    io_dword_seq_in(channels[channel].ctrl + reg - 0xa, buf, quads);
  } else if (reg < 0xf) {
    // This was 0x16 originall but it seems regs are 0x0..0xd.
    io_dword_seq_in(channels[channel].bus_master_ide + reg - 0xe, buf, quads);
  } else {
    panic("Unexpected reg");
  }

  asm("popw %es;");

  if (0x7 < reg && reg < 0xC) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].no_interrupt);
  }
}

u8 ide_read(u8 channel, u8 reg) {
  u8 result;

  if (0x7 < reg && reg < 0xC) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].no_interrupt);
  }

  if (reg < 0x8) {
    result = io_byte_in(channels[channel].base + reg - 0x0);
  } else if (reg < 0xc) {
    result = io_byte_in(channels[channel].base + reg - 0x6);
  } else if (reg < 0xe) {
    result = io_byte_in(channels[channel].ctrl + reg - 0xa);
  } else if (reg < 0xf) {
    // This was 0x16 originall but it seems regs are 0x0..0xd.
    result = io_byte_in(channels[channel].bus_master_ide + reg - 0xe);
  } else {
    panic("Unexpected reg");
  }

  if (0x7 < reg && reg < 0xC) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].no_interrupt);
  }

  return result;
}

u8 ide_polling(u8 channel, u32 advanced_check) {
  // Just to wait 400ms.
  for (int i = 0; i < 4; i++) {
    ide_read(channel, ATA_REG_ALTSTATUS);
  }

  while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
    ;

  if (advanced_check) {
    u8 state = ide_read(channel, ATA_REG_STATUS);

    if (state & ATA_SR_ERR) {
      return IDE_POLL_ERR_GENERIC;
    }

    if (state & ATA_SR_DF) {
      return IDE_POLL_ERR_DEVICE_FAULT;
    }

    if ((state & ATA_SR_DRQ) == 0) {
      return IDE_POLL_ERR_DRQ;
    }
  }

  return 0;
}

void ide_initialize(u32 bar0, u32 bar1, u32 bar2, u32 bar3, u32 bar4) {
  channels[ATA_PRIMARY].base = (bar0 & 0xFFFFFFFC) + BAR0 * (!bar0);
  channels[ATA_PRIMARY].ctrl = (bar1 & 0xFFFFFFFC) + BAR1 * (!bar1);
  channels[ATA_PRIMARY].bus_master_ide = (bar4 & 0xFFFFFFFC) + 0;

  channels[ATA_SECONDARY].base = (bar2 & 0xFFFFFFFC) + BAR2 * (!bar2);
  channels[ATA_SECONDARY].ctrl = (bar3 & 0xFFFFFFFC) + BAR3 * (!bar3);
  channels[ATA_SECONDARY].bus_master_ide = (bar4 & 0xFFFFFFFC) + 8;

  // Disable IRQ.
  ide_write(ATA_PRIMARY, ATA_REG_CONTROL, 2);
  ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

  unsigned char ide_buf[2048] = {0};
  int count = 0;

  // Detect ATA/ATAPI devices.
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      u8 err = 0;
      u8 type = IDE_ATA;
      u8 status;

      ide_write(i, ATA_REG_HDDEVSEL, 0xa0 | (j << 4));
      timer_sleep(1);

      ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
      timer_sleep(1);

      if (ide_read(i, ATA_REG_STATUS) == 0) continue;
      for (;;) {
        status = ide_read(i, ATA_REG_STATUS);

        if ((status & ATA_SR_ERR)) {
          // Device not ATA.
          err = 1;
          break;
        }

        if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) {
          // All good.
          break;
        }
      }

      if (err != 0) {
        u8 cl = ide_read(i, ATA_REG_LBA1);
        u8 ch = ide_read(i, ATA_REG_LBA2);

        if ((cl == 0x14 && ch == 0xeb) || (cl == 0x69 && ch == 0x96)) {
          type = IDE_ATAPI;
        } else {
          continue;
        }

        ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
        timer_sleep(1);
      }

      // Read identitifaction space of the device.
      ide_read_buffer(i, ATA_REG_DATA, ide_buf, 128);

      ide_devices[count].reserved = 1;
      ide_devices[count].type = type;
      ide_devices[count].channel = i;
      ide_devices[count].drive = j;
      ide_devices[count].signature = *((u16*)(ide_buf + ATA_IDENT_DEVICETYPE));
      ide_devices[count].capabilities =
          *((u16*)(ide_buf + ATA_IDENT_CAPABILITIES));
      ide_devices[count].command_sets =
          *((u32*)(ide_buf + ATA_IDENT_COMMANDSETS));

      if (ide_devices[count].command_sets & (1 << 26)) {
        ide_devices[count].size = *((u32*)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
      } else {
        ide_devices[count].size = *((u32*)(ide_buf + ATA_IDENT_MAX_LBA));
      }

      for (int k = 0; k < 40; k += 2) {
        ide_devices[count].model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
        ide_devices[count].model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];
      }
      ide_devices[count].model[40] = 0;

      count++;
    }
  }
}

void disk_init() {
  //
  ide_initialize(BAR0, BAR1, BAR2, BAR3, BAR4);
}
