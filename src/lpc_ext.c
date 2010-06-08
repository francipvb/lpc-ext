# define LPCEXT			/* declare */
# include "lpc_ext.h"
# include <stdarg.h>


/*
 * NAME:	ext->cb()
 * DESCRIPTION:	set up callbacks
 */
static int ext_cb(void *ftab[], int size, int n, ...)
{
    va_list args;
    void **func;

    if (size < n) {
	return 0;
    }

    va_start(args, n);
    do {
	func = va_arg(args, void*);
	*func = *ftab++;
    } while (--n > 0);
    va_end(args);
}

/*
 * NAME:	ext->init()
 * DESCRIPTION:	initialize extension handling
 */
int ext_init(int major, int minor, void **ftabs[], int sizes[])
{
    if (major != LPC_EXT_VERSION_MAJOR ||
	!ext_cb(ftabs[0], sizes[0], 2, &lpc_error, &lpc_ext_kfun) ||
	!ext_cb(ftabs[1], sizes[1], 4, &lpc_frame_object, &lpc_frame_dataspace,
		&lpc_frame_arg, &lpc_frame_atomic) ||
	!ext_cb(ftabs[2], sizes[2], 2, &lpc_data_get_val, &lpc_data_set_val) ||
	!ext_cb(ftabs[3], sizes[3], 3, &lpc_value_type, &lpc_value_nil,
		&lpc_value_temp) ||
	!ext_cb(ftabs[4], sizes[4], 2, &lpc_int_getval, &lpc_int_putval) ||
	!ext_cb(ftabs[5], sizes[5], 2, &lpc_float_getval, &lpc_float_putval) ||
	!ext_cb(ftabs[6], sizes[6], 5, &lpc_string_getval, &lpc_string_putval,
		&lpc_string_new, &lpc_string_text, &lpc_string_length) ||
	!ext_cb(ftabs[7], sizes[7], 5, &lpc_object_name, &lpc_object_isspecial,
		&lpc_object_ismarked, &lpc_object_mark, &lpc_object_unmark) ||
	!ext_cb(ftabs[8], sizes[8], 6, &lpc_array_getval, &lpc_array_putval,
		&lpc_array_new, &lpc_array_index, &lpc_array_assign,
		&lpc_array_size) ||
	!ext_cb(ftabs[9], sizes[9], 6, &lpc_mapping_getval, &lpc_mapping_putval,
		&lpc_mapping_new, &lpc_mapping_index, &lpc_mapping_assign,
		&lpc_mapping_size)) {
	return 0;
    }

    lpc_ext_init();
}