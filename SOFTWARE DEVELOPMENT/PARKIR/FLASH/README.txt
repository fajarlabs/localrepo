Untuk mengeset lock fuse bit.
Buka software Avrdudes, 
Pilih programmer USBasp, http://www.fischl.de/usbasp/, 
Pilih MCU (ATmega8)
-------------------------------------------------------
Fuses & lock bits
-------------------------------------------------------
L : 0xA4
H : 0xD9
E : kosong
LB : 0x3C


Tambahkan script ini di boards.txt, pathnya disini C:\Program Files (x86)\Arduino\hardware\arduino\avr\boards.txt

##############################################################

atmega8noxtal.name=ATmega8-noxtal @8MHz

atmega8noxtal.upload.protocol=stk500
atmega8noxtal.upload.maximum_size=7168
atmega8noxtal.upload.speed=38400
atmega8noxtal.upload.tool=avrdude

atmega8noxtal.bootloader.low_fuses=0xe4
atmega8noxtal.bootloader.high_fuses=0xc4
atmega8noxtal.bootloader.file=atmega8_noxtal/ATmegaBOOT.hex
atmega8noxtal.bootloader.unlock_bits=0x3F
atmega8noxtal.bootloader.lock_bits=0x0F

atmega8noxtal.build.mcu=atmega8
atmega8noxtal.build.f_cpu=8000000L
atmega8noxtal.build.core=arduino
atmega8noxtal.build.board=AVR_NG
atmega8noxtal.build.variant=standard


Copy pastekan folder atmega8_noxtal kedalam path dibawah ini,
C:\Program Files (x86)\Arduino\hardware\arduino\avr\bootloaders

Restart arduino ide.

Settting arduino IDE, 

Pilih Tools -> Boards "ATmega8-noxtal @8Mhz", 
pilih Tools -> programmer -> USBAsp
Lalu untuk upload codenya 
Pilih Sketch -> Upload Using Programmer

Jika errornya, coba pindahkan kable USB atau cek drivernya

