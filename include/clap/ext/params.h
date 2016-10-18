﻿#ifndef CLAP_EXT_PARAMS_H
# define CLAP_EXT_PARAMS_H

# include "../clap.h"
# include "ports.h"

#ifdef __cplusplus
extern "C" {
#endif

# define CLAP_EXT_PARAMS "clap/params"
# define CLAP_ROOT_MODULE_ID ""

enum clap_param_type
{
  CLAP_PARAM_FLOAT = 0, // uses value.f
  CLAP_PARAM_BOOL  = 1, // uses value.b
  CLAP_PARAM_INT   = 2, // uses value.i
  CLAP_PARAM_ENUM  = 3, // uses value.i
};

/* This is useful for the knob scaling. */
enum clap_param_scale
{
  CLAP_PARAM_SCALE_LINEAR = 0,
  CLAP_PARAM_SCALE_LOG    = 1,
  CLAP_PARAM_SCALE_EXP    = 2,
};

/* This describes the parameter and provides the current value */
struct clap_param
{
  int32_t index;  // parameter's index

  /* param info */
  char                    module_id[CLAP_ID_SIZE];
  char                    id[CLAP_ID_SIZE]; // a string which identify the param
  char                    name[CLAP_NAME_SIZE]; // the display name
  char                    desc[CLAP_DESC_SIZE];
  char                    display[CLAP_DISPLAY_SIZE]; // the text used to display the value
  bool                    is_per_note;
  bool                    is_per_channel;
  bool                    is_used;   // is this parameter used by the patch?
  bool                    is_periodic; // after the last value, go back to the first one
  bool                    is_locked; // if true, the parameter can't be changed by
                                     // the host

  /* Can the parameter be automated at sample rate by an audio buffer? */
  bool                    supports_audio_automation;

  /* Can the parameter be automated by automation events? */
  bool                    supports_events_automation;
  bool                    supports_ramp_events;

  /* value */
  enum clap_param_type    type;
  union clap_param_value  value; // current value
  union clap_param_value  min;   // minimum value
  union clap_param_value  max;   // maximum value
  union clap_param_value  deflt; // default value
  enum  clap_param_scale  scale; // scaling of the knob in the UI.
};

struct clap_param_module
{
  char id[CLAP_ID_SIZE];
  char parent_id[CLAP_ID_SIZE];
  char name[CLAP_NAME_SIZE];
  char desc[CLAP_DESC_SIZE];
};

struct clap_plugin_params
{
  /* Returns the number of parameters.
   * [audio-thread] */
  int32_t (*count)(struct clap_plugin *plugin);

  /* Copies the parameter's info to param and returns true.
   * If index is greater or equal to the number then return false.
   * [audio-thread] */
  bool (*get_param)(struct clap_plugin *plugin,
                    int32_t             index,
                    struct clap_param  *param);

  /* Copies the module's info to module and returns true.
   * If module_id is NULL or invalid then return false.
   * [audio-thread] */
  bool (*get_module)(struct clap_plugin       *plugin,
                     const char               *module_id,
                     struct clap_param_module *module);

  /* Use an audio buffer to automate a parameter at sample rate.
   * Once a parameter is automated by an audio buffer, concurrent
   * automation event shall be ignored in favor of the audio rate
   * automation.
   *
   * To disconnect the automation, set buffer to NULL.
   * [audio-thread] */
  bool (*set_param_port)(struct clap_plugin     *plugin,
                         int32_t                 param_index,
                         int32_t                 channel,
                         int32_t                 note,
                         struct clap_audio_port *port);
};

struct clap_host_params
{
  /* [thread-safe] */
  void (*touch_begin)(struct clap_host   *host,
                      struct clap_plugin *plugin,
                      int32_t             index);

  /* [thread-safe] */
  void (*touch_end)(struct clap_host     *host,
                    struct clap_plugin   *plugin,
                    int32_t               index);

  /* [thread-safe] */
  void (*changed)(struct clap_host       *host,
                  struct clap_plugin     *plugin,
                  int32_t                 index,
                  union clap_param_value  value,
                  bool                    is_recordable);

  /* [thread-safe] */
  void (*rescan)(struct clap_host   *host,
                 struct clap_plugin *plugin);
};

#ifdef __cplusplus
}
#endif

#endif /* !CLAP_EXT_PARAMS_H */
