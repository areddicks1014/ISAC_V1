
#include "main.h"
  
static Window *s_main_window;
static GFont t_font;
static GFont d_font;
static GFont b_font;
static TextLayer *s_time_layer_i;
static TextLayer *s_time_layer_m;
static TextLayer *s_date_layer;
static TextLayer *s_day_layer;
static TextLayer *s_battery_percent;
static Layer *s_canvas_layer;
static int s_battery_level;
static GBitmap *s_bitmap_x;
static GBitmap *s_bitmap_bt;
static GBitmap *s_bitmap_icon;
static GColor8 theme_color;
static BitmapLayer *s_layer_bt;
static BitmapLayer *s_layer_icon;
int s_icon_id;
bool s_color_mode;

ClaySettings settings;

// Initialize the default settings
static void prv_default_settings() {
  settings.ThemeColor = GColorOrange;
  settings.CustomColor = GColorOrange;
  settings.ThemeIcon = 0;
  settings.ColorMode = false;
  s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_PHOENIX_WHITE);
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

// Update the display elements
static void prv_update_display() {
  theme_color = settings.ThemeColor;
  s_icon_id = settings.ThemeIcon;
  switch (s_icon_id) {
    case 0:
      s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_TECH);
      bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
      break;
    case 1: 
      s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_SECURITY);
      bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
      break;
    case 2:
      s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_MEDICAL);
      bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
      break;
    case 3:
      s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_DZ);
      bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
      break;
    case 4: 
      s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_ROGUE);
      bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
      break;
    case 5:
      s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_PHOENIX_WHITE);
      bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
      break;
    default: 
      s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_PHOENIX_WHITE);
      bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
      break;
  }
  s_color_mode = settings.ColorMode;
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Read mode preferences
  Tuple *mode_t = dict_find(iter, MESSAGE_KEY_mode);
  if(strcmp(mode_t->value->cstring, "true") ==0){
    s_color_mode = true;
  }else {
    s_color_mode = false;
  }
  
  
  // Read color preferences
  
  Tuple *theme_t = dict_find(iter, MESSAGE_KEY_theme);
  
  if(strcmp(theme_t->value->cstring, "tech") == 0){
    settings.ThemeColor = GColorOrange;
    settings.ThemeIcon = 0;
    s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_TECH);
    bitmap_layer_set_compositing_mode(s_layer_icon, GCompOpSet);
    bitmap_layer_set_background_color(s_layer_icon, GColorClear);
    bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
  }else if(strcmp(theme_t->value->cstring, "security") == 0){
    settings.ThemeColor = GColorBlueMoon;
    settings.ThemeIcon = 1;
    s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_SECURITY);
    bitmap_layer_set_compositing_mode(s_layer_icon, GCompOpSet);
    bitmap_layer_set_background_color(s_layer_icon, GColorClear);
    bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
  }else if(strcmp(theme_t->value->cstring, "medical") == 0){
    settings.ThemeColor = GColorGreen;
    settings.ThemeIcon = 2;
    s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_MEDICAL);
    bitmap_layer_set_compositing_mode(s_layer_icon, GCompOpSet);
    bitmap_layer_set_background_color(s_layer_icon, GColorClear);
    bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
  }else if(strcmp(theme_t->value->cstring, "dark_zone") == 0){
    settings.ThemeColor = GColorPurple;
    settings.ThemeIcon = 3;
    s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_DZ);
    bitmap_layer_set_compositing_mode(s_layer_icon, GCompOpSet);
    bitmap_layer_set_background_color(s_layer_icon, GColorClear);
    bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
  }else if(strcmp(theme_t->value->cstring, "rogue") == 0){
    settings.ThemeColor = GColorRed;
    settings.ThemeIcon = 4;
    s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_ROGUE);
    bitmap_layer_set_compositing_mode(s_layer_icon, GCompOpSet);
    bitmap_layer_set_background_color(s_layer_icon, GColorClear);
    bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
  }
  
  Tuple *custom_t = dict_find(iter, MESSAGE_KEY_custom);
  if(custom_t && (strcmp(theme_t->value->cstring, "custom") == 0)) {
    settings.ThemeColor = GColorFromHEX(custom_t->value->int32);
    settings.ThemeIcon = 5;
    s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_PHOENIX_WHITE);
    bitmap_layer_set_compositing_mode(s_layer_icon, GCompOpSet);
    bitmap_layer_set_background_color(s_layer_icon, GColorClear);
    bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);
  }
  
  
  
  /**
  // Read boolean preferences
  Tuple *time_hours_t = dict_find(iter, MESSAGE_KEY_time);
  if(time_hours_t) {
    settings.Time = time_hours_t->value->int32 == 1;
  }  **/
  
  // Save the new settings to persistent storage
  prv_save_settings();
  layer_mark_dirty(s_canvas_layer);

}

