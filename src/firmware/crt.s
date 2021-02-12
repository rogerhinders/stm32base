.cpu cortex-m3
.thumb

# handlers start
.thumb_func
_reset_handler:
	bl main
	b .
