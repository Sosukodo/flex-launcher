#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include "launcher.h"
#include <launcher_config.h>
#include "util.h"
#include "debug.h"

extern config_t config;
extern SDL_RWops *log_file;


// A function to initialize the logging subsystem
int init_log()
{
  // Determine log path
  char log_file_path[MAX_PATH_BYTES];
  #ifdef __unix__
  char log_file_directory[MAX_PATH_BYTES];
  join_paths(log_file_directory, 4, getenv("HOME"), ".local", "share", EXECUTABLE_TITLE);
  make_directory(log_file_directory);
  join_paths(log_file_path, 2, log_file_directory, FILENAME_LOG);
  #else
  join_paths(log_file_path, 2, config.exe_path, FILENAME_LOG);
  #endif

  // Open log
  log_file = SDL_RWFromFile(log_file_path, "w");
  if (log_file == NULL) {
    printf("Failed to create log file\n");
    cleanup();
    exit(1);
  }
  if (config.debug) {
    printf("Debug mode enabled\nLog is outputted to %s\n", log_file_path);
  }
  return 0;
}

// A function to output a log with a printf-style format
void output_log(log_level_t log_level, const char *format, ...)
{
  // Don't output a debug message if we aren't in debug mode
  if (log_level == LOGLEVEL_DEBUG && !config.debug) {
    return;
  }

  // Initialize logging if not already initialized
  else if (log_file == NULL) {
    init_log();
  }
  
  // Output log
  static char buffer[MAX_LOG_LINE_BYTES];
  va_list args;
  va_start(args, format);
  int length = vsnprintf(buffer, MAX_LOG_LINE_BYTES - 1, format, args);
  SDL_RWwrite(log_file, buffer, 1, length);
  
  if (log_level > LOGLEVEL_DEBUG) {
    fputs(buffer, stderr);
  }
  va_end(args);
}
// A function to print the parsed settings to the command line
void debug_settings()
{
  output_log(LOGLEVEL_DEBUG, "======================== Settings ========================\n");
  output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_DEFAULT_MENU,config.default_menu);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_MAX_BUTTONS,config.max_buttons);
  if (config.background_mode == MODE_COLOR) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_BACKGROUND_MODE,"Color");
  }
  else if (config.background_mode == MODE_IMAGE) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_BACKGROUND_MODE,"Image");
  }
  else if (config.background_mode == MODE_SLIDESHOW) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_BACKGROUND_MODE,"Slideshow");
  }
  output_log(LOGLEVEL_DEBUG, "%s R: %i\n",SETTING_BACKGROUND_COLOR,config.background_color.r);
  output_log(LOGLEVEL_DEBUG, "%s G: %i\n",SETTING_BACKGROUND_COLOR,config.background_color.g);
  output_log(LOGLEVEL_DEBUG, "%s B: %i\n",SETTING_BACKGROUND_COLOR,config.background_color.b);
  if (config.background_image != NULL) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_BACKGROUND_IMAGE,config.background_image);
  }
  if (config.slideshow_directory != NULL) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_SLIDESHOW_DIRECTORY ,config.slideshow_directory);
  }
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_SLIDESHOW_IMAGE_DURATION,config.slideshow_image_duration);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_SLIDESHOW_TRANSITION_TIME,config.slideshow_transition_time);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_ICON_SIZE,config.icon_size);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_ICON_SPACING,config.icon_spacing);
  output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_TITLE_FONT,config.title_font_path);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_TITLE_FONT_SIZE,config.font_size);
  output_log(LOGLEVEL_DEBUG, "%s R: %i\n",SETTING_TITLE_COLOR,config.title_color.r);
  output_log(LOGLEVEL_DEBUG, "%s G: %i\n",SETTING_TITLE_COLOR,config.title_color.g);
  output_log(LOGLEVEL_DEBUG, "%s B: %i\n",SETTING_TITLE_COLOR,config.title_color.b);
  output_log(LOGLEVEL_DEBUG, "%s A: %i\n",SETTING_TITLE_COLOR,config.title_color.a);
  if (config.title_oversize_mode == MODE_TEXT_TRUNCATE) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_TITLE_OVERSIZE_MODE,"Truncate");
  }
  else if (config.title_oversize_mode == MODE_TEXT_SHRINK) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_TITLE_OVERSIZE_MODE,"Shrink");
  }
  else if (config.title_oversize_mode == MODE_TEXT_NONE) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_TITLE_OVERSIZE_MODE,"None");
  }
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_TITLE_PADDING,config.title_padding);
  output_log(LOGLEVEL_DEBUG, "%s R: %i\n",SETTING_HIGHLIGHT_COLOR,config.highlight_color.r);
  output_log(LOGLEVEL_DEBUG, "%s G: %i\n",SETTING_HIGHLIGHT_COLOR,config.highlight_color.g);
  output_log(LOGLEVEL_DEBUG, "%s B: %i\n",SETTING_HIGHLIGHT_COLOR,config.highlight_color.b);
  output_log(LOGLEVEL_DEBUG, "%s A: %i\n",SETTING_HIGHLIGHT_COLOR,config.highlight_color.a);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_HIGHLIGHT_CORNER_RADIUS,config.highlight_rx);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_HIGHLIGHT_VPADDING,config.highlight_vpadding);
  output_log(LOGLEVEL_DEBUG, "%s: %i\n",SETTING_HIGHLIGHT_HPADDING,config.highlight_hpadding);
  if (config.scroll_indicators) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_SCROLL_INDICATORS,"true");
  }
  else {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_SCROLL_INDICATORS,"false");
  }
  output_log(LOGLEVEL_DEBUG, "%s R: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config.highlight_color.r);
  output_log(LOGLEVEL_DEBUG, "%s G: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config.highlight_color.g);
  output_log(LOGLEVEL_DEBUG, "%s B: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config.highlight_color.b);
  output_log(LOGLEVEL_DEBUG, "%s A: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config.highlight_color.a);
  if (config.on_launch == MODE_ON_LAUNCH_HIDE) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n", SETTING_ON_LAUNCH,"Hide");
  }
  else if (config.on_launch == MODE_ON_LAUNCH_BLANK) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n", SETTING_ON_LAUNCH,"Blank");
  }
  else if (config.on_launch == MODE_ON_LAUNCH_NONE) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n", SETTING_ON_LAUNCH,"None");
  }
  if (config.reset_on_back) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_RESET_ON_BACK,"true");
  }
  else {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_RESET_ON_BACK,"false");
  }
  if (config.esc_quit) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_ESC_QUIT,"true");
  }
  else {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_ESC_QUIT,"false");
  }
  output_log(LOGLEVEL_DEBUG, "======================== Gamepad ========================\n");
  if (config.gamepad_enabled) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_GAMEPAD_ENABLED, "true");
  }
  else {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n",SETTING_GAMEPAD_ENABLED, "false");
  }
  output_log(LOGLEVEL_DEBUG, "%s: %i\n", SETTING_GAMEPAD_DEVICE, config.gamepad_device);
  if (config.gamepad_mappings_file != NULL) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n", SETTING_GAMEPAD_MAPPINGS_FILE, config.gamepad_mappings_file);
  }
  for (gamepad_control_t *i = config.gamepad_controls; i != NULL; i = i->next) {
    output_log(LOGLEVEL_DEBUG, "%s: %s\n", i->label, i->cmd);
  }
}

