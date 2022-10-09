#include <stdint.h>
#include <alix/bus.h>
#include <alix/dev.h>
#include <alix/device.h>
#include <alix/mem.h>
#include <alix/tty.h>

static int ns8250_open(dev_t device, int flags);
static int ns8250_close(dev_t device);
static int ns8250_read(dev_t device);
static int ns8250_write(dev_t device, char c);
static void ns8250_ioctl(dev_t device);

struct dev ns8250 = {
	"ns8250",
	ns8250_open,
	ns8250_close,
	ns8250_read,
	ns8250_write,
	ns8250_ioctl
};

#define BAUD_MAX 115200

#define NS8250_REG_DATA 0
#define NS8250_REG_INTERRUPT_ENABLE 1
#define NS8250_REG_BAUD_DIVISOR_LO 0
#define NS8250_REG_BAUD_DIVISOR_HI 1
#define NS8250_REG_INTERRUPT_ID 2
#define NS8250_REG_LINE_CONTROL 3
#define NS8250_REG_MODEM_CONTROL 4
#define NS8250_REG_LINE_STATUS 5
#define NS8250_REG_MODEM_STATUS 6
#define NS8250_REG_SCRATCH 7

#define NS8250_INTERRUPT_DATA_AVAIL (1 << 0)
#define NS8250_INTERRUPT_TRANSMITTER_EMPTY (1 << 1)
#define NS8250_INTERRUPT_BREAK_ERROR (1 << 2)
#define NS8250_INTERRUPT_STATUS_CHANGE (1 << 3)

#define NS8250_MODEM_CONTROL_DTR (1 << 0) /* data terminal ready pin */
#define NS8250_MODEM_CONTROL_RTS (1 << 1) /* request to send pin */
#define NS8250_MODEM_CONTROL_OUT1 (1 << 2) /* OUT1 pin (unused on pc) */
#define NS8250_MODEM_CONTROL_OUT2 (1 << 3) /* OUT2 pin (enable irq on pc) */
#define NS8250_MODEM_CONTROL_LOOP (1 << 4) /* local loopback mode */

#define NS8250_LINE_CONTROL_BITS5 0x00
#define NS8250_LINE_CONTROL_BITS6 0x01
#define NS8250_LINE_CONTROL_BITS7 0x02
#define NS8250_LINE_CONTROL_BITS8 0x03
#define NS8250_LINE_CONTROL_STOP1 0x00
#define NS8250_LINE_CONTROL_STOP2 0x04
#define NS8250_LINE_CONTROL_PARITY_NONE 0x00
#define NS8250_LINE_CONTROL_PARITY_ODD 0x08
#define NS8250_LINE_CONTROL_PARITY_EVEN 0x18
#define NS8250_LINE_CONTROL_PARITY_MARK 0x28
#define NS8250_LINE_CONTROL_PARITY_SPACE 0x38
#define NS8250_LINE_CONTROL_DLAB 0x80

#define NS8250_LINE_STATUS_DR (1 << 0) /* data ready */
#define NS8250_LINE_STATUS_OE (1 << 1) /* overrun error */
#define NS8250_LINE_STATUS_PE (1 << 2) /* parity error */
#define NS8250_LINE_STATUS_FE (1 << 3) /* framing error */
#define NS8250_LINE_STATUS_BI (1 << 4) /* break indicator */
#define NS8250_LINE_STATUS_THRE (1 << 5) /* transmitter holding register empty */
#define NS8250_LINE_STATUS_TEMT (1 << 6) /* transmitter empty */
#define NS8250_LINE_STATUS_IE (1 << 7) /* impending error */

#define _send(target, value) \
	msg.i = (value); \
	bus->send(target, &msg)

#define _receive(target) \
	bus->receive(target, &msg)

static int
ns8250_open(dev_t device, int flags)
{
	struct tty **config = (struct tty **)&device->config;
	const struct bus *bus = device->bus;
	struct bus_message msg = { BUS_MESSAGE_INT | BUS_MESSAGE_INT8 };
	uint8_t modem;
	uint16_t divisor;

	if (!*config) {
		*config = kalloc(sizeof(struct tty));
	}

	/*if (!*config) {
		return 1;
	}*/

	divisor = BAUD_MAX / 9600;

	_send(device->base + NS8250_REG_INTERRUPT_ENABLE, 0x00);
	_send(device->base + NS8250_REG_LINE_CONTROL, NS8250_LINE_CONTROL_DLAB);
	_send(device->base + NS8250_REG_BAUD_DIVISOR_LO, divisor & 0xff);
	_send(device->base + NS8250_REG_BAUD_DIVISOR_HI, divisor >> 8);
	_send(device->base + NS8250_REG_LINE_CONTROL, NS8250_LINE_CONTROL_BITS8 |
			NS8250_LINE_CONTROL_PARITY_NONE | NS8250_LINE_CONTROL_STOP1);
	//~ _send(device->base + NS8250_REG_INTERRUPT_ID, NS8250_INTERRUPT_DATA_AVAIL |
			//~ NS8250_INTERRUPT_TRANSMITTER_EMPTY | NS8250_INTERRUPT_BREAK_ERROR);
	modem = NS8250_MODEM_CONTROL_OUT2 | NS8250_MODEM_CONTROL_RTS | NS8250_MODEM_CONTROL_DTR;
	//~ _send(device->base + NS8250_REG_MODEM_CONTROL, modem);

	/* test port */
	_send(device->base + NS8250_REG_MODEM_CONTROL, modem | NS8250_MODEM_CONTROL_LOOP);
	_send(device->base + NS8250_REG_DATA, 0x55);
	_receive(device->base + NS8250_REG_DATA);
	if (0x55 != msg.i) {
		kfree(*config);
		return 1;
	}
	_send(device->base + NS8250_REG_DATA, 0xaa);
	_receive(device->base + NS8250_REG_DATA);
	if (0xaa != msg.i) {
		kfree(*config);
		return 1;
	}
	_send(device->base + NS8250_REG_MODEM_CONTROL, modem);

	return 0;
}

static int
ns8250_close(dev_t device)
{
	kfree(device->config);
	return 0;
}

static int
ns8250_read(dev_t device)
{
	const struct bus *bus = device->bus;
	struct bus_message msg = { BUS_MESSAGE_INT | BUS_MESSAGE_INT8 };
	uint8_t lflags = NS8250_LINE_STATUS_DR | NS8250_LINE_STATUS_BI |
			NS8250_LINE_STATUS_PE;

	do {
		_receive(device->base + NS8250_REG_LINE_STATUS);
	} while (!(msg.i & lflags));
	if (!(msg.i & NS8250_LINE_STATUS_DR)) {
		return -1;
	}
	_receive(device->base + NS8250_REG_DATA);
	return msg.i;
}

static int
ns8250_write(dev_t device, char c)
{
	const struct bus *bus = device->bus;
	struct bus_message msg = { BUS_MESSAGE_INT | BUS_MESSAGE_INT8 };

	_send(device->base + NS8250_REG_DATA, c);
	return 0;
}

static void
ns8250_ioctl(dev_t device)
{

}
