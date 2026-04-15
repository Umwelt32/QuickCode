import lief

base_addr = int(0x70100000)
elf = lief.ELF.parse("nor_spicomdrv.elf")
flash_image = bytearray(0x4000)
idx = 0
for seg in elf.segments:
    print(str(hex(seg.virtual_address)) +' / '+ str(hex(seg.physical_size)))
    content = bytes(seg.content)
    idx = int(seg.virtual_address)-base_addr
    for b in content:
        if idx>=0:flash_image[idx]=b
        idx=idx+1
# Optionally write to a binary file
with open("nor_spicomdrv.bin", "wb") as f:
    f.write(flash_image)

print("Dumped {} bytes".format(len(flash_image)))
