options = -O3 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

debug =

Main_files = 	$(wildcard Main/*.cpp) $(wildcard Stack/*.cpp)
ASM = 			$(wildcard Asm/*.cpp)
DISASM = 	    $(wildcard Disasm/*.cpp)

start : run_asm run_disasm run_main finish_asm finish_disasm finish

run_asm :
	g++ $(ASM) -o asm $(options) $(debug)

run_disasm :
	g++ $(DISASM) -o disasm $(options) $(debug)

run_main :
	g++ $(Main_files) -o start $(options) $(debug)

finish_asm:
	./asm

finish_disasm:
	./disasm

finish:	
	./start