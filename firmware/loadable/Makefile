# Make doesn't allow dependencies on parent directory, so we need to
# run make from one level up:

MAKEFILE=loadable/Makefile.sub
MAKE+=--no-print-directory

all:
	@cd .. && $(MAKE) -f $(MAKEFILE)

clean:
	@cd .. && $(MAKE) -f $(MAKEFILE) clean
	
