#include <common.h>
#include <net.h>
#include <netdev.h>

phys_size_t initdram(int board_type) {
	return CONFIG_SYS_SDRAM_SIZE;	/* in bytes */
}


#ifdef CONFIG_DRIVER_DM9000
int board_eth_init(bd_t *bis) {
	return dm9000_initialize(bis);
}
#endif
