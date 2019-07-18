##
##  \file
##  \author     Alberto Hamilton
##  \date       2014-2017
##  \copyright  GNU General Public License v3
##
##  \brief Fichero Makefile para compilar los distintos objetivos
##  \details
## Se consideran objetivos los ejecutables correpondientes
##  a los fichero con patron test_*.cpp
##
##  Las clases deben comenzar en mayúscula (`Clase`) y estar definidas en
##   dos ficheros: `Clase.hpp` y `Clase.cpp`.
##
##  \cond
##

BDIR=build
CC=g++ -Wall --std=c++14
DOCDIR=doc

test:=$(wildcard test_*.cpp)
clases:=$(wildcard [A-Z]*.cpp)
defclases:=$(wildcard [A-Z]*.hpp)

ejecutables:=$(addprefix $(BDIR)/,$(test:.cpp=))

#all y clean son objetivos especiales
.PHONY : all clean
#Evitamos que se borren los fichero intermedios (%.o)
.SECONDARY :

all : $(BDIR) $(ejecutables)

$(BDIR):
	@mkdir -p $@


depend : $(BDIR)/.dependC $(BDIR)/.dependP $(BDIR)/.dependE

# Crea dependencias compilación de las clases
$(BDIR)/.dependC: $(clases) Makefile
	@$(CC)  -MM $(clases) > $@ 2>/dev/null
	@sed -e"s?^\([^ ]\)?$(BDIR)/\1?" -i $@

# Crea dependencia compilación de las pruebas
$(BDIR)/.dependP: $(test) $(clases) Makefile
	@$(CC)  -MM $(test) > $@ 2>/dev/null
	@sed -e"s?^\([^ ]\)?$(BDIR)/\1?" -i $@

# Crea dependencias enlazado de los ejecutables
$(BDIR)/.dependE: $(BDIR)/.dependP
	@cat $< | sed -e's/\.o//' -e's/\..pp/\.o/g' \
	  -e"s? \([a-zA-Z]\)? $(BDIR)/\1?g" \
	  > $@


-include $(BDIR)/.dependC
-include $(BDIR)/.dependP
-include $(BDIR)/.dependE

#Reglas genéricas
$(BDIR)/%.o : %.cpp
	$(CC) -c -o $@ $<

$(BDIR)/test_% : $(BDIR)/test_%.o
	$(CC) -pthread -o  $@ $^

#reglas para borrar los fichero generados
# y dejar sólo los fuente
clean :
	rm -f $(BDIR)/*.o $(ejecutables) $(BDIR)/.depend* *.hpp.gch resumen.log core
	rm -rf $(DOCDIR)/*

docu :
	doxygen Doxyfile

variables:
	@echo test: $(test)
	@echo clases: $(clases)
	@echo defclases: $(defclases)
	@echo ejecutables: $(ejecutables)


##
##  \endcond
##
