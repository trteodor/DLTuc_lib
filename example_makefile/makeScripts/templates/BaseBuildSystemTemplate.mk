MODULES     := A B C
TEMPLATES   := X Y Z
ALL         :=

define MODULE_TEMPLATE_rule
$(1)/$(2):
	@echo The Module is $(1) and the Template is $(2)

ALL += $(1)/$(2)
endef

define MODULE_rule
$(foreach template,$(TEMPLATES),$(eval $(call MODULE_TEMPLATE_rule,$(1),$(template)))   )
endef

$(foreach module,$(MODULES),   $(eval $(call MODULE_rule,%$(module))))

all: $(ALL)

#https://stackoverflow.com/questions/32532999/double-foreach-in-makepp/32535737#32535737