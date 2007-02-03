/* 
 * FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 * Copyright (C) 2005/2006, Anthony Minessale II <anthmct@yahoo.com>
 *
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 *
 * The Initial Developer of the Original Code is
 * Anthony Minessale II <anthmct@yahoo.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * 
 * Anthony Minessale II <anthmct@yahoo.com>
 * Neal Horman <neal at wanlink dot com>
 *
 *
 * switch_ivr.h -- IVR Library
 *
 */
/** 
 * @file switch_ivr.h
 * @brief IVR Library
 * @see switch_ivr
 */

#ifndef SWITCH_IVR_H
#define SWITCH_IVR_H

#include <switch.h>

SWITCH_BEGIN_EXTERN_C

/**
 * @defgroup switch_ivr IVR Library
 * @ingroup core1
 *	A group of core functions to do IVR related functions designed to be 
 *	building blocks for a higher level IVR interface.
 * @{
 */


/*!
  \brief Parse command from an event
  \param session the session to send the message to
  \param event the event to send
  \return SWITCH_STATUS_SUCCESS if successful
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_parse_event(switch_core_session_t *session, switch_event_t *event);

/*!
  \brief Wait for time to pass for a specified number of milliseconds
  \param session the session to wait for.
  \param ms the number of milliseconds
  \return SWITCH_STATUS_SUCCESS if the channel is still up
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_sleep(switch_core_session_t *session, uint32_t ms);

SWITCH_DECLARE(switch_status_t) switch_ivr_park(switch_core_session_t *session);

/*!
  \brief Wait for DTMF digits calling a pluggable callback function when digits are collected.
  \param session the session to read.
  \param dtmf_callback code to execute if any dtmf is dialed during the recording
  \param buf an object to maintain across calls
  \param buflen the size of buf
  \param timeout a timeout in milliseconds
  \return SWITCH_STATUS_SUCCESS to keep the collection moving.
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_collect_digits_callback(switch_core_session_t *session,
                                                                   switch_input_args_t *args,
																   uint32_t timeout);

/*!
  \brief Wait for specified number of DTMF digits, untile terminator is received or until the channel hangs up.
  \param session the session to read.
  \param buf strig to write to
  \param buflen max size of buf
  \param maxdigits max number of digits to read
  \param terminators digits to end the collection
  \param terminator actual digit that caused the collection to end (if any)
  \param timeout timeout in ms
  \return SWITCH_STATUS_SUCCESS to keep the collection moving.
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_collect_digits_count(switch_core_session_t *session,
																char *buf,
																uint32_t buflen,
																uint32_t maxdigits,
																const char *terminators,
																char *terminator,
																uint32_t timeout);

/*!
  \brief Engage background Speech detection on a session
  \param session the session to attach
  \param mod_name the module name of the ASR library
  \param grammar the grammar name
  \param path the path to the grammar file
  \param dest the destination address
  \param ah an ASR handle to use (NULL to create one)
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_detect_speech(switch_core_session_t *session,
														 char *mod_name,
														 char *grammar,
														 char *path,
														 char *dest,
														 switch_asr_handle_t *ah);

/*!
  \brief Stop background Speech detection on a session
  \param session The session to stop detection on
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_stop_detect_speech(switch_core_session_t *session);

/*!
  \brief Pause background Speech detection on a session
  \param session The session to pause detection on
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_pause_detect_speech(switch_core_session_t *session);

/*!
  \brief Resume background Speech detection on a session
  \param session The session to resume detection on
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_resume_detect_speech(switch_core_session_t *session);

/*!
  \brief Load a grammar on a background speech detection handle
  \param session The session to change the grammar on
  \param grammar the grammar name
  \param path the grammar path
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_detect_speech_load_grammar(switch_core_session_t *session, char *grammar, char *path);

/*!
  \brief Unload a grammar on a background speech detection handle
  \param session The session to change the grammar on
  \param grammar the grammar name
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_detect_speech_unload_grammar(switch_core_session_t *session, char *grammar);

/*!
  \brief Record a session to disk
  \param session the session to record
  \param file the path to the file
  \param fh file handle to use (NULL for builtin one)
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_record_session(switch_core_session_t *session, char *file,  switch_file_handle_t *fh);

/*!
  \brief Stop Recording a session
  \param session the session to stop recording
  \param file the path to the file
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_stop_record_session(switch_core_session_t *session, char *file);

SWITCH_DECLARE(switch_status_t) switch_ivr_inband_dtmf_session(switch_core_session_t *session);
SWITCH_DECLARE(switch_status_t) switch_ivr_stop_inband_dtmf_session(switch_core_session_t *session);

/*!
  \brief play a file from the disk to the session
  \param session the session to play the file too
  \param fh file handle to use (NULL for builtin one)
  \param file the path to the file
  \param dtmf_callback code to execute if any dtmf is dialed during the playback
  \param buf an object to maintain across calls
  \param buflen the size of buf
  \return SWITCH_STATUS_SUCCESS if all is well
  \note passing a NULL dtmf_callback nad a not NULL buf indicates to copy any dtmf to buf and stop playback.
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_play_file(switch_core_session_t *session,
                                                     switch_file_handle_t *fh,
                                                     char *file,
                                                     switch_input_args_t *args);


/*!
  \brief record a file from the session to a file
  \param session the session to record from
  \param fh file handle to use
  \param file the path to the file
  \param dtmf_callback code to execute if any dtmf is dialed during the recording
  \param buf an object to maintain across calls
  \param buflen the size of buf
  \param limit max limit to record for (0 for infinite)
  \return SWITCH_STATUS_SUCCESS if all is well
  \note passing a NULL dtmf_callback nad a not NULL buf indicates to copy any dtmf to buf and stop recording.
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_record_file(switch_core_session_t *session,
                                                       switch_file_handle_t *fh,
                                                       char *file,
                                                       switch_input_args_t *args,
                                                       uint32_t limit);

/*!
 \brief Function to evaluate an expression against a string
 \param target The string to find a match in
 \param expression The regular expression to run against the string
 \return Boolean if a match was found or not
*/
SWITCH_DECLARE(switch_status_t) switch_regex_match(char *target, char *expression);

