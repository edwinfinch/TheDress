#include <pebble.h>

Window *window;
TextLayer *time_layer;
GBitmap *the_dress_bitmap;
BitmapLayer *the_dress_layer;

void tick_handler(struct tm *t, TimeUnits units){
  static char time_buffer[] = "00:00";
  if(clock_is_24h_style()){
    strftime(time_buffer, sizeof(time_buffer), "%I:%M", t);
  }
  else{
    strftime(time_buffer, sizeof(time_buffer), "%H:%M", t);
  }
  text_layer_set_text(time_layer, time_buffer);
}

void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  the_dress_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_THE_DRESS);

  the_dress_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(the_dress_layer, the_dress_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(the_dress_layer));

  time_layer = text_layer_create(GRect(15, 0, 144, 168));
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(time_layer));

  struct tm *t;
  time_t temp;        
  temp = time(NULL);        
  t = localtime(&temp);

  tick_handler(t, MINUTE_UNIT);
}

void window_unload(Window *window) {
 text_layer_destroy(time_layer);
}

void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

void deinit() {
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
