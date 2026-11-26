#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "../includes.h"
#include "../error.h"
#include "../render.h"

extern palette mode13h_palette;
uint32_t mode13h_lut[PALSIZE];

extern buffer *front_buf;

struct wl_display *display;
struct wl_compositor* compositor;
struct wl_surface* surface;
struct wl_buffer* wl_buf;
struct wl_shm* sharedmem;
struct xdg_wm_base* shell;
struct xdg_toplevel* top;
struct xdg_surface* xdg_surface;
struct wl_seat* seat;
struct wl_keyboard* keyboard;

uint16_t window_width = GAME_HRES;
uint16_t window_height = GAME_VRES;
uint32_t *pixbuf;

int allocate_sharedmem(uint64_t size) {
    char name[8];
    name[0] = '/';
    name[7] = 0;
    for (int i = 1; i < 6; i++)
        name[i] = (rand() & 23) + 97;

    int fd = shm_open(name,
            O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);
    shm_unlink(name);
    ftruncate(fd, size);

    return fd;
}

void resize() {
    int fd = allocate_sharedmem(window_width * window_height * 4);

    pixbuf = mmap(NULL, window_width * window_height * sizeof(uint32_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct wl_shm_pool* pool = wl_shm_create_pool(sharedmem, fd,
            window_width * window_height * sizeof(uint32_t));
    wl_buf = wl_shm_pool_create_buffer(pool, 0, window_width, window_height,
            window_width * sizeof(uint32_t), WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);
}

void xdg_surface_configure(void* data, struct xdg_surface *xdg_surface,
        uint32_t serial){
    xdg_surface_ack_configure(xdg_surface, serial);
    if (!pixbuf) resize();
}

struct xdg_surface_listener surface_listener = {
    .configure = xdg_surface_configure
};

void xdg_toplevel_configure(void* data, struct xdg_toplevel *top, int32_t
        new_width, int32_t new_height, struct wl_array* stat){
    if (!new_width && !new_height) return;
}

void xdg_toplevel_close(void *data, struct xdg_toplevel *top) {
    // close flag or something idk
}

struct xdg_toplevel_listener top_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close
};

void shell_ping(void *data, struct xdg_wm_base *shell, uint32_t serial){
    xdg_wm_base_pong(shell, serial);
}

struct xdg_wm_base_listener shell_listener = {
    .ping = shell_ping
};

static void registry_handle_global(void *data, struct wl_registry *registry,
		uint32_t name, const char *interface, uint32_t version) {
    if (strcmp(interface, wl_compositor_interface.name) == 0)
        compositor = wl_registry_bind(registry, name, &wl_compositor_interface,
                4);
    else if (strcmp(interface, wl_shm_interface.name) == 0)
        sharedmem = wl_registry_bind(registry, name, &wl_shm_interface, 1);
    else if (strcmp(interface, xdg_wm_base_interface.name)) {
        shell = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(shell, &shell_listener, NULL);
    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        // TODO: Wayland seat interface; input handling
    }
}

static void registry_handle_global_remove(void *data,
        struct wl_registry *registry, uint32_t name) {
}

static const struct wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};

void render_init(void){
    // first off, populate the LUT for the palette
    for (int i = 0; i < PALSIZE; i++)
        mode13h_lut[i] = (uint32_t)mode13h_palette[i].padding << 6
            ^ (uint32_t)mode13h_palette[i].r << 4
            ^ (uint32_t)mode13h_palette[i].g << 2
            ^ (uint32_t)mode13h_palette[i].b;

    // and now we initialize our Wayland client
    display = wl_display_connect(NULL);
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);

    surface = wl_compositor_create_surface(compositor);

    xdg_surface = xdg_wm_base_get_xdg_surface(shell, surface);
    xdg_surface_add_listener(xdg_surface, &surface_listener, NULL);
    top = xdg_surface_get_toplevel(xdg_surface);
    xdg_toplevel_add_listener(top, &top_listener, NULL);
    xdg_toplevel_set_title(top, "Artificial Threat");
    wl_surface_commit(surface);
}
void render_frame(void){
    for (int y = 0; y < GAME_VRES; y++)
        for (int x = 0; x < GAME_HRES; x++)
            pixbuf[y * window_width + x]
                = mode13h_lut[front_buf->bitmap[y * GAME_HRES + x]];

    wl_surface_attach(surface, wl_buf, 0, 0);
    wl_surface_damage_buffer(surface, 0, 0, GAME_HRES, GAME_VRES);
    wl_surface_commit(surface);
}
void render_cleanup(void){
    if(keyboard) wl_keyboard_release(keyboard);
    // wl_seat_release(seat);
    if(wl_buf) wl_buffer_destroy(wl_buf);
    xdg_toplevel_destroy(top);
    xdg_surface_destroy(xdg_surface);
    wl_surface_destroy(surface);
    wl_display_disconnect(display);
}