static void bluetooth_callback(bool connected) {
  // Show icon if disconnected
  layer_set_hidden(bitmap_layer_get_layer(s_layer_bt), connected);
  if(!connected) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
}

static void battery_callback(BatteryChargeState state) {
  // Record the new battery level
  s_battery_level = state.charge_percent;
  
  static char battery_buffer[3];
  if (state.is_charging) {
    snprintf(battery_buffer, sizeof(battery_buffer), "CH");
    text_layer_set_text_color(s_battery_percent, GColorWhite);
    //potentially do something else here
  } else {
    snprintf(battery_buffer, sizeof(battery_buffer), "%d", state.charge_percent);
  }
  
  if (state.charge_percent == 100) {
    //placeholder until I can put phoenix image over the circle
    snprintf(battery_buffer, sizeof(battery_buffer), "FL");
  }
  
  text_layer_set_text(s_battery_percent, battery_buffer);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char bufferH[] = "00";
  static char bufferI[] = "00";
  static char bufferM[] = "00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 24 hour format
    //strftime(buffer, sizeof("00 00"), "%H %M", tick_time);
    strftime(bufferH, sizeof("00"), "%H", tick_time);
    strftime(bufferM, sizeof("00"), "%M", tick_time);
    text_layer_set_text(s_time_layer_i, bufferH);
  } else { 
    //Use 12 hour format
    //strftime(buffer, sizeof("00 00"), "%I %M", tick_time);
    strftime(bufferI, sizeof("00"), "%I", tick_time);
    strftime(bufferM, sizeof("00"), "%M", tick_time);
    text_layer_set_text(s_time_layer_i, bufferI);
  }

  //strftime(bufferI, sizeof("00"), "%I", tick_time);
  //strftime(bufferM, sizeof("00"), "%M", tick_time);
  
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%b %d", tick_time);
  static char day_buffer[8];
  strftime(day_buffer, sizeof(day_buffer), "%a", tick_time);
  

  // Display this time on the TextLayer
  
  text_layer_set_text(s_time_layer_m, bufferM);
  text_layer_set_text(s_date_layer, date_buffer);
  text_layer_set_text(s_day_layer, day_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
  
  //theme_color = GColorOrange;
  
  s_bitmap_x = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_X);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  
  
  // -----------Hour Rectangle-------------
  #ifdef PBL_COLOR 
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, theme_color);
  #else 
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorDarkGray);
  #endif 
  
  // Set the stroke width (must be an odd integer value)
  graphics_context_set_stroke_width(ctx, 1);
  
  GRect rect_hour = GRect(12, 12, 60, 60);
  graphics_draw_rect(ctx, rect_hour);
  graphics_fill_rect(ctx, rect_hour, 3, GCornersAll);
  
  //-----------Month/Day Date Rect------------
  #ifdef PBL_COLOR 
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, theme_color);
  #else 
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorDarkGray);
  #endif 
  
  graphics_context_set_stroke_width(ctx, 1);
  GRect rect_current = GRect(12, 82, 60, 40);
  graphics_draw_rect(ctx, rect_current);
  graphics_fill_rect(ctx, rect_current, 3, GCornersAll);
  
  //graphics_draw_bitmap_in_rect(ctx, s_bitmap_x, GRect(12, 82, 30, 30));

  
  //-----------Day of Week Rect--------------
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorDarkGray);
  
  graphics_context_set_stroke_width(ctx, 1);
  GRect rect_forecast = GRect(12, 132, 60, 25);
  graphics_draw_rect(ctx, rect_forecast);
  graphics_fill_rect(ctx, rect_forecast, 3, GCornersAll);
  
  //graphics_draw_bitmap_in_rect(ctx, s_bitmap_x, GRect(12, 122, 30, 30));
  
  
  //--------------Battery Circle BG--------------
  int32_t angle_start_bg = DEG_TO_TRIGANGLE(0);
  int32_t angle_end_bg = DEG_TO_TRIGANGLE(360);
  graphics_context_set_stroke_color(ctx, GColorDarkGray);
  graphics_context_set_stroke_width(ctx, 4);
  graphics_draw_arc(ctx, GRect(84, 114, 40, 40), GOvalScaleModeFitCircle, angle_start_bg, angle_end_bg);
  
  //--------------Battery Circle--------------
  int battery_arc = (int)(float)(((float)(s_battery_level * 360) / 100));
  int32_t angle_start = DEG_TO_TRIGANGLE(0);
  int32_t angle_end = DEG_TO_TRIGANGLE(battery_arc);
  #ifdef PBL_COLOR 
  graphics_context_set_stroke_color(ctx, theme_color);
  #else 
  graphics_context_set_stroke_color(ctx, GColorWhite);
  #endif 
  
  graphics_context_set_stroke_width(ctx, 4);
  graphics_draw_arc(ctx, GRect(84, 114, 40, 40), GOvalScaleModeFitCircle, angle_start, angle_end);
}

