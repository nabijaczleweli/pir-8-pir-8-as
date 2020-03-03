# The MIT License (MIT)

# Copyright (c) 2020 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


include configMakefile


LDAR := $(PIC) $(LNCXXAR) -L$(BLDDIR) $(foreach l,$(OS_LD_LIBS) handler,-l$(l))
INCAR := $(foreach l,ext/pir-8-emu/include,-isystem$(l))
HANDLER_SOURCES := $(sort $(wildcard $(HNDDIR)*.cpp))
HANDLER_LIB_SOURCES := $(sort $(wildcard $(HNDDIR)**/*.cpp $(HNDDIR)**/**/*.cpp))
ASSEMBLY_SOURCES := $(sort $(wildcard $(ASMDIR)*.p8a.pp))


.PHONY : all clean handlers assemblies
.SECONDARY :


all : handlers assemblies

clean :
	rm -rf $(OUTDIR)

handlers : $(foreach l,$(HANDLER_SOURCES),$(patsubst $(HNDDIR)%.cpp,$(OUTDIR)$(PREDLL)%$(DLL),$(l)))

assemblies : $(foreach l,$(ASSEMBLY_SOURCES),$(patsubst $(ASMDIR)%.p8a.pp,$(OUTDIR)%$(PIR_8_EXE),$(l)))


$(BLDDIR)libhandler$(ARCH) : $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%$(OBJ),$(HANDLER_LIB_SOURCES))
	@mkdir -p $(dir $@)
	$(AR) crs $@ $^


$(OUTDIR)$(PREDLL)%$(DLL) : $(BLDDIR)libhandler$(ARCH) $(OBJDIR)%$(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -o$@ $(filter-out $<,$^) $(LDAR) -shared

$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) -c -o$@ $^

$(OBJDIR)%$(OBJ) : $(HNDDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) -c -o$@ $^

$(BLDDIR)% : $(ASMDIR)%.pp
	@mkdir -p $(dir $@)
	$(CPP) $(notdir $(1)) -nostdinc -I$(abspath $(BLDDIR)/highlit) -CC -P -o$@ $^

$(OUTDIR)%$(PIR_8_EXE) : $(BLDDIR)%.p8a
	@mkdir -p $(dir $@)
	$(PIR_8_AS) -o$@ $^
