echo "out/resource.o out/main.o" > out/cmd_
G:/sgdk/bin/gcc.exe -BG:/sgdk/bin -n -T G:/sgdk/md.ld -nostdlib out/sega.o @out/cmd_ G:/sgdk/lib/libmd.a G:/sgdk/lib/libgcc.a -o out/rom.out -Wl,--gc-sections
G:/sgdk/bin/rm.exe out/cmd_
G:/sgdk/bin/objcopy.exe -O binary out/rom.out out/rom.bin
java -jar G:/sgdk/bin/sizebnd.jar out/rom.bin -sizealign 131072 -checksum
G:/sgdk/bin/nm.exe --plugin=liblto_plugin-0.dll -n out/rom.out > out/symbol.txt
rm resource.rs