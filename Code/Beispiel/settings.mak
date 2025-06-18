#---------------------------------------------------------------------#
#                                                                     #
#                        Bihl+Wiedemann GmbH                          #
#                                                                     #
#                                                                     #
#        project: Control_III                                         #
#    module name: settings.mak                                        #
#         author: Christian Sommerfeld                                #
#           date: 2015-02-26                                          #
#                                                                     #
#    version: 1.0 first version                                       #
#                                                                     #
#                                                                     #
#---------------------------------------------------------------------#
##### ###### ##### ######
#  ## ##  ## #  ##   ##
##### ##  ## #####   ##
##    ##  ## # #     ##
##    ###### #  #    ##


#+++ Einstellung Port +++
#Beispiel: COM1, COM2, UDP:192.168.42.129

PORT=UDP:192.168.1.1

#---------------------------------------------------------------------#

CTRL= ../../Compiler/Tools/bwctrl.exe
			

.PHONY: header
header:
	echo -en "" \
		"-----------------------\n" \
		" ++++ CONTROL III ++++ \n" \
		"-----------------------\n" \
		"\n"

			
.PHONY: download_ctrl
download_ctrl: header
	$(CTRL) "port $(PORT); ctrl dn control.bin; exit"

.PHONY: start_ctrl
start_ctrl: header
	$(CTRL) "port $(PORT); ctrl wrflags 1; exit"

.PHONY: stop_ctrl
stop_ctrl: header
	$(CTRL) "port $(PORT); ctrl wrflags 0; exit"
	
.PHONY: zykluszeit_ctrl
zykluszeit_ctrl: header
	$(CTRL) "port $(PORT); ctrl ct; exit"		

.PHONY: auto_start_ctrl
auto_start_ctrl: header
	$(CTRL) "port $(PORT); ctrl wrflags 0x08; exit"
	
.PHONY: auto_stop_ctrl
auto_stop_ctrl: header
	$(CTRL) "port $(PORT); ctrl wrflags 0x04; exit"	
	
.PHONY: read_merker
read_merker: header
	$(CTRL) "port $(PORT); ctrl rdmerk 0x00, 0xFF; exit";

.PHONY: read_merker_cyclic
read_merker_cyclic:
	for number in 1 2 3 4 5 6 7 8 9 10; do \
	    $(CTRL) "port $(PORT); ctrl rdmerk 0x00, 0xFF; sleep 1000; exit"; \
	done
		
.PHONY: reset_cycle
reset_cycle: header
	$(CTRL) "port $(PORT); ctrl resetcycle; exit"	