/*!
  \brief Play a sound and gather digits with the number of retries specified if the user doesn't give digits in the set time
  \param session the current session to play sound to and collect digits
  \param min_digits the fewest digits allowed for the response to be valid
  \param max_digits the max number of digits to accept
  \param max_tries number of times to replay the sound and capture digits
  \param timeout time to wait for input (this is per iteration, so total possible time = max_tries * (timeout + audio playback length)
  \param valid_terminators for input that can include # or * (useful for variable length prompts)
  \param audio_file file to play
  \param bad_input_audio_file file to play if the input from the user was invalid
  \param digit_buffer variable digits captured will be put back into (empty if capture failed)
  \param digit_buffer_length length of the buffer for digits (should be the same or larger than max_digits)
  \param digits_regex the qualifying regex
  \return switch status, used to note status of channel (will still return success if digit capture failed)
  \note to test for digit capture failure look for \\0 in the first position of the buffer
*/
SWITCH_DECLARE(switch_status_t) switch_play_and_get_digits(switch_core_session_t *session,
                                                           uint32_t min_digits,
                                                           uint32_t max_digits,
                                                           uint32_t max_tries,
                                                           uint32_t timeout,
                                                           char* valid_terminators,
                                                           char* audio_file,
                                                           char* bad_input_audio_file,
                                                           void* digit_buffer,
                                                           uint32_t digit_buffer_length,
                                                           char* digits_regex);

SWITCH_DECLARE(switch_status_t) switch_ivr_speak_text_handle(switch_core_session_t *session,
                                                             switch_speech_handle_t *sh,
                                                             switch_codec_t *codec,
                                                             switch_timer_t *timer,
                                                             char *text,
                                                             switch_input_args_t *args);

