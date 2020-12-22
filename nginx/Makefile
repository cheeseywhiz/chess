NGINX := nginx -c $(PWD)/nginx.conf
PID := $(shell cat nginx.pid 2>/dev/null)

ifeq ($(ASKPASS),y)
	SUDO := sudo -A
else
	SUDO := sudo
endif

.PHONY: all
all: quit
	$(SUDO) $(NGINX)
	tail -f *.log

.PHONY: quit
quit:
ifneq ($(PID),)
	$(SUDO) $(NGINX) -s quit
endif

.PHONY: restart
restart: quit all