static void main_window_load(Window *window) {
  
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  // Create canvas layer
  s_canvas_layer = layer_create(bounds);
  
  // Assign the custom drawing procedure
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  
  // Load the custom font
  t_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORDA_MEDIUM_48));
  d_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORDA_BOLD_18));
  b_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORDA_BOLD_22));
  
  // Create time/date/battery TextLayer
  s_time_layer_i = text_layer_create(GRect(12, 12, 60, 60));
  s_time_layer_m = text_layer_create(GRect(78, 12, 60, 60));
  s_date_layer = text_layer_create(GRect(12, 90, 60, 40));
  s_day_layer = text_layer_create(GRect(12, 132, 60, 25));
  s_battery_percent = text_layer_create(GRect(85, 120, 40, 40));
  
  
  // Text Color
  text_layer_set_background_color(s_time_layer_i, GColorClear);
  text_layer_set_text_color(s_time_layer_i, GColorWhite);
  
  text_layer_set_background_color(s_time_layer_m, GColorClear);
  text_layer_set_text_color(s_time_layer_m, GColorWhite);
  
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, GColorWhite);
  
  text_layer_set_background_color(s_battery_percent, GColorClear);
  text_layer_set_text_color(s_battery_percent, GColorWhite);
  
  
  text_layer_set_text(s_time_layer_i, "00");
  text_layer_set_text(s_time_layer_m, "00");
  
  //------------BT Layer-----------------
  s_bitmap_bt = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NO_BT);
  s_layer_bt = bitmap_layer_create(GRect(94, 82, 24, 24));
  bitmap_layer_set_compositing_mode(s_layer_bt, GCompOpSet);
  bitmap_layer_set_background_color(s_layer_bt, GColorRed);
  bitmap_layer_set_bitmap(s_layer_bt, s_bitmap_bt);
  
  //------------Icon Layer-----------------
  //s_bitmap_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_TECH);
  
  s_layer_icon = bitmap_layer_create(GRect(94, 82, 24, 24));
  bitmap_layer_set_compositing_mode(s_layer_icon, GCompOpSet);
  bitmap_layer_set_background_color(s_layer_icon, GColorClear);
  bitmap_layer_set_bitmap(s_layer_icon, s_bitmap_icon);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer_i, t_font);
  text_layer_set_font(s_time_layer_m, t_font);
  text_layer_set_font(s_date_layer, d_font);
  text_layer_set_font(s_day_layer, d_font);
  text_layer_set_font(s_battery_percent, b_font);
  
  text_layer_set_text_alignment(s_time_layer_i, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_m, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_battery_percent, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), s_canvas_layer);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_i));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_m));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_percent));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_layer_icon));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_layer_bt));
  
  // Make sure the time is displayed from the start
  update_time();
  
  prv_update_display();
  
  // Redraw this as soon as possible
  layer_mark_dirty(s_canvas_layer);
  
  // Show the correct state of the BT connection from the start
  bluetooth_callback(connection_service_peek_pebble_app_connection());
  // Only tick once a minute, much more time asleep
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer_i);
  text_layer_destroy(s_time_layer_m);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_battery_percent);
  bitmap_layer_destroy(s_layer_bt);
  bitmap_layer_destroy(s_layer_icon);
  gbitmap_destroy(s_bitmap_x);
  gbitmap_destroy(s_bitmap_bt);
  gbitmap_destroy(s_bitmap_icon);
}
  
static void init() {
  
  prv_load_settings();
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  // Window Color
  #ifdef PBL_COLOR
  window_set_background_color(s_main_window, GColorBlack);
  #else
  window_set_background_color(s_main_window, GColorBlack);
  #endif

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);
  // Ensure battery level is displayed from the start
  battery_callback(battery_state_service_peek());
  // Register for Bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  

  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