/*!
  \brief Speak given text with given tts engine
  \param session the session to speak on
  \param tts_name the desired tts module
  \param voice_name the desired voice
  \param rate the sample rate
  \param dtmf_callback code to execute if any dtmf is dialed during the audio
  \param text the text to speak
  \param buf an option data pointer to pass to the callback or a string to put encountered digits in
  \param buflen the len of buf
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_speak_text(switch_core_session_t *session, 
													  char *tts_name,
													  char *voice_name,
													  uint32_t rate,
													  char *text,
                                                      switch_input_args_t *args);

/*!
  \brief Make an outgoing call
  \param session originating session
  \param bleg B leg session
  \param cause a pointer to hold call cause
  \param bridgeto the desired remote callstring
  \param timelimit_sec timeout in seconds for outgoing call
  \param table optional state handler table to install on the channel
  \param cid_name_override override the caller id name
  \param cid_num_override override the caller id number
  \param caller_profile_override override the entire calling caller profile
  \return SWITCH_STATUS_SUCCESS if bleg is a running session.
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_originate(switch_core_session_t *session,
													 switch_core_session_t **bleg,
													 switch_call_cause_t *cause,
													 char *bridgeto,
													 uint32_t timelimit_sec,
													 const switch_state_handler_table_t *table,
													 char *cid_name_override,
													 char *cid_num_override,
													 switch_caller_profile_t *caller_profile_override);

/*!
  \brief Bridge Audio from one session to another
  \param session one session
  \param peer_session the other session
  \param dtmf_callback code to execute if any dtmf is dialed during the bridge
  \param session_data data to pass to the DTMF callback for session
  \param peer_session_data data to pass to the DTMF callback for peer_session
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_multi_threaded_bridge(switch_core_session_t *session, 
																 switch_core_session_t *peer_session,
																 switch_input_callback_function_t dtmf_callback,
																 void *session_data,
																 void *peer_session_data);

/*!
  \brief Bridge Signalling from one session to another
  \param session one session
  \param peer_session the other session
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_signal_bridge(switch_core_session_t *session, switch_core_session_t *peer_session);

/*!
  \brief Transfer an existing session to another location
  \param session the session to transfer
  \param extension the new extension
  \param dialplan the new dialplan (OPTIONAL, may be NULL)
  \param context the new context (OPTIONAL, may be NULL)
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_session_transfer(switch_core_session_t *session, char *extension, char *dialplan, char *context);

/*!
  \brief Bridge two existing sessions
  \param originator_uuid the uuid of the originator
  \param originatee_uuid the uuid of the originator
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_uuid_bridge(char *originator_uuid, char *originatee_uuid);

/*!
  \brief Signal a session to request direct media access to it's remote end
  \param uuid the uuid of the session to request
  \param flags flags to influence behaviour (SMF_REBRIDGE to rebridge the call in media mode)
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_media(char *uuid, switch_media_flag_t flags);

/*!
  \brief Signal a session to request indirect media allowing it to exchange media directly with another device
  \param uuid the uuid of the session to request
  \param flags flags to influence behaviour (SMF_REBRIDGE to rebridge the call in no_media mode)
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_nomedia(char *uuid, switch_media_flag_t flags);

/*!
  \brief Signal the session with a protocol specific hold message.
  \param uuid the uuid of the session to hold
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_hold_uuid(char *uuid);

/*!
  \brief Signal the session with a protocol specific unhold message.
  \param uuid the uuid of the session to hold
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_unhold_uuid(char *uuid);

/*!
  \brief Signal the session with a protocol specific hold message.
  \param session the session to hold
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_hold(switch_core_session_t *session);

/*!
  \brief Signal the session with a protocol specific unhold message.
  \param session the session to unhold
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_unhold(switch_core_session_t *session);

/*!
  \brief Signal the session to broadcast audio
  \param uuid the uuid of the session to broadcast on
  \param path the path data of the broadcast "/path/to/file.wav [<timer name>]" or "speak:<engine>|<voice>|<Text to say>"
  \param flags flags to send to the request (SMF_ECHO_BRIDGED to send the broadcast to both sides of the call)
  \return SWITCH_STATUS_SUCCESS if all is well
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_broadcast(char *uuid, char *path, switch_media_flag_t flags);

/*!
  \brief Transfer variables from one session to another 
  \param sessa the original session
  \param sessb the new session
  \param var the name of the variable to transfer (NULL for all)
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_transfer_variable(switch_core_session_t *sessa, switch_core_session_t *sessb, char *var);


/******************************************************************************************************/

struct switch_ivr_digit_stream_parser;
typedef struct switch_ivr_digit_stream_parser switch_ivr_digit_stream_parser_t;
struct switch_ivr_digit_stream;
typedef struct switch_ivr_digit_stream switch_ivr_digit_stream_t;
/*!
  \brief Create a digit stream parser object
  \param pool the pool to use for the new hash
  \param parser a pointer to the object pointer
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_parser_new(switch_memory_pool_t *pool, switch_ivr_digit_stream_parser_t **parser);

/*!
  \brief Destroy a digit stream parser object
  \param parser a pointer to the parser object
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_parser_destroy(switch_ivr_digit_stream_parser_t *parser);

/*!
  \brief Create a new digit stream object
  \param parser a pointer to the parser object created by switch_ivr_digit_stream_parser_new
  \param stream a pointer to the stream object pointer
  \return NULL if no match found or consumer data that was associated with a given digit string when matched
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_new(switch_ivr_digit_stream_parser_t *parser, switch_ivr_digit_stream_t **stream);

/*!
  \brief Destroys a digit stream object
  \param stream a pointer to the stream object
  \return NULL if no match found or consumer data that was associated with a given digit string when matched
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_destroy(switch_ivr_digit_stream_t *stream);

/*!
  \brief Set a digit string to action mapping
  \param parser a pointer to the parser object created by switch_ivr_digit_stream_parser_new
  \param digits a string of digits to associate with an action
  \param data consumer data attached to this digit string
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_parser_set_event(switch_ivr_digit_stream_parser_t *parser, char *digits, void *data);

/*!
  \brief Delete a string to action mapping
  \param parser a pointer to the parser object created by switch_ivr_digit_stream_parser_new
  \param digits the digit string to be removed from the map
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_parser_del_event(switch_ivr_digit_stream_parser_t *parser, char *digits);

/*!
  \brief Feed digits collected into the stream for event match testing
  \param parser a pointer to the parser object created by switch_ivr_digit_stream_parser_new
  \param digit a digit to collect and test against the map of digit strings
  \return NULL if no match found or consumer data that was associated with a given digit string when matched
*/
SWITCH_DECLARE(void *) switch_ivr_digit_stream_parser_feed(switch_ivr_digit_stream_parser_t *parser, switch_ivr_digit_stream_t *stream, char digit);

