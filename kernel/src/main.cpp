#include <limine.h>
#include <libs/libc.h>
#include <libs/flanterm/flanterm.h>
#include <libs/flanterm/backends/fb.h>

#include "core/io.h"
#include "core/gdt.h"
#include "core/idt.h"
#include "core/pic.h"
#include "core/irq.h"

#include "core/IRHandlers/RTC.h"
#include "core/IRHandlers/Keyboard.h"

#include <utils/logger.h>

// anonymous namspaces are the same as static
namespace
{
__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(3);
}

namespace
{
__attribute__((used, section(".limine_requests")))
volatile limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0,
	.response = nullptr
};
}

namespace
{
__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;
}

static void hcf()
{
	for (;;)
		asm volatile("hlt");
}

flanterm_context *g_ft_ctx = nullptr;
limine_framebuffer *g_fb = nullptr;

// for printf
void putchar_(char c) {
    char str[] = {c};
    flanterm_write(g_ft_ctx, str, 1);
}

extern "C" void kmain()
{
	if (framebuffer_request.response == nullptr ||
		framebuffer_request.response->framebuffer_count < 1) {
		hcf();
	}

	g_fb = framebuffer_request.response->framebuffers[0];
	g_ft_ctx = flanterm_fb_init(
		nullptr,
		nullptr,
		reinterpret_cast<uint32_t*>(g_fb->address), g_fb->width, g_fb->height, g_fb->pitch,
		g_fb->red_mask_size, g_fb->red_mask_shift,
		g_fb->green_mask_size, g_fb->green_mask_shift,
		g_fb->blue_mask_size, g_fb->blue_mask_shift,
		nullptr,
		nullptr, nullptr,
		nullptr, nullptr,
		nullptr, nullptr,
		nullptr, 0, 0, 1,
		0, 0,
		0
	);

	if (!IO::Init()) {
		log(INFO, "Failed to initialize IO(Port)!\n");
    }
	else {
		log(INFO, "IO(Port) - Initialized!\n");
    }

	GDT::Manager::Init();
	IDT::Manager::Init();
	IRQ::Init();
	PIC::Mask(0); // Mask the PIT, no use as of now

	IRQ::RegisterHandlers(8, RTC::InterruptHandler);
	IRQ::RegisterHandlers(1, Keyboard::InterruptHandler);
	RTC::Init();
	Keyboard::Init();

	// greeting text
	printf("\n\n");
	printf("\033[93m");
	printf("<-. (`-')                              (`-').-> \n");
	printf("	\(OO )_      .->            .->    ( OO)_   \n");
	printf("),--./  ,-.) ,--.'  ,-.    (`-')----. (_)--\_)  \n");
	printf("|   `.'   |(`-')'.'  /    ( OO).-.  '/    _ /  \n");
	printf("|  |'.'|  |(OO \    /     ( _) | |  |\_..`--.  \n");
	printf("|  |   |  | |  /   /)      \|  |)|  |.-._)   \ \n");
	printf("|  |   |  | `-/   /`        '  '-'  '\       / \n");
	printf("`--'   `--'   `--'           `-----'  `-----'  \n");
	printf("\033[0m");
	printf("--- Welcome to MY OS! ---\n");
	DateTime &dt = RTC::getDateTime();
    printf("Booted at: %02u-%02u-%02u %02u:%02u:%02u\n\n",
           dt.day, dt.month, dt.year,
           dt.hour, dt.minute, dt.second);
		
	hcf();
}
