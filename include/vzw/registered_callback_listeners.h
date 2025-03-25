/** @file registered_callback_listeners.h
 *  @brief Macro and function defines for the vzw thingspace registered callback listeners API
 */

#ifndef REGISTERED_CALLBACK_LISTENERS_H
#define REGISTERED_CALLBACK_LISTENERS_H

int vzw_get_registered_callback_listeners(
    const char *account_name, char *auth_token, char *session_token, void *response_data
);

int vzw_set_registered_callback_listeners(
    const char *account_name, char *auth_token, char *session_token, void *response_data,
    char *service_name, char *url
);

int vzw_delete_registered_callback_listeners(
    const char *account_name, char *auth_token, char *session_token, void *response_data,
    char *service_name
);

#endif  // REGISTERED_CALLBACK_LISTENERS_H