/*!
  \brief Reset the collected digit stream to nothing
  \param stream a pointer to the parser stream object created by switch_ivr_digit_stream_new
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_reset(switch_ivr_digit_stream_t *stream);

/*!
  \brief Set a digit string terminator
  \param parser a pointer to the parser object created by switch_ivr_digit_stream_parser_new
  \param digit the terminator digit
  \return SWITCH_STATUS_SUCCESS if all is well 
*/
SWITCH_DECLARE(switch_status_t) switch_ivr_digit_stream_parser_set_terminator(switch_ivr_digit_stream_parser_t *parser, char digit);


/******************************************************************************************************/


/** @} */

/**
 * @defgroup switch_ivr_menu IVR Menu Library
 * @ingroup switch_ivr
 *	IVR menu functions
 *	
 * @{
 */

typedef enum {
	SWITCH_IVR_MENU_FLAG_FALLTOMAIN = (1 << 0),
	SWITCH_IVR_MENU_FLAG_FREEPOOL = (1 << 1),
	SWITCH_IVR_MENU_FLAG_STACK = (1 << 2),
} switch_ivr_menu_flags;
/* Actions are either set in switch_ivr_menu_bind_function or returned by a callback */
typedef enum {
	SWITCH_IVR_ACTION_DIE, 		/* Exit the menu.                  */
	SWITCH_IVR_ACTION_EXECMENU,	/* Goto another menu in the stack. */
	SWITCH_IVR_ACTION_EXECAPP,	/* Execute an application.         */
	SWITCH_IVR_ACTION_PLAYSOUND,	/* Play a sound.                   */
	SWITCH_IVR_ACTION_SAYTEXT,	/* say text.                       */
	SWITCH_IVR_ACTION_BACK,		/* Go back 1 menu.                 */
	SWITCH_IVR_ACTION_TOMAIN,	/* Go back to the top level menu.  */
	SWITCH_IVR_ACTION_TRANSFER,	/* Transfer caller to another ext. */
	SWITCH_IVR_ACTION_NOOP,		/* No operation                    */
} switch_ivr_action_t;
struct switch_ivr_menu;
typedef switch_ivr_action_t switch_ivr_menu_action_function_t(struct switch_ivr_menu *, char *, char *, size_t, void *);
typedef struct switch_ivr_menu switch_ivr_menu_t;
typedef struct switch_ivr_menu_action switch_ivr_menu_action_t;
/******************************************************************************************************/

