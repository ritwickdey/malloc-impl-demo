define map_src_to_desc
	$(patsubst $(1)/%.c,$(2)/%.o,$(wildcard $(1)/*.c))
endef