// A function to print the parsed menu entries to the command line
void debug_menu_entries(menu_t *first_menu, int num_menus) 
{
  if (first_menu == NULL) {
    output_log(LOGLEVEL_DEBUG, "No valid menus found\n");
    return;
  }
  output_log(LOGLEVEL_DEBUG, "======================= Menu Entries =======================\n");
  menu_t *menu = first_menu;
  entry_t *entry;
  for (int i = 0; i < num_menus; i ++) {
    output_log(LOGLEVEL_DEBUG, "Menu Name: %s\n",menu->name);
    output_log(LOGLEVEL_DEBUG, "Number of Entries: %i\n",menu->num_entries);
    entry = menu->first_entry;
    for (int j = 0; j < menu->num_entries; j++) {
      output_log(LOGLEVEL_DEBUG, "Entry %i Title: %s\n",j,entry->title);
      output_log(LOGLEVEL_DEBUG, "Entry %i Icon Path: %s\n",j,entry->icon_path);
      output_log(LOGLEVEL_DEBUG, "Entry %i Command: %s\n",j,entry->cmd);
      if (j != menu->num_entries - 1) {
        output_log(LOGLEVEL_DEBUG, "\n");
      }
      entry = entry->next;
    }
    if (i != num_menus - 1) {
      output_log(LOGLEVEL_DEBUG, "----------------------------------------------------------\n");
    }
    menu = menu->next;
  }
  output_log(LOGLEVEL_DEBUG, "\n");
}