/*!
 *\brief Create a new menu object.
 *\param new_menu the pointer to the new menu
 *\param main The top level menu, (NULL if this is the top level one).
 *\param name A pointer to the name of this menu.
 *\param greeting_sound Optional pointer to a main sound (press 1 for this 2 for that).
 *\param short_greeting_sound Optional pointer to a shorter main sound for subsequent loops.
 *\param exit_sound Optional pointer to a sound to play upon exiting the menu
 *\param invalid_sound Optional pointer to a sound to play after invalid input.
 *\param tts_engine Text To Speech engine name
 *\param tts_voice Text To Speech engine voice name
 *\param timeout A number of milliseconds to pause before looping.
 *\param max_failures Maximum number of failures to withstand before hangingup This resets everytime you enter the menu.
 *\param pool memory pool (NULL to create one)
 *\return SWITCH_STATUS_SUCCESS if the menu was created
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_init(switch_ivr_menu_t **new_menu,
													 switch_ivr_menu_t *main,
													 char *name, 
													 char *greeting_sound, 
													 char *short_greeting_sound,
													 char *exit_sound,
													 char *invalid_sound, 
													 char *tts_engine,
													 char *tts_voice,
													 int timeout,
													 int max_failures, 
													 switch_memory_pool_t *pool);

/*!
 *\brief switch_ivr_menu_bind_action: Bind a keystroke to an action.
 *\param menu The menu obj you wish to bind to.
 *\param ivr_action switch_ivr_action_t enum of what you want to do.
 *\param arg Optional (sometimes necessary) string arguement.
 *\param bind KeyStrokes to bind the action to.
 *\return SWUTCH_STATUS_SUCCESS if the action was binded
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_bind_action(switch_ivr_menu_t *menu, switch_ivr_action_t ivr_action, char *arg, char *bind);


/*!
 *\brief Bind a keystroke to a callback function.
 *\param menu The menu obj you wish to bind to.
 *\param function The function to call [int proto(struct switch_ivr_menu *, char *, size_t, void *)]
 *\param arg Optional (sometimes necessary) string arguement.
 *\param bind KeyStrokes to bind the action to.
 *\note The function is passed a buffer to fill in with any required argument data.
 *\note The function is also passed an optional void pointer to an object set upon menu execution. (think threads)
 *\note The function returns an switch_ivr_action_t enum of what you want to do. and looks to your buffer for args.
 *\return SWUTCH_STATUS_SUCCESS if the function was binded
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_bind_function(switch_ivr_menu_t *menu, switch_ivr_menu_action_function_t *function, char *arg, char *bind);


/*!
 *\brief Execute a menu.
 *\param session The session running the menu.
 *\param stack The top-level menu object (the first one you created.)
 *\param name A pointer to the name of the menu.
 *\param obj A void pointer to an object you want to make avaliable to your callback functions that you may have binded with switch_ivr_menu_bind_function.
 *\return SWITCH_STATUS_SUCCESS if all is well
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_execute(switch_core_session_t *session, switch_ivr_menu_t *stack, char *name, void *obj);

/*!
 *\brief free a stack of menu objects.
 *\param stack The top level menu you wish to destroy.
 *\return SWITCH_STATUS_SUCCESS if the object was a top level menu and it was freed
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_stack_free(switch_ivr_menu_t *stack);

struct switch_ivr_menu_xml_ctx;
typedef struct switch_ivr_menu_xml_ctx switch_ivr_menu_xml_ctx_t;
/*!
 *\brief Build a menu stack from an xml source
 *\param xml_menu_ctx The XML menu parser context previously created by switch_ivr_menu_stack_xml_init
 *\param menu_stack The menu stack object that will be created for you
 *\param xml_menus The xml Menus source
 *\param xml_menu The xml Menu source of the menu to be created
 *\return SWITCH_STATUS_SUCCESS if all is well
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_stack_xml_build(switch_ivr_menu_xml_ctx_t *xml_menu_ctx,
                                                                switch_ivr_menu_t **menu_stack,
                                                                switch_xml_t xml_menus,
                                                                switch_xml_t xml_menu);

/*!
 *\param xml_menu_ctx The XML menu parser context previously created by switch_ivr_menu_stack_xml_init
 *\param name The xml tag name to add to the parser engine
 *\param function The menu function callback that will be executed when menu digits are bound to this name
 *\return SWITCH_STATUS_SUCCESS if all is well
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_stack_xml_add_custom(switch_ivr_menu_xml_ctx_t *xml_menu_ctx,
										char *name,
										switch_ivr_menu_action_function_t *function);

/*!
 *\param xml_menu_ctx A pointer of a XML menu parser context to be created
 *\param pool memory pool (NULL to create one)
 *\return SWITCH_STATUS_SUCCESS if all is well
 */
SWITCH_DECLARE(switch_status_t) switch_ivr_menu_stack_xml_init(switch_ivr_menu_xml_ctx_t **xml_menu_ctx, switch_memory_pool_t *pool);

SWITCH_DECLARE(switch_status_t) switch_ivr_phrase_macro(switch_core_session_t *session,
                                                        char *macro_name,
                                                        char *data,
                                                        char *lang,
                                                        switch_input_args_t *args);
/** @} */

SWITCH_END_EXTERN_C

#endif

/* For Emacs:
 * Local Variables:
 * mode:c
 * indent-tabs-mode:nil
 * tab-width:4
 * c-basic-offset:4
 * End:
 * For VIM:
 * vim:set softtabstop=4 shiftwidth=4 tabstop=4 expandtab:
 */
