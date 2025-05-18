#include <limine.h>
#include <libs/libc.h>
#include <libs/printf.h>
#include <libs/flanterm/flanterm.h>
#include <libs/flanterm/backends/fb.h>

#include "core/gdt.h"
#include "core/idt.h"

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
		__asm__ volatile("hlt");
}

flanterm_context *g_ft_ctx = nullptr;

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

	limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

	g_ft_ctx = flanterm_fb_init(
		nullptr,
		nullptr,
		static_cast<uint32_t*>(fb->address), fb->width, fb->height, fb->pitch,
		fb->red_mask_size, fb->red_mask_shift,
		fb->green_mask_size, fb->green_mask_shift,
		fb->blue_mask_size, fb->blue_mask_shift,
		nullptr,
		nullptr, nullptr,
		nullptr, nullptr,
		nullptr, nullptr,
		nullptr, 0, 0, 1,
		0, 0,
		0
	);

	GDT::Init();
	IDT::Init();

	hcf();
}