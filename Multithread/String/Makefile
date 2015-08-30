folder_root = ~/multithread
string_fold = $(folder_root)/String
tstring_fold = $(folder_root)/Tstring
flags=-g -Wall -Werror -o
lib=-lpthread
TARGET = string
TTARGET = tstring
dirs := String 
dirs := Tstring 

nodes=50
delta=0.1
thread=2

PHONY= st tst clean uninstall

all : folders tstringPr stringPr 

stringPr: string.o 
		cd $(string_fold); g++ string.o $(flags) $(TARGET)
tstringPr: tstring.o
		cd $(tstring_fold); g++ tstring.o $(flags) $(TTARGET) $(lib) 

folders: 
		mkdir -p String
		mkdir -p Tstring


string.o: $(folder_root)/string.cpp 
		cd $(string_fold); g++ -c ../string.cpp 

tstring.o : $(folder_root)/tstring.cpp 
		cd $(tstring_fold); g++ -c ../tstring.cpp

st:
		cd $(string_fold); ./$(TARGET) $(nodes) $(delta)
tst:
		cd $(tstring_fold); ./$(TTARGET) $(thread) $(nodes) $(delta) 

clean:
		cd $(string_fold);rm *.o
		cd $(tstring_fold);rm *.o
uninstall:
		rm -rf String
		rm -rf Tstring
