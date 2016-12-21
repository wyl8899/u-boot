/* Based on serial/serial_xuartlite.c and serial/sandbox.c
 *
 * Created by wyl8899
 */

#include <config.h>
#include <common.h>
#include <dm.h>
#include <asm/io.h>
#include <linux/compiler.h>
#include <serial.h>

struct neptunus {
        unsigned int data;
        unsigned int status;
};

struct neptunus_platdata {
        struct neptunus *regs;
};

#define NEPTUNUS_WRITABLE 0x01
#define NEPTUNUS_READABLE 0x02

#define in_32(addr) (*(volatile unsigned int *)addr)
#define out_32(addr, data) (*(volatile unsigned int*)addr = data)

static int neptunus_putc(struct udevice *dev, const char ch)
{
        struct neptunus_platdata *plat = dev_get_platdata(dev);
        struct neptunus *regs = plat->regs;

        if (!(in_32(&regs->status) & NEPTUNUS_WRITABLE)) {
                return -EAGAIN;
        }
        out_32(&regs->data, ch & 0xff);

        return 0;
}

static int neptunus_getc(struct udevice *dev)
{
        struct neptunus_platdata *plat = dev_get_platdata(dev);
        struct neptunus *regs = plat->regs;

        if (!(in_32(&regs->status) & NEPTUNUS_READABLE)) {
                return -EAGAIN;
        }

        return in_32(&regs->data) & 0xff;
}

static int neptunus_pending(struct udevice *dev, bool input)
{
        struct neptunus_platdata *plat = dev_get_platdata(dev);
        struct neptunus *regs = plat->regs;

        if (input) {
            return in_32(&regs->status) & NEPTUNUS_READABLE;
        } else {
            return 0;
        }
}

static int neptunus_probe(struct udevice *dev)
{
        // struct neptunus_platdata *plat = dev_get_platdata(dev);
        // struct neptunus *regs = plat->regs;
        //
        // nothing need to be done
        return 0;
}

static int neptunus_ofdata_to_platdata(struct udevice *dev)
{
        struct neptunus_platdata *plat = dev_get_platdata(dev);

        plat->regs = (struct neptunus *)dev_get_addr(dev);

        return 0;
}

static const struct dm_serial_ops neptunus_ops = {
        .putc    = neptunus_putc,
        .pending = neptunus_pending,
        .getc    = neptunus_getc,
};

static const struct udevice_id neptunus_ids[] = {
        { .compatible = "neptunus,serial", },
        { }
};

U_BOOT_DRIVER(serial_neptunus) = {
        .name   = "serial_neptunus",
        .id     = UCLASS_SERIAL,
        .of_match = neptunus_ids,
        .ofdata_to_platdata = neptunus_ofdata_to_platdata,
        .platdata_auto_alloc_size = sizeof(struct neptunus_platdata),
        .probe = neptunus_probe,
        .ops    = &neptunus_ops,
        .flags = DM_FLAG_PRE_RELOC,
};
