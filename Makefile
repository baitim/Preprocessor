options = -O3 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

debug =

DEFAULT =       $(wildcard Stack/*.cpp) $(wildcard *.cpp) 
ASM = 			$(DEFAULT) $(wildcard Asm/*.cpp)
DISASM = 	    $(DEFAULT) $(wildcard Disasm/*.cpp) $(wildcard Asm/Input.cpp)
Main_files = 	$(DEFAULT) $(wildcard Processor/*.cpp) $(wildcard Asm/Input.cpp)

start : run_asm run_disasm run_main finish_asm finish_disasm finish

run_asm :
	g++ $(ASM) -o asm $(options) $(debug)

run_disasm :
	g++ $(DISASM) -o disasm $(options) $(debug)

run_main :
	g++ $(Main_files) -o start $(options) $(debug)

finish_asm:
	./asm files/Input_commands.txt files/Asm.txt files/Asm.bin

finish_disasm:
	./disasm files/Asm.txt files/Disasm_txt.txt files/Asm.bin files/Disasm_bin.txt

finish:	
	./start files/ASM.bin