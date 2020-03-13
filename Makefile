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


LDAR := $(PIC) $(LNCXXAR) -L$(BLDDIR) $(foreach l,$(OS_LD_LIBS) handler fmt,-l$(l))
INCAR := $(foreach l,pir-8-emu fmt,-isystem$(EXTDIR)$(l)/include)
HANDLER_SOURCES := $(sort $(wildcard $(HNDDIR)*.cpp))
HANDLER_LIB_SOURCES := $(sort $(wildcard $(HNDDIR)**/*.cpp $(HNDDIR)**/**/*.cpp))
TEST_SOURCES := $(sort $(wildcard $(TSTDIR)*.cpp $(TSTDIR)**/*.cpp $(TSTDIR)**/**/*.cpp))
ASSEMBLIES := $(patsubst $(ASMDIR)%.p8a,%,$(sort $(wildcard $(ASMDIR)*.p8a)))
FMT_SOURCES := $(sort $(wildcard $(EXTDIR)fmt/src/*.cc))


.PHONY : all clean handlers assemblies tests
.SECONDARY :


all : handlers assemblies tests

clean :
	rm -rf $(OUTDIR)

handlers : $(foreach l,$(HANDLER_SOURCES),$(patsubst $(HNDDIR)%.cpp,$(OUTDIR)$(PREDLL)%$(DLL),$(l)))

assemblies : $(foreach l,$(ASSEMBLIES),$(patsubst %,$(OUTDIR)%$(PIR_8_EXE),$(l)))

tests : $(foreach l,$(TEST_SOURCES),$(patsubst %.cpp,$(OUTDIR)%$(EXE),$(l)))
	$(foreach l,$^,$l &&) :


$(BLDDIR)libhandler$(ARCH) : $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%$(OBJ),$(HANDLER_LIB_SOURCES))
	@mkdir -p $(dir $@)
	$(AR) crs $@ $^

$(BLDDIR)libfmt$(ARCH) : $(patsubst $(EXTDIR)%.cc,$(OBJDIR)ext/%$(OBJ),$(FMT_SOURCES))
	@mkdir -p $(dir $@)
	$(AR) crs $@ $^


# This is ugly, but I couldn't get Make to pass % to $(wildcard) or $(call)
define ASSEMBLY_TEMPLATE =
$(OUTDIR)$(1)$(PIR_8_EXE) : $(ASMDIR)$(1).p8a $(sort $(wildcard $(ASMDIR)$(1)/*.p8a $(ASMDIR)$(1)/**/*.p8a $(ASMDIR)$(1)/**/**/*.p8a))
	@mkdir -p $$(dir $$@)
	$(PIR_8_AS) -o$$@ $$^
endef
$(foreach l,$(ASSEMBLIES),$(eval $(call ASSEMBLY_TEMPLATE,$(l))))


$(OUTDIR)$(PREDLL)%$(DLL) : $(OBJDIR)%$(OBJ) $(foreach l,fmt handler,$(BLDDIR)lib$(l)$(ARCH))
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -o$@ $< $(LDAR) -shared

$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) -c -o$@ $^

$(OBJDIR)%$(OBJ) : $(HNDDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) -c -o$@ $^

$(OBJDIR)tests/%$(OBJ) : $(TSTDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) -I$(TSTDIR) -I$(HNDDIR) -c -o$@ $^

$(OBJDIR)ext/%$(OBJ) : $(EXTDIR)%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) -c -o$@ $^

$(BLDDIR)% : $(ASMDIR)%.pp
	@mkdir -p $(dir $@)
	$(CPP) $(notdir $(1)) -nostdinc -I$(abspath $(BLDDIR)highlit) -CC -P -o$@ $^

$(OUTDIR)%$(EXE) : $(OBJDIR)%$(OBJ) $(foreach l,fmt handler,$(BLDDIR)lib$(l)$(ARCH))
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -o$@ $< $(LDAR)
