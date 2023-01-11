.RECIPEPREFIX = >

DIR_SRC:=SRC
DIR_INC:=INC
DIR_OBJ:=OBJ
DIR_OUT:=OUT
DIR_CUR:=./

DEL:= DEL /Q /F 
CC := cl65
CFLAGS := -g -Oi
TARGET := PROGRAM.BIN

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
SRCS := $(wildcard $(DIR_CUR)$(DIR_SRC)/*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
>   @echo $(notdir $(OBJS)) > $(DIR_CUR)$(DIR_OUT)/_obj.txt
>   @echo $(notdir $(SRCS)) > $(DIR_CUR)$(DIR_OUT)/_src.txt
>   $(CC) --obj-path $(DIR_CUR)$(DIR_OBJ) -o $(DIR_CUR)$(DIR_OUT)/$@ $(notdir $^) >> $(DIR_CUR)$(DIR_OUT)/l_obj.txt

%.o: %.c
>   $(CC) $(CFLAGS) --include-dir $(DIR_CUR)$(DIR_INC) -o $(DIR_CUR)$(DIR_OBJ)/$(patsubst %.c,%.o,$(notdir $<)) -c $< >> $(DIR_CUR)$(DIR_OUT)/c_obj.txt

clean:
>   @echo clean_target
>   @$(DEL) .\\$(DIR_OBJ)\\*.*
>   @$(DEL) .\\$(DIR_OUT)\\*.*

.PHONY: all clean