void debug_slideshow(slideshow_t *slideshow)
{
  output_log(LOGLEVEL_DEBUG, "======================== Slideshow ========================\n");
  output_log(LOGLEVEL_DEBUG, 
             "Found %i images in directory %s:\n", 
             slideshow->num_images, 
             config.slideshow_directory);
  for (int i = 0; i < slideshow->num_images; i++) {
    output_log(LOGLEVEL_DEBUG, "%s\n", slideshow->images[slideshow->order[i]]);
  }
}


void debug_video(SDL_Renderer *renderer)
{
  SDL_RendererInfo info;
  SDL_GetRendererInfo(renderer, &info);
  output_log(LOGLEVEL_DEBUG, "Supported Texture formats:\n");
  for(int i = 0; i < info.num_texture_formats; i++) {
    output_log(LOGLEVEL_DEBUG, "%s\n", SDL_GetPixelFormatName(info.texture_formats[i]));
  }
}

// A function to print the current button geometry to the command line
void debug_button_positions(entry_t *entry, menu_t *current_menu, geometry_t *geo)
{
  output_log(LOGLEVEL_DEBUG, "================= Menu \"%s\" Page %i =================\n",
         current_menu->name,
         current_menu->page);
  int left_margin;
  int right_margin;
  for (int i = 0; i < geo->num_buttons; i++) {
    if (i == 0) {
      left_margin = entry->icon_rect.x;
    }
    else if (i == geo->num_buttons - 1) {
      right_margin = geo->screen_width - (entry->icon_rect.x + entry->icon_rect.w);
    }
    output_log(LOGLEVEL_DEBUG, "Button %i:\n",i);
    output_log(LOGLEVEL_DEBUG, "Icon X1: %i\n",entry->icon_rect.x);
    output_log(LOGLEVEL_DEBUG, "Icon X2: %i\n",entry->icon_rect.x + entry->icon_rect.w);
    output_log(LOGLEVEL_DEBUG, "Icon Y1: %i\n",entry->icon_rect.y);
    output_log(LOGLEVEL_DEBUG, "Icon Y2: %i\n",entry->icon_rect.y + entry->icon_rect.h);
    output_log(LOGLEVEL_DEBUG, "Text X1: %i\n",entry->text_rect.x);
    output_log(LOGLEVEL_DEBUG, "Text X2: %i\n",entry->text_rect.x + entry->icon_rect.w);
    output_log(LOGLEVEL_DEBUG, "Text Y1: %i\n",entry->text_rect.y);
    output_log(LOGLEVEL_DEBUG, "Text Y2: %i\n\n",entry->text_rect.y + entry->icon_rect.h);
    entry = entry->next;
  }
  output_log(LOGLEVEL_DEBUG, "Total Buttons: %i\n",geo->num_buttons);
  output_log(LOGLEVEL_DEBUG, "Left Margin: %i\n",left_margin);
  output_log(LOGLEVEL_DEBUG, "Right Margin: %i\n",right_margin);
}
