options = -O3 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

debug =

Main_files = 			$(wildcard Main/*.cpp) $(wildcard Stack/*.cpp)
To_byte_code = 			$(wildcard To_byte_code/*.cpp)
From_byte_code_files = 	$(wildcard From_byte_code/*.cpp)

start : run_to_byte_code run_from_byte_code run_main finish_to_byte_code finish_from_byte_code finish

run_to_byte_code :
	g++ $(To_byte_code) -o to_byte_code $(options) $(debug)

run_from_byte_code :
	g++ $(From_byte_code_files) -o from_byte_code $(options) $(debug)

run_main :
	g++ $(Main_files) -o start $(options) $(debug)

finish_to_byte_code:
	./to_byte_code

finish_from_byte_code:
	./from_byte_code

finish:	
	./start