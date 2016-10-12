#include <pebble.h>
#include "common.h"
static Window *s_window = NULL;
static Layer *s_main_layer = NULL;
static TextLayer *s_text_layer;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;

  // Prepare the outbox buffer for this message
  app_message_outbox_begin(&out_iter);
  dict_write_cstring(out_iter, SEND_DATA, "select");
  dict_write_end(out_iter);
  app_message_outbox_send();
}
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "UP");
  // Prepare the outbox buffer for this message
  app_message_outbox_begin(&out_iter);
  dict_write_cstring(out_iter, SEND_DATA, "up");
  dict_write_end(out_iter);
  app_message_outbox_send();
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Down");
  // Prepare the outbox buffer for this message
  app_message_outbox_begin(&out_iter);
  dict_write_cstring(out_iter, SEND_DATA, "down");
  dict_write_end(out_iter);
  app_message_outbox_send();
}
static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_main_layer = layer_create(bounds);
  s_text_layer = text_layer_create(bounds);
  text_layer_set_text(s_text_layer, "blank");
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  layer_add_child(window_layer, s_main_layer);
}

static void window_unload(Window *window) {
  layer_destroy(s_main_layer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped. Reason: %d", (int)reason);
}
static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  Tuple *message_tuple = dict_find(iter, SEND_DATA);
  static char receivedMessage[100];

  memset(receivedMessage, '\0', 100);
  if(message_tuple) {
    strncpy(receivedMessage, message_tuple->value->cstring, sizeof(receivedMessage));
    text_layer_set_text(s_text_layer, receivedMessage);
  }
}
static void outbox_sent_callback(DictionaryIterator *iter, void *context) {
}
static void outbox_failed_callback(DictionaryIterator *iter, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message send failed. Reason: %d", (int)reason);
}

static void init(void) {
  
  s_window = window_create();
  window_set_background_color(s_window, PBL_IF_COLOR_ELSE(GColorChromeYellow, GColorBlack));
    window_set_click_config_provider(s_window, click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
}

static void deